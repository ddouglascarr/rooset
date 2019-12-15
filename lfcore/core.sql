
-- Execute the following command manually for PostgreSQL prior version 9.0:
-- CREATE LANGUAGE plpgsql;

-- NOTE: In PostgreSQL every UNIQUE constraint implies creation of an index

BEGIN;

CREATE VIEW "liquid_feedback_version" AS
  SELECT * FROM (VALUES ('3.2.2', 3, 2, 2))
  AS "subquery"("string", "major", "minor", "revision");



----------------------
-- Full text search --
----------------------


CREATE FUNCTION "text_search_query"("query_text_p" TEXT)
  RETURNS TSQUERY
  LANGUAGE 'plpgsql' IMMUTABLE AS $$
    BEGIN
      RETURN plainto_tsquery('pg_catalog.simple', "query_text_p");
    END;
  $$;

COMMENT ON FUNCTION "text_search_query"(TEXT) IS 'Usage: WHERE "text_search_data" @@ "text_search_query"(''<user query>'')';


CREATE FUNCTION "highlight"
  ( "body_p"       TEXT,
    "query_text_p" TEXT )
  RETURNS TEXT
  LANGUAGE 'plpgsql' IMMUTABLE AS $$
    BEGIN
      RETURN ts_headline(
        'pg_catalog.simple',
        replace(replace("body_p", e'\\', e'\\\\'), '*', e'\\*'),
        "text_search_query"("query_text_p"),
        'StartSel=* StopSel=* HighlightAll=TRUE' );
    END;
  $$;

COMMENT ON FUNCTION "highlight"
  ( "body_p"       TEXT,
    "query_text_p" TEXT )
  IS 'For a given a user query this function encapsulates all matches with asterisks. Asterisks and backslashes being already present are preceeded with one extra backslash.';



-------------------------
-- Tables and indicies --
-------------------------


CREATE TABLE "temporary_transaction_data" (
        PRIMARY KEY ("txid", "key"),
        "txid"                  INT8            DEFAULT txid_current(),
        "key"                   TEXT,
        "value"                 TEXT            NOT NULL );

COMMENT ON TABLE "temporary_transaction_data" IS 'Table to store temporary transaction data; shall be emptied before a transaction is committed';

COMMENT ON COLUMN "temporary_transaction_data"."txid" IS 'Value returned by function txid_current(); should be added to WHERE clause, when doing SELECT on this table, but ignored when doing DELETE on this table';


CREATE TABLE "system_setting" (
        "member_ttl"            INTERVAL );
CREATE UNIQUE INDEX "system_setting_singleton_idx" ON "system_setting" ((1));

COMMENT ON TABLE "system_setting" IS 'This table contains only one row with different settings in each column.';
COMMENT ON INDEX "system_setting_singleton_idx" IS 'This index ensures that "system_setting" only contains one row maximum.';

COMMENT ON COLUMN "system_setting"."member_ttl" IS 'Time after members get their "active" flag set to FALSE, if they do not show any activity.';


CREATE TABLE "contingent" (
        PRIMARY KEY ("polling", "time_frame"),
        "polling"               BOOLEAN,
        "time_frame"            INTERVAL,
        "text_entry_limit"      INT4,
        "initiative_limit"      INT4 );

COMMENT ON TABLE "contingent" IS 'Amount of text entries or initiatives a user may create within a given time frame. Only one row needs to be fulfilled for a member to be allowed to post. This table must not be empty.';

COMMENT ON COLUMN "contingent"."polling"          IS 'Determines if settings are for creating initiatives and new drafts of initiatives with "polling" flag set';
COMMENT ON COLUMN "contingent"."text_entry_limit" IS 'Number of new drafts or suggestions to be submitted by each member within the given time frame';
COMMENT ON COLUMN "contingent"."initiative_limit" IS 'Number of new initiatives to be opened by each member within a given time frame';


CREATE TABLE "member" (
        "id"                    SERIAL4         PRIMARY KEY,
        "created"               TIMESTAMPTZ     NOT NULL DEFAULT now(),
        "invite_code"           TEXT            UNIQUE,
        "invite_code_expiry"    TIMESTAMPTZ,
        "admin_comment"         TEXT,
        "activated"             TIMESTAMPTZ,
        "last_activity"         DATE,
        "last_login"            TIMESTAMPTZ,
        "last_delegation_check" TIMESTAMPTZ,
        "login"                 TEXT            UNIQUE,
        "password"              TEXT,
        "authority"             TEXT,
        "authority_uid"         TEXT,
        "authority_login"       TEXT,
        "locked"                BOOLEAN         NOT NULL DEFAULT FALSE,
        "active"                BOOLEAN         NOT NULL DEFAULT FALSE,
        "admin"                 BOOLEAN         NOT NULL DEFAULT FALSE,
        "lang"                  TEXT,
        "notify_email"          TEXT,
        "notify_email_unconfirmed"     TEXT,
        "notify_email_secret"          TEXT     UNIQUE,
        "notify_email_secret_expiry"   TIMESTAMPTZ,
        "notify_email_lock_expiry"     TIMESTAMPTZ,
        "disable_notifications"    BOOLEAN      NOT NULL DEFAULT FALSE,
        "notification_counter"     INT4         NOT NULL DEFAULT 1,
        "notification_sample_size" INT4         NOT NULL DEFAULT 3,
        "notification_dow"         INT4         CHECK ("notification_dow" BETWEEN 0 AND 6),
        "notification_hour"        INT4         DEFAULT floor(random() * 24) CHECK ("notification_hour" BETWEEN 0 AND 23),
        "notification_sent"        TIMESTAMP,
        "login_recovery_expiry"        TIMESTAMPTZ,
        "password_reset_secret"        TEXT     UNIQUE,
        "password_reset_secret_expiry" TIMESTAMPTZ,
        "name"                  TEXT            UNIQUE,
        "identification"        TEXT            UNIQUE,
        "authentication"        TEXT,
        "organizational_unit"   TEXT,
        "internal_posts"        TEXT,
        "realname"              TEXT,
        "birthday"              DATE,
        "address"               TEXT,
        "email"                 TEXT,
        "xmpp_address"          TEXT,
        "website"               TEXT,
        "phone"                 TEXT,
        "mobile_phone"          TEXT,
        "profession"            TEXT,
        "external_memberships"  TEXT,
        "external_posts"        TEXT,
        "formatting_engine"     TEXT,
        "statement"             TEXT,
        "text_search_data"      TSVECTOR,
        CONSTRAINT "active_requires_activated_and_last_activity"
          CHECK ("active" = FALSE OR ("activated" NOTNULL AND "last_activity" NOTNULL)),
        CONSTRAINT "authority_requires_uid_and_vice_versa" 
          CHECK (("authority" NOTNULL) = ("authority_uid" NOTNULL)),
        CONSTRAINT "authority_uid_unique_per_authority"
          UNIQUE ("authority", "authority_uid"),
        CONSTRAINT "authority_login_requires_authority"
          CHECK ("authority" NOTNULL OR "authority_login" ISNULL),
        CONSTRAINT "notification_dow_requires_notification_hour"
          CHECK ("notification_dow" ISNULL OR "notification_hour" NOTNULL),
        CONSTRAINT "name_not_null_if_activated"
          CHECK ("activated" ISNULL OR "name" NOTNULL) );
CREATE INDEX "member_authority_login_idx" ON "member" ("authority_login");
CREATE INDEX "member_active_idx" ON "member" ("active");
CREATE INDEX "member_text_search_data_idx" ON "member" USING gin ("text_search_data");
CREATE TRIGGER "update_text_search_data"
  BEFORE INSERT OR UPDATE ON "member"
  FOR EACH ROW EXECUTE PROCEDURE
  tsvector_update_trigger('text_search_data', 'pg_catalog.simple',
    "name", "identification", "organizational_unit", "internal_posts",
    "realname", "external_memberships", "external_posts", "statement" );

COMMENT ON TABLE "member" IS 'Users of the system, e.g. members of an organization';

COMMENT ON COLUMN "member"."created"              IS 'Creation of member record and/or invite code';
COMMENT ON COLUMN "member"."invite_code"          IS 'Optional invite code, to allow a member to initialize his/her account the first time';
COMMENT ON COLUMN "member"."invite_code_expiry"   IS 'Expiry data/time for "invite_code"';
COMMENT ON COLUMN "member"."admin_comment"        IS 'Hidden comment for administrative purposes';
COMMENT ON COLUMN "member"."activated"            IS 'Timestamp of first activation of account (i.e. usage of "invite_code"); required to be set for "active" members';
COMMENT ON COLUMN "member"."last_activity"        IS 'Date of last activity of member; required to be set for "active" members';
COMMENT ON COLUMN "member"."last_login"           IS 'Timestamp of last login';
COMMENT ON COLUMN "member"."last_delegation_check" IS 'Timestamp of last delegation check (i.e. confirmation of all unit and area delegations)';
COMMENT ON COLUMN "member"."login"                IS 'Login name';
COMMENT ON COLUMN "member"."password"             IS 'Password (preferably as crypto-hash, depending on the frontend or access layer)';
COMMENT ON COLUMN "member"."authority"            IS 'NULL if LiquidFeedback Core is authoritative for the member account; otherwise a string that indicates the source/authority of the external account (e.g. ''LDAP'' for an LDAP account)';
COMMENT ON COLUMN "member"."authority_uid"        IS 'Unique identifier (unique per "authority") that allows to identify an external account (e.g. even if the login name changes)';
COMMENT ON COLUMN "member"."authority_login"      IS 'Login name for external accounts (field is not unique!)';
COMMENT ON COLUMN "member"."locked"               IS 'Locked members can not log in.';
COMMENT ON COLUMN "member"."active"               IS 'Memberships, support and votes are taken into account when corresponding members are marked as active. Automatically set to FALSE, if "last_activity" is older than "system_setting"."member_ttl".';
COMMENT ON COLUMN "member"."admin"                IS 'TRUE for admins, which can administrate other users and setup policies and areas';
COMMENT ON COLUMN "member"."lang"                 IS 'Language code of the preferred language of the member';
COMMENT ON COLUMN "member"."notify_email"         IS 'Email address where notifications of the system are sent to';
COMMENT ON COLUMN "member"."notify_email_unconfirmed"   IS 'Unconfirmed email address provided by the member to be copied into "notify_email" field after verification';
COMMENT ON COLUMN "member"."notify_email_secret"        IS 'Secret sent to the address in "notify_email_unconformed"';
COMMENT ON COLUMN "member"."notify_email_secret_expiry" IS 'Expiry date/time for "notify_email_secret"';
COMMENT ON COLUMN "member"."notify_email_lock_expiry"   IS 'Date/time until no further email confirmation mails may be sent (abuse protection)';
COMMENT ON COLUMN "member"."disable_notifications"    IS 'TRUE if member does not want to receive notifications';
COMMENT ON COLUMN "member"."notification_counter"     IS 'Sequential number of next scheduled notification message (used as a seed for pseudo-random initiative selection algorithm)';
COMMENT ON COLUMN "member"."notification_sample_size" IS 'Number of featured initiatives per issue in scheduled notification messages';
COMMENT ON COLUMN "member"."notification_dow"         IS 'Day of week for scheduled notifications (NULL to receive a daily digest)';
COMMENT ON COLUMN "member"."notification_hour"        IS 'Time of day when scheduled notifications are sent out';
COMMENT ON COLUMN "member"."notification_sent"        IS 'Timestamp of last scheduled notification mail that has been sent out';
COMMENT ON COLUMN "member"."login_recovery_expiry"        IS 'Date/time after which another login recovery attempt is allowed';
COMMENT ON COLUMN "member"."password_reset_secret"        IS 'Secret string sent via e-mail for password recovery';
COMMENT ON COLUMN "member"."password_reset_secret_expiry" IS 'Date/time until the password recovery secret is valid, and date/time after which another password recovery attempt is allowed';
COMMENT ON COLUMN "member"."name"                 IS 'Distinct name of the member, may be NULL if account has not been activated yet';
COMMENT ON COLUMN "member"."identification"       IS 'Optional identification number or code of the member';
COMMENT ON COLUMN "member"."authentication"       IS 'Information about how this member was authenticated';
COMMENT ON COLUMN "member"."organizational_unit"  IS 'Branch or division of the organization the member belongs to';
COMMENT ON COLUMN "member"."internal_posts"       IS 'Posts (offices) of the member inside the organization';
COMMENT ON COLUMN "member"."realname"             IS 'Real name of the member, may be identical with "name"';
COMMENT ON COLUMN "member"."email"                IS 'Published email address of the member; not used for system notifications';
COMMENT ON COLUMN "member"."external_memberships" IS 'Other organizations the member is involved in';
COMMENT ON COLUMN "member"."external_posts"       IS 'Posts (offices) outside the organization';
COMMENT ON COLUMN "member"."formatting_engine"    IS 'Allows different formatting engines (i.e. wiki formats) to be used for "member"."statement"';
COMMENT ON COLUMN "member"."statement"            IS 'Freely chosen text of the member for his/her profile';


CREATE TABLE "member_history" (
        "id"                    SERIAL8         PRIMARY KEY,
        "member_id"             INT4            NOT NULL REFERENCES "member" ("id") ON DELETE CASCADE ON UPDATE CASCADE,
        "until"                 TIMESTAMPTZ     NOT NULL DEFAULT now(),
        "active"                BOOLEAN         NOT NULL,
        "name"                  TEXT            NOT NULL );
CREATE INDEX "member_history_member_id_idx" ON "member_history" ("member_id");

COMMENT ON TABLE "member_history" IS 'Filled by trigger; keeps information about old names and active flag of members';

COMMENT ON COLUMN "member_history"."id"    IS 'Primary key, which can be used to sort entries correctly (and time warp resistant)';
COMMENT ON COLUMN "member_history"."until" IS 'Timestamp until the data was valid';


CREATE TABLE "rendered_member_statement" (
        PRIMARY KEY ("member_id", "format"),
        "member_id"             INT4            REFERENCES "member" ("id") ON DELETE CASCADE ON UPDATE CASCADE,
        "format"                TEXT,
        "content"               TEXT            NOT NULL );

COMMENT ON TABLE "rendered_member_statement" IS 'This table may be used by frontends to cache "rendered" member statements (e.g. HTML output generated from wiki text)';


CREATE TABLE "setting" (
        PRIMARY KEY ("member_id", "key"),
        "member_id"             INT4            REFERENCES "member" ("id") ON DELETE CASCADE ON UPDATE CASCADE,
        "key"                   TEXT            NOT NULL,
        "value"                 TEXT            NOT NULL );
CREATE INDEX "setting_key_idx" ON "setting" ("key");

COMMENT ON TABLE "setting" IS 'Place to store a frontend specific setting for members as a string';

COMMENT ON COLUMN "setting"."key" IS 'Name of the setting, preceded by a frontend specific prefix';


CREATE TABLE "setting_map" (
        PRIMARY KEY ("member_id", "key", "subkey"),
        "member_id"             INT4            REFERENCES "member" ("id") ON DELETE CASCADE ON UPDATE CASCADE,
        "key"                   TEXT            NOT NULL,
        "subkey"                TEXT            NOT NULL,
        "value"                 TEXT            NOT NULL );
CREATE INDEX "setting_map_key_idx" ON "setting_map" ("key");

COMMENT ON TABLE "setting_map" IS 'Place to store a frontend specific setting for members as a map of key value pairs';

COMMENT ON COLUMN "setting_map"."key"    IS 'Name of the setting, preceded by a frontend specific prefix';
COMMENT ON COLUMN "setting_map"."subkey" IS 'Key of a map entry';
COMMENT ON COLUMN "setting_map"."value"  IS 'Value of a map entry';


CREATE TABLE "member_relation_setting" (
        PRIMARY KEY ("member_id", "key", "other_member_id"),
        "member_id"             INT4            REFERENCES "member" ("id") ON DELETE CASCADE ON UPDATE CASCADE,
        "key"                   TEXT            NOT NULL,
        "other_member_id"       INT4            REFERENCES "member" ("id") ON DELETE CASCADE ON UPDATE CASCADE,
        "value"                 TEXT            NOT NULL );

COMMENT ON TABLE "member_relation_setting" IS 'Place to store a frontend specific setting related to relations between members as a string';


CREATE TYPE "member_image_type" AS ENUM ('photo', 'avatar');

COMMENT ON TYPE "member_image_type" IS 'Types of images for a member';


CREATE TABLE "member_image" (
        PRIMARY KEY ("member_id", "image_type", "scaled"),
        "member_id"             INT4            REFERENCES "member" ("id") ON DELETE CASCADE ON UPDATE CASCADE,
        "image_type"            "member_image_type",
        "scaled"                BOOLEAN,
        "content_type"          TEXT,
        "data"                  BYTEA           NOT NULL );

COMMENT ON TABLE "member_image" IS 'Images of members';

COMMENT ON COLUMN "member_image"."scaled" IS 'FALSE for original image, TRUE for scaled version of the image';


CREATE TABLE "member_count" (
        "calculated"            TIMESTAMPTZ     NOT NULL DEFAULT now(),
        "total_count"           INT4            NOT NULL );

COMMENT ON TABLE "member_count" IS 'Contains one row which contains the total count of active(!) members and a timestamp indicating when the total member count and area member counts were calculated';

COMMENT ON COLUMN "member_count"."calculated"  IS 'timestamp indicating when the total member count and area member counts were calculated';
COMMENT ON COLUMN "member_count"."total_count" IS 'Total count of active(!) members';


CREATE TABLE "contact" (
        PRIMARY KEY ("member_id", "other_member_id"),
        "member_id"             INT4            REFERENCES "member" ("id") ON DELETE CASCADE ON UPDATE CASCADE,
        "other_member_id"       INT4            REFERENCES "member" ("id") ON DELETE CASCADE ON UPDATE CASCADE,
        "public"                BOOLEAN         NOT NULL DEFAULT FALSE,
        CONSTRAINT "cant_save_yourself_as_contact"
          CHECK ("member_id" != "other_member_id") );
CREATE INDEX "contact_other_member_id_idx" ON "contact" ("other_member_id");

COMMENT ON TABLE "contact" IS 'Contact lists';

COMMENT ON COLUMN "contact"."member_id"       IS 'Member having the contact list';
COMMENT ON COLUMN "contact"."other_member_id" IS 'Member referenced in the contact list';
COMMENT ON COLUMN "contact"."public"          IS 'TRUE = display contact publically';


CREATE TABLE "ignored_member" (
        PRIMARY KEY ("member_id", "other_member_id"),
        "member_id"             INT4            REFERENCES "member" ("id") ON DELETE CASCADE ON UPDATE CASCADE,
        "other_member_id"       INT4            REFERENCES "member" ("id") ON DELETE CASCADE ON UPDATE CASCADE );
CREATE INDEX "ignored_member_other_member_id_idx" ON "ignored_member" ("other_member_id");

COMMENT ON TABLE "ignored_member" IS 'Possibility to filter other members';

COMMENT ON COLUMN "ignored_member"."member_id"       IS 'Member ignoring someone';
COMMENT ON COLUMN "ignored_member"."other_member_id" IS 'Member being ignored';


CREATE TABLE "session" (
        "ident"                 TEXT            PRIMARY KEY,
        "additional_secret"     TEXT,
        "expiry"                TIMESTAMPTZ     NOT NULL DEFAULT now() + '24 hours',
        "member_id"             INT4            REFERENCES "member" ("id") ON DELETE SET NULL,
        "authority"             TEXT,
        "authority_uid"         TEXT,
        "authority_login"       TEXT,
        "needs_delegation_check" BOOLEAN        NOT NULL DEFAULT FALSE,
        "lang"                  TEXT );
CREATE INDEX "session_expiry_idx" ON "session" ("expiry");

COMMENT ON TABLE "session" IS 'Sessions, i.e. for a web-frontend or API layer';

COMMENT ON COLUMN "session"."ident"             IS 'Secret session identifier (i.e. random string)';
COMMENT ON COLUMN "session"."additional_secret" IS 'Additional field to store a secret, which can be used against CSRF attacks';
COMMENT ON COLUMN "session"."member_id"         IS 'Reference to member, who is logged in';
COMMENT ON COLUMN "session"."authority"         IS 'Temporary store for "member"."authority" during member account creation';
COMMENT ON COLUMN "session"."authority_uid"     IS 'Temporary store for "member"."authority_uid" during member account creation';
COMMENT ON COLUMN "session"."authority_login"   IS 'Temporary store for "member"."authority_login" during member account creation';
COMMENT ON COLUMN "session"."needs_delegation_check" IS 'Set to TRUE, if member must perform a delegation check to proceed with login; see column "last_delegation_check" in "member" table';
COMMENT ON COLUMN "session"."lang"              IS 'Language code of the selected language';


CREATE TYPE "defeat_strength" AS ENUM ('simple', 'tuple');

COMMENT ON TYPE "defeat_strength" IS 'How pairwise defeats are measured for the Schulze method: ''simple'' = only the number of winning votes, ''tuple'' = primarily the number of winning votes, secondarily the number of losing votes';


CREATE TYPE "tie_breaking" AS ENUM ('simple', 'variant1', 'variant2');

COMMENT ON TYPE "tie_breaking" IS 'Tie-breaker for the Schulze method: ''simple'' = only initiative ids are used, ''variant1'' = use initiative ids in variant 1 for tie breaking of the links (TBRL) and sequentially forbid shared links, ''variant2'' = use initiative ids in variant 2 for tie breaking of the links (TBRL) and sequentially forbid shared links';


CREATE TABLE "policy" (
        "id"                    SERIAL4         PRIMARY KEY,
        "index"                 INT4            NOT NULL,
        "active"                BOOLEAN         NOT NULL DEFAULT TRUE,
        "name"                  TEXT            NOT NULL UNIQUE,
        "description"           TEXT            NOT NULL DEFAULT '',
        "polling"               BOOLEAN         NOT NULL DEFAULT FALSE,
        "min_admission_time"    INTERVAL,
        "max_admission_time"    INTERVAL,
        "discussion_time"       INTERVAL,
        "verification_time"     INTERVAL,
        "voting_time"           INTERVAL,
        "issue_quorum_num"      INT4,
        "issue_quorum_den"      INT4,
        "initiative_quorum_num" INT4            NOT NULL,
        "initiative_quorum_den" INT4            NOT NULL,
        "defeat_strength"     "defeat_strength" NOT NULL DEFAULT 'tuple',
        "tie_breaking"          "tie_breaking"  NOT NULL DEFAULT 'variant1',
        "direct_majority_num"           INT4    NOT NULL DEFAULT 1,
        "direct_majority_den"           INT4    NOT NULL DEFAULT 2,
        "direct_majority_strict"        BOOLEAN NOT NULL DEFAULT TRUE,
        "direct_majority_positive"      INT4    NOT NULL DEFAULT 0,
        "direct_majority_non_negative"  INT4    NOT NULL DEFAULT 0,
        "indirect_majority_num"         INT4    NOT NULL DEFAULT 1,
        "indirect_majority_den"         INT4    NOT NULL DEFAULT 2,
        "indirect_majority_strict"      BOOLEAN NOT NULL DEFAULT TRUE,
        "indirect_majority_positive"    INT4    NOT NULL DEFAULT 0,
        "indirect_majority_non_negative" INT4   NOT NULL DEFAULT 0,
        "no_reverse_beat_path"          BOOLEAN NOT NULL DEFAULT FALSE,
        "no_multistage_majority"        BOOLEAN NOT NULL DEFAULT FALSE,
        CONSTRAINT "timing" CHECK (
          ( "polling" = FALSE AND
            "min_admission_time" NOTNULL AND "max_admission_time" NOTNULL AND
            "min_admission_time" <= "max_admission_time" AND
            "discussion_time" NOTNULL AND
            "verification_time" NOTNULL AND
            "voting_time" NOTNULL ) OR
          ( "polling" = TRUE AND
            "min_admission_time" ISNULL AND "max_admission_time" ISNULL AND
            "discussion_time" NOTNULL AND
            "verification_time" NOTNULL AND
            "voting_time" NOTNULL ) OR
          ( "polling" = TRUE AND
            "min_admission_time" ISNULL AND "max_admission_time" ISNULL AND
            "discussion_time" ISNULL AND
            "verification_time" ISNULL AND
            "voting_time" ISNULL ) ),
        CONSTRAINT "issue_quorum_if_and_only_if_not_polling" CHECK (
          "polling" = ("issue_quorum_num" ISNULL) AND
          "polling" = ("issue_quorum_den" ISNULL) ),
        CONSTRAINT "no_reverse_beat_path_requires_tuple_defeat_strength" CHECK (
          "defeat_strength" = 'tuple'::"defeat_strength" OR
          "no_reverse_beat_path" = FALSE ) );
CREATE INDEX "policy_active_idx" ON "policy" ("active");

COMMENT ON TABLE "policy" IS 'Policies for a particular proceeding type (timelimits, quorum)';

COMMENT ON COLUMN "policy"."index"                 IS 'Determines the order in listings';
COMMENT ON COLUMN "policy"."active"                IS 'TRUE = policy can be used for new issues';
COMMENT ON COLUMN "policy"."polling"               IS 'TRUE = special policy for non-user-generated issues without issue quorum, where certain initiatives (those having the "polling" flag set) do not need to pass the initiative quorum; "min_admission_time" and "max_admission_time" MUST be set to NULL, the other timings may be set to NULL altogether, allowing individual timing for those issues';
COMMENT ON COLUMN "policy"."min_admission_time"    IS 'Minimum duration of issue state ''admission''; Minimum time an issue stays open';
COMMENT ON COLUMN "policy"."max_admission_time"    IS 'Maximum duration of issue state ''admission''; Maximum time an issue stays open without being "accepted"';
COMMENT ON COLUMN "policy"."discussion_time"       IS 'Duration of issue state ''discussion''; Regular time until an issue is "half_frozen" after being "accepted"';
COMMENT ON COLUMN "policy"."verification_time"     IS 'Duration of issue state ''verification''; Regular time until an issue is "fully_frozen" (e.g. entering issue state ''voting'') after being "half_frozen"';
COMMENT ON COLUMN "policy"."voting_time"           IS 'Duration of issue state ''voting''; Time after an issue is "fully_frozen" but not "closed" (duration of issue state ''voting'')';
COMMENT ON COLUMN "policy"."issue_quorum_num"      IS   'Numerator of potential supporter quorum to be reached by one initiative of an issue to be "accepted" and enter issue state ''discussion''';
COMMENT ON COLUMN "policy"."issue_quorum_den"      IS 'Denominator of potential supporter quorum to be reached by one initiative of an issue to be "accepted" and enter issue state ''discussion''';
COMMENT ON COLUMN "policy"."initiative_quorum_num" IS   'Numerator of satisfied supporter quorum  to be reached by an initiative to be "admitted" for voting';
COMMENT ON COLUMN "policy"."initiative_quorum_den" IS 'Denominator of satisfied supporter quorum to be reached by an initiative to be "admitted" for voting';
COMMENT ON COLUMN "policy"."defeat_strength"       IS 'How pairwise defeats are measured for the Schulze method; see type "defeat_strength"; ''tuple'' is the recommended setting';
COMMENT ON COLUMN "policy"."tie_breaking"          IS 'Tie-breaker for the Schulze method; see type "tie_breaking"; ''variant1'' or ''variant2'' are recommended';
COMMENT ON COLUMN "policy"."direct_majority_num"            IS 'Numerator of fraction of neccessary direct majority for initiatives to be attainable as winner';
COMMENT ON COLUMN "policy"."direct_majority_den"            IS 'Denominator of fraction of neccessary direct majority for initaitives to be attainable as winner';
COMMENT ON COLUMN "policy"."direct_majority_strict"         IS 'If TRUE, then the direct majority must be strictly greater than "direct_majority_num"/"direct_majority_den", otherwise it may also be equal.';
COMMENT ON COLUMN "policy"."direct_majority_positive"       IS 'Absolute number of "positive_votes" neccessary for an initiative to be attainable as winner';
COMMENT ON COLUMN "policy"."direct_majority_non_negative"   IS 'Absolute number of sum of "positive_votes" and abstentions neccessary for an initiative to be attainable as winner';
COMMENT ON COLUMN "policy"."indirect_majority_num"          IS 'Numerator of fraction of neccessary indirect majority (through beat path) for initiatives to be attainable as winner';
COMMENT ON COLUMN "policy"."indirect_majority_den"          IS 'Denominator of fraction of neccessary indirect majority (through beat path) for initiatives to be attainable as winner';
COMMENT ON COLUMN "policy"."indirect_majority_strict"       IS 'If TRUE, then the indirect majority must be strictly greater than "indirect_majority_num"/"indirect_majority_den", otherwise it may also be equal.';
COMMENT ON COLUMN "policy"."indirect_majority_positive"     IS 'Absolute number of votes in favor of the winner neccessary in a beat path to the status quo for an initaitive to be attainable as winner';
COMMENT ON COLUMN "policy"."indirect_majority_non_negative" IS 'Absolute number of sum of votes in favor and abstentions in a beat path to the status quo for an initiative to be attainable as winner';
COMMENT ON COLUMN "policy"."no_reverse_beat_path" IS 'EXPERIMENTAL FEATURE: Causes initiatives with "reverse_beat_path" flag to not be "eligible", thus disallowing them to be winner. See comment on column "initiative"."reverse_beat_path". This option ensures both that a winning initiative is never tied in a (weak) condorcet paradox with the status quo and a winning initiative always beats the status quo directly with a simple majority.';
COMMENT ON COLUMN "policy"."no_multistage_majority" IS 'EXPERIMENTAL FEATURE: Causes initiatives with "multistage_majority" flag to not be "eligible", thus disallowing them to be winner. See comment on column "initiative"."multistage_majority". This disqualifies initiatives which could cause an instable result. An instable result in this meaning is a result such that repeating the ballot with same preferences but with the winner of the first ballot as status quo would lead to a different winner in the second ballot. If there are no direct majorities required for the winner, or if in direct comparison only simple majorities are required and "no_reverse_beat_path" is true, then results are always stable and this flag does not have any effect on the winner (but still affects the "eligible" flag of an "initiative").';


CREATE TABLE "unit" (
        "id"                    SERIAL4         PRIMARY KEY,
        "parent_id"             INT4            REFERENCES "unit" ("id") ON DELETE CASCADE ON UPDATE CASCADE,
        "active"                BOOLEAN         NOT NULL DEFAULT TRUE,
        "name"                  TEXT            NOT NULL,
        "description"           TEXT            NOT NULL DEFAULT '',
        "external_reference"    TEXT,
        "member_count"          INT4,
        "text_search_data"      TSVECTOR );
CREATE INDEX "unit_root_idx" ON "unit" ("id") WHERE "parent_id" ISNULL;
CREATE INDEX "unit_parent_id_idx" ON "unit" ("parent_id");
CREATE INDEX "unit_active_idx" ON "unit" ("active");
CREATE INDEX "unit_text_search_data_idx" ON "unit" USING gin ("text_search_data");
CREATE TRIGGER "update_text_search_data"
  BEFORE INSERT OR UPDATE ON "unit"
  FOR EACH ROW EXECUTE PROCEDURE
  tsvector_update_trigger('text_search_data', 'pg_catalog.simple',
    "name", "description" );

COMMENT ON TABLE "unit" IS 'Organizational units organized as trees; Delegations are not inherited through these trees.';

COMMENT ON COLUMN "unit"."parent_id"          IS 'Parent id of tree node; Multiple roots allowed';
COMMENT ON COLUMN "unit"."active"             IS 'TRUE means new issues can be created in areas of this unit';
COMMENT ON COLUMN "unit"."external_reference" IS 'Opaque data field to store an external reference';
COMMENT ON COLUMN "unit"."member_count"       IS 'Count of members as determined by column "voting_right" in table "privilege"';


CREATE TABLE "subscription" (
        PRIMARY KEY ("member_id", "unit_id"),
        "member_id"             INT4            REFERENCES "member" ("id") ON DELETE CASCADE ON UPDATE CASCADE,
        "unit_id"               INT4            REFERENCES "unit" ("id") ON DELETE CASCADE ON UPDATE CASCADE );
CREATE INDEX "subscription_unit_id_idx" ON "subscription" ("unit_id");

COMMENT ON TABLE "subscription" IS 'An entry in this table denotes that the member wishes to receive notifications regardless of his/her privileges in the given unit';


CREATE TABLE "unit_setting" (
        PRIMARY KEY ("member_id", "key", "unit_id"),
        "member_id"             INT4            REFERENCES "member" ("id") ON DELETE CASCADE ON UPDATE CASCADE,
        "key"                   TEXT            NOT NULL,
        "unit_id"               INT4            REFERENCES "unit" ("id") ON DELETE CASCADE ON UPDATE CASCADE,
        "value"                 TEXT            NOT NULL );

COMMENT ON TABLE "unit_setting" IS 'Place for frontend to store unit specific settings of members as strings';


CREATE TABLE "area" (
        "id"                    SERIAL4         PRIMARY KEY,
        "unit_id"               INT4            NOT NULL REFERENCES "unit" ("id") ON DELETE CASCADE ON UPDATE CASCADE,
        "active"                BOOLEAN         NOT NULL DEFAULT TRUE,
        "name"                  TEXT            NOT NULL,
        "description"           TEXT            NOT NULL DEFAULT '',
        "external_reference"    TEXT,
        "direct_member_count"   INT4,
        "member_weight"         INT4,
        "text_search_data"      TSVECTOR );
CREATE INDEX "area_unit_id_idx" ON "area" ("unit_id");
CREATE INDEX "area_active_idx" ON "area" ("active");
CREATE INDEX "area_text_search_data_idx" ON "area" USING gin ("text_search_data");
CREATE TRIGGER "update_text_search_data"
  BEFORE INSERT OR UPDATE ON "area"
  FOR EACH ROW EXECUTE PROCEDURE
  tsvector_update_trigger('text_search_data', 'pg_catalog.simple',
    "name", "description" );

COMMENT ON TABLE "area" IS 'Subject areas';

COMMENT ON COLUMN "area"."active"              IS 'TRUE means new issues can be created in this area';
COMMENT ON COLUMN "area"."external_reference"  IS 'Opaque data field to store an external reference';
COMMENT ON COLUMN "area"."direct_member_count" IS 'Number of active members of that area (ignoring their weight), as calculated from view "area_member_count"';
COMMENT ON COLUMN "area"."member_weight"       IS 'Same as "direct_member_count" but respecting delegations';


CREATE TABLE "ignored_area" (
        PRIMARY KEY ("member_id", "area_id"),
        "member_id"             INT4            REFERENCES "unit" ("id") ON DELETE CASCADE ON UPDATE CASCADE,
        "area_id"               INT4            REFERENCES "area" ("id") ON DELETE CASCADE ON UPDATE CASCADE );
CREATE INDEX "ignored_area_area_id_idx" ON "ignored_area" ("area_id");

COMMENT ON TABLE "ignored_area" IS 'An entry in this table denotes that the member does not wish to receive notifications for the given subject area unless he/she declared interested in a particular issue';


CREATE TABLE "area_setting" (
        PRIMARY KEY ("member_id", "key", "area_id"),
        "member_id"             INT4            REFERENCES "member" ("id") ON DELETE CASCADE ON UPDATE CASCADE,
        "key"                   TEXT            NOT NULL,
        "area_id"               INT4            REFERENCES "area" ("id") ON DELETE CASCADE ON UPDATE CASCADE,
        "value"                 TEXT            NOT NULL );

COMMENT ON TABLE "area_setting" IS 'Place for frontend to store area specific settings of members as strings';


CREATE TABLE "allowed_policy" (
        PRIMARY KEY ("area_id", "policy_id"),
        "area_id"               INT4            REFERENCES "area" ("id") ON DELETE CASCADE ON UPDATE CASCADE,
        "policy_id"             INT4            NOT NULL REFERENCES "policy" ("id") ON DELETE CASCADE ON UPDATE CASCADE,
        "default_policy"        BOOLEAN         NOT NULL DEFAULT FALSE );
CREATE UNIQUE INDEX "allowed_policy_one_default_per_area_idx" ON "allowed_policy" ("area_id") WHERE "default_policy";

COMMENT ON TABLE "allowed_policy" IS 'Selects which policies can be used in each area';

COMMENT ON COLUMN "allowed_policy"."default_policy" IS 'One policy per area can be set as default.';


CREATE TYPE "snapshot_event" AS ENUM ('periodic', 'end_of_admission', 'half_freeze', 'full_freeze');

COMMENT ON TYPE "snapshot_event" IS 'Reason for snapshots: ''periodic'' = due to periodic recalculation, ''end_of_admission'' = saved state at end of admission period, ''half_freeze'' = saved state at end of discussion period, ''full_freeze'' = saved state at end of verification period';


CREATE TYPE "issue_state" AS ENUM (
        'admission', 'discussion', 'verification', 'voting',
        'canceled_by_admin',
        'canceled_revoked_before_accepted',
        'canceled_issue_not_accepted',
        'canceled_after_revocation_during_discussion',
        'canceled_after_revocation_during_verification',
        'canceled_no_initiative_admitted',
        'finished_without_winner', 'finished_with_winner');

COMMENT ON TYPE "issue_state" IS 'State of issues';


CREATE TABLE "issue" (
        "id"                    SERIAL4         PRIMARY KEY,
        "area_id"               INT4            NOT NULL REFERENCES "area" ("id") ON DELETE CASCADE ON UPDATE CASCADE,
        "policy_id"             INT4            NOT NULL REFERENCES "policy" ("id") ON DELETE RESTRICT ON UPDATE CASCADE,
        "admin_notice"          TEXT,
        "external_reference"    TEXT,
        "state"                 "issue_state"   NOT NULL DEFAULT 'admission',
        "phase_finished"        TIMESTAMPTZ,
        "created"               TIMESTAMPTZ     NOT NULL DEFAULT now(),
        "accepted"              TIMESTAMPTZ,
        "half_frozen"           TIMESTAMPTZ,
        "fully_frozen"          TIMESTAMPTZ,
        "closed"                TIMESTAMPTZ,
        "cleaned"               TIMESTAMPTZ,
        "min_admission_time"    INTERVAL,
        "max_admission_time"    INTERVAL,
        "discussion_time"       INTERVAL        NOT NULL,
        "verification_time"     INTERVAL        NOT NULL,
        "voting_time"           INTERVAL        NOT NULL,
        "snapshot"              TIMESTAMPTZ,
        "latest_snapshot_event" "snapshot_event",
        "population"            INT4,
        "voter_count"           INT4,
        "status_quo_schulze_rank" INT4,
        CONSTRAINT "admission_time_not_null_unless_instantly_accepted" CHECK (
          ("min_admission_time" NOTNULL) = ("max_admission_time" NOTNULL) AND
          ("min_admission_time" NOTNULL OR ("accepted" NOTNULL AND "accepted" = "created")) ),
        CONSTRAINT "valid_state" CHECK (
          (
            ("accepted" ISNULL  AND "half_frozen" ISNULL  AND "fully_frozen" ISNULL ) OR
            ("accepted" NOTNULL AND "half_frozen" ISNULL  AND "fully_frozen" ISNULL ) OR
            ("accepted" NOTNULL AND "half_frozen" NOTNULL AND "fully_frozen" ISNULL ) OR
            ("accepted" NOTNULL AND "half_frozen" NOTNULL AND "fully_frozen" NOTNULL)
          ) AND (
            ("state" = 'admission'    AND "closed" ISNULL AND "accepted" ISNULL) OR
            ("state" = 'discussion'   AND "closed" ISNULL AND "accepted" NOTNULL AND "half_frozen" ISNULL) OR
            ("state" = 'verification' AND "closed" ISNULL AND "half_frozen" NOTNULL AND "fully_frozen" ISNULL) OR
            ("state" = 'voting'       AND "closed" ISNULL AND "fully_frozen" NOTNULL) OR
            ("state" = 'canceled_by_admin' AND "closed" NOTNULL) OR
            ("state" = 'canceled_revoked_before_accepted'              AND "closed" NOTNULL AND "accepted" ISNULL) OR
            ("state" = 'canceled_issue_not_accepted'                   AND "closed" NOTNULL AND "accepted" ISNULL) OR
            ("state" = 'canceled_after_revocation_during_discussion'   AND "closed" NOTNULL AND "half_frozen"  ISNULL) OR
            ("state" = 'canceled_after_revocation_during_verification' AND "closed" NOTNULL AND "fully_frozen" ISNULL) OR
            ("state" = 'canceled_no_initiative_admitted' AND "closed" NOTNULL AND "fully_frozen" NOTNULL AND "closed" = "fully_frozen") OR
            ("state" = 'finished_without_winner'         AND "closed" NOTNULL AND "fully_frozen" NOTNULL AND "closed" != "fully_frozen") OR
            ("state" = 'finished_with_winner'            AND "closed" NOTNULL AND "fully_frozen" NOTNULL AND "closed" != "fully_frozen")
          )),
        CONSTRAINT "phase_finished_only_when_not_closed" CHECK (
          "phase_finished" ISNULL OR "closed" ISNULL ),
        CONSTRAINT "state_change_order" CHECK (
          "created"      <= "accepted" AND
          "accepted"     <= "half_frozen" AND
          "half_frozen"  <= "fully_frozen" AND
          "fully_frozen" <= "closed" ),
        CONSTRAINT "only_closed_issues_may_be_cleaned" CHECK (
          "cleaned" ISNULL OR "closed" NOTNULL ),
        CONSTRAINT "last_snapshot_on_full_freeze"
          CHECK ("snapshot" = "fully_frozen"),  -- NOTE: snapshot can be set, while frozen is NULL yet
        CONSTRAINT "freeze_requires_snapshot"
          CHECK ("fully_frozen" ISNULL OR "snapshot" NOTNULL),
        CONSTRAINT "set_both_or_none_of_snapshot_and_latest_snapshot_event"
          CHECK (("snapshot" NOTNULL) = ("latest_snapshot_event" NOTNULL)) );
CREATE INDEX "issue_area_id_idx" ON "issue" ("area_id");
CREATE INDEX "issue_policy_id_idx" ON "issue" ("policy_id");
CREATE INDEX "issue_created_idx" ON "issue" ("created");
CREATE INDEX "issue_accepted_idx" ON "issue" ("accepted");
CREATE INDEX "issue_half_frozen_idx" ON "issue" ("half_frozen");
CREATE INDEX "issue_fully_frozen_idx" ON "issue" ("fully_frozen");
CREATE INDEX "issue_closed_idx" ON "issue" ("closed");
CREATE INDEX "issue_created_idx_open" ON "issue" ("created") WHERE "closed" ISNULL;
CREATE INDEX "issue_closed_idx_canceled" ON "issue" ("closed") WHERE "fully_frozen" ISNULL;

COMMENT ON TABLE "issue" IS 'Groups of initiatives';

COMMENT ON COLUMN "issue"."admin_notice"            IS 'Public notice by admin to explain manual interventions, or to announce corrections';
COMMENT ON COLUMN "issue"."external_reference"      IS 'Opaque data field to store an external reference';
COMMENT ON COLUMN "issue"."phase_finished"          IS 'Set to a value NOTNULL, if the current phase has finished, but calculations are pending; No changes in this issue shall be made by the frontend or API when this value is set';
COMMENT ON COLUMN "issue"."accepted"                IS 'Point in time, when one initiative of issue reached the "issue_quorum"';
COMMENT ON COLUMN "issue"."half_frozen"             IS 'Point in time, when "discussion_time" has elapsed; Frontends must ensure that for half_frozen issues a) initiatives are not revoked, b) no new drafts are created, c) no initiators are added or removed.';
COMMENT ON COLUMN "issue"."fully_frozen"            IS 'Point in time, when "verification_time" has elapsed and voting has started; Frontends must ensure that for fully_frozen issues additionally to the restrictions for half_frozen issues a) initiatives are not created, b) no interest is created or removed, c) no supporters are added or removed, d) no opinions are created, changed or deleted.';
COMMENT ON COLUMN "issue"."closed"                  IS 'Point in time, when "max_admission_time" or "voting_time" have elapsed, and issue is no longer active; Frontends must ensure that for closed issues additionally to the restrictions for half_frozen and fully_frozen issues a) no voter is added or removed to/from the direct_voter table, b) no votes are added, modified or removed.';
COMMENT ON COLUMN "issue"."cleaned"                 IS 'Point in time, when discussion data and votes had been deleted';
COMMENT ON COLUMN "issue"."min_admission_time"      IS 'Copied from "policy" table at creation of issue';
COMMENT ON COLUMN "issue"."max_admission_time"      IS 'Copied from "policy" table at creation of issue';
COMMENT ON COLUMN "issue"."discussion_time"         IS 'Copied from "policy" table at creation of issue';
COMMENT ON COLUMN "issue"."verification_time"       IS 'Copied from "policy" table at creation of issue';
COMMENT ON COLUMN "issue"."voting_time"             IS 'Copied from "policy" table at creation of issue';
COMMENT ON COLUMN "issue"."snapshot"                IS 'Point in time, when snapshot tables have been updated and "population" and *_count values were precalculated';
COMMENT ON COLUMN "issue"."latest_snapshot_event"   IS 'Event type of latest snapshot for issue; Can be used to select the latest snapshot data in the snapshot tables';
COMMENT ON COLUMN "issue"."population"              IS 'Sum of "weight" column in table "direct_population_snapshot"';
COMMENT ON COLUMN "issue"."voter_count"             IS 'Total number of direct and delegating voters; This value is related to the final voting, while "population" is related to snapshots before the final voting';
COMMENT ON COLUMN "issue"."status_quo_schulze_rank" IS 'Schulze rank of status quo, as calculated by "calculate_ranks" function';


CREATE TABLE "issue_order_in_admission_state" (
        "id"                    INT8            PRIMARY KEY, --REFERENCES "issue" ("id") ON DELETE CASCADE ON UPDATE CASCADE,
        "order_in_area"         INT4,
        "order_in_unit"         INT4 );

COMMENT ON TABLE "issue_order_in_admission_state" IS 'Ordering information for issues that are not stored in the "issue" table to avoid locking of multiple issues at once; Filled/updated by "lf_update_issue_order"';

COMMENT ON COLUMN "issue_order_in_admission_state"."id"            IS 'References "issue" ("id") but has no referential integrity trigger associated, due to performance/locking issues';
COMMENT ON COLUMN "issue_order_in_admission_state"."order_in_area" IS 'Order of issues in admission state within a single area; NULL values sort last';
COMMENT ON COLUMN "issue_order_in_admission_state"."order_in_unit" IS 'Order of issues in admission state within all areas of a unit; NULL values sort last';


CREATE TABLE "issue_setting" (
        PRIMARY KEY ("member_id", "key", "issue_id"),
        "member_id"             INT4            REFERENCES "member" ("id") ON DELETE CASCADE ON UPDATE CASCADE,
        "key"                   TEXT            NOT NULL,
        "issue_id"              INT4            REFERENCES "issue" ("id") ON DELETE CASCADE ON UPDATE CASCADE,
        "value"                 TEXT            NOT NULL );

COMMENT ON TABLE "issue_setting" IS 'Place for frontend to store issue specific settings of members as strings';


CREATE TABLE "initiative" (
        UNIQUE ("issue_id", "id"),  -- index needed for foreign-key on table "vote"
        "issue_id"              INT4            NOT NULL REFERENCES "issue" ("id") ON DELETE CASCADE ON UPDATE CASCADE,
        "id"                    SERIAL4         PRIMARY KEY,
        "name"                  TEXT            NOT NULL,
        "polling"               BOOLEAN         NOT NULL DEFAULT FALSE,
        "created"               TIMESTAMPTZ     NOT NULL DEFAULT now(),
        "revoked"               TIMESTAMPTZ,
        "revoked_by_member_id"  INT4            REFERENCES "member" ("id") ON DELETE RESTRICT ON UPDATE CASCADE,
        "suggested_initiative_id" INT4          REFERENCES "initiative" ("id") ON DELETE CASCADE ON UPDATE CASCADE,
        "external_reference"    TEXT,
        "admitted"              BOOLEAN,
        "supporter_count"                    INT4,
        "informed_supporter_count"           INT4,
        "satisfied_supporter_count"          INT4,
        "satisfied_informed_supporter_count" INT4,
        "harmonic_weight"       NUMERIC(12, 3),
        "final_suggestion_order_calculated" BOOLEAN NOT NULL DEFAULT FALSE,
        "first_preference_votes" INT4,
        "positive_votes"        INT4,
        "negative_votes"        INT4,
        "direct_majority"       BOOLEAN,
        "indirect_majority"     BOOLEAN,
        "schulze_rank"          INT4,
        "better_than_status_quo" BOOLEAN,
        "worse_than_status_quo" BOOLEAN,
        "reverse_beat_path"     BOOLEAN,
        "multistage_majority"   BOOLEAN,
        "eligible"              BOOLEAN,
        "winner"                BOOLEAN,
        "rank"                  INT4,
        "text_search_data"      TSVECTOR,
        CONSTRAINT "all_or_none_of_revoked_and_revoked_by_member_id_must_be_null"
          CHECK (("revoked" NOTNULL) = ("revoked_by_member_id" NOTNULL)),
        CONSTRAINT "non_revoked_initiatives_cant_suggest_other"
          CHECK ("revoked" NOTNULL OR "suggested_initiative_id" ISNULL),
        CONSTRAINT "revoked_initiatives_cant_be_admitted"
          CHECK ("revoked" ISNULL OR "admitted" ISNULL),
        CONSTRAINT "non_admitted_initiatives_cant_contain_voting_results" CHECK (
          ( "admitted" NOTNULL AND "admitted" = TRUE ) OR
          ( "first_preference_votes" ISNULL AND
            "positive_votes" ISNULL AND "negative_votes" ISNULL AND
            "direct_majority" ISNULL AND "indirect_majority" ISNULL AND
            "schulze_rank" ISNULL AND
            "better_than_status_quo" ISNULL AND "worse_than_status_quo" ISNULL AND
            "reverse_beat_path" ISNULL AND "multistage_majority" ISNULL AND
            "eligible" ISNULL AND "winner" ISNULL AND "rank" ISNULL ) ),
        CONSTRAINT "better_excludes_worse" CHECK (NOT ("better_than_status_quo" AND "worse_than_status_quo")),
        CONSTRAINT "minimum_requirement_to_be_eligible" CHECK (
          "eligible" = FALSE OR
          ("direct_majority" AND "indirect_majority" AND "better_than_status_quo") ),
        CONSTRAINT "winner_must_be_eligible" CHECK ("winner"=FALSE OR "eligible"=TRUE),
        CONSTRAINT "winner_must_have_first_rank" CHECK ("winner"=FALSE OR "rank"=1),
        CONSTRAINT "eligible_at_first_rank_is_winner" CHECK ("eligible"=FALSE OR "rank"!=1 OR "winner"=TRUE),
        CONSTRAINT "unique_rank_per_issue" UNIQUE ("issue_id", "rank") );
CREATE INDEX "initiative_created_idx" ON "initiative" ("created");
CREATE INDEX "initiative_revoked_idx" ON "initiative" ("revoked");
CREATE INDEX "initiative_text_search_data_idx" ON "initiative" USING gin ("text_search_data");
CREATE TRIGGER "update_text_search_data"
  BEFORE INSERT OR UPDATE ON "initiative"
  FOR EACH ROW EXECUTE PROCEDURE
  tsvector_update_trigger('text_search_data', 'pg_catalog.simple', "name");

COMMENT ON TABLE "initiative" IS 'Group of members publishing drafts for resolutions to be passed; Frontends must ensure that initiatives of half_frozen issues are not revoked, and that initiatives of fully_frozen or closed issues are neither revoked nor created.';

COMMENT ON COLUMN "initiative"."polling"                IS 'Initiative does not need to pass the initiative quorum (see "policy"."polling")';
COMMENT ON COLUMN "initiative"."revoked"                IS 'Point in time, when one initiator decided to revoke the initiative';
COMMENT ON COLUMN "initiative"."revoked_by_member_id"   IS 'Member, who decided to revoke the initiative';
COMMENT ON COLUMN "initiative"."external_reference"     IS 'Opaque data field to store an external reference';
COMMENT ON COLUMN "initiative"."admitted"               IS 'TRUE, if initiative reaches the "initiative_quorum" when freezing the issue';
COMMENT ON COLUMN "initiative"."supporter_count"                    IS 'Calculated from table "direct_supporter_snapshot"';
COMMENT ON COLUMN "initiative"."informed_supporter_count"           IS 'Calculated from table "direct_supporter_snapshot"';
COMMENT ON COLUMN "initiative"."satisfied_supporter_count"          IS 'Calculated from table "direct_supporter_snapshot"';
COMMENT ON COLUMN "initiative"."satisfied_informed_supporter_count" IS 'Calculated from table "direct_supporter_snapshot"';
COMMENT ON COLUMN "initiative"."harmonic_weight"        IS 'Indicates the relevancy of the initiative, calculated from the potential supporters weighted with the harmonic series to avoid a large number of clones affecting other initiative''s sorting positions too much; shall be used as secondary sorting key after "admitted" as primary sorting key';
COMMENT ON COLUMN "initiative"."final_suggestion_order_calculated" IS 'Set to TRUE, when "proportional_order" of suggestions has been calculated the last time';
COMMENT ON COLUMN "initiative"."first_preference_votes" IS 'Number of direct and delegating voters who ranked this initiative as their first choice';
COMMENT ON COLUMN "initiative"."positive_votes"         IS 'Number of direct and delegating voters who ranked this initiative better than the status quo';
COMMENT ON COLUMN "initiative"."negative_votes"         IS 'Number of direct and delegating voters who ranked this initiative worse than the status quo';
COMMENT ON COLUMN "initiative"."direct_majority"        IS 'TRUE, if "positive_votes"/("positive_votes"+"negative_votes") is strictly greater or greater-equal than "direct_majority_num"/"direct_majority_den", and "positive_votes" is greater-equal than "direct_majority_positive", and ("positive_votes"+abstentions) is greater-equal than "direct_majority_non_negative"';
COMMENT ON COLUMN "initiative"."indirect_majority"      IS 'Same as "direct_majority", but also considering indirect beat paths';
COMMENT ON COLUMN "initiative"."schulze_rank"           IS 'Schulze-Ranking';
COMMENT ON COLUMN "initiative"."better_than_status_quo" IS 'TRUE, if initiative has a schulze-ranking better than the status quo';
COMMENT ON COLUMN "initiative"."worse_than_status_quo"  IS 'TRUE, if initiative has a schulze-ranking worse than the status quo (DEPRECATED, since schulze-ranking is unique per issue; use "better_than_status_quo"=FALSE)';
COMMENT ON COLUMN "initiative"."reverse_beat_path"      IS 'TRUE, if there is a beat path (may include ties) from this initiative to the status quo; set to NULL if "policy"."defeat_strength" is set to ''simple''';
COMMENT ON COLUMN "initiative"."multistage_majority"    IS 'TRUE, if either (a) this initiative has no better rank than the status quo, or (b) there exists a better ranked initiative X, which directly beats this initiative, and either more voters prefer X to this initiative than voters preferring X to the status quo or less voters prefer this initiative to X than voters preferring the status quo to X';
COMMENT ON COLUMN "initiative"."eligible"               IS 'Initiative has a "direct_majority" and an "indirect_majority", is "better_than_status_quo" and depending on selected policy the initiative has no "reverse_beat_path" or "multistage_majority"';
COMMENT ON COLUMN "initiative"."winner"                 IS 'Winner is the "eligible" initiative with best "schulze_rank"';
COMMENT ON COLUMN "initiative"."rank"                   IS 'Unique ranking for all "admitted" initiatives per issue; lower rank is better; a winner always has rank 1, but rank 1 does not imply that an initiative is winner; initiatives with "direct_majority" AND "indirect_majority" always have a better (lower) rank than other initiatives';


CREATE TABLE "battle" (
        "issue_id"              INT4            NOT NULL,
        "winning_initiative_id" INT4,
        FOREIGN KEY ("issue_id", "winning_initiative_id") REFERENCES "initiative" ("issue_id", "id") ON DELETE CASCADE ON UPDATE CASCADE,
        "losing_initiative_id"  INT4,
        FOREIGN KEY ("issue_id", "losing_initiative_id") REFERENCES "initiative" ("issue_id", "id") ON DELETE CASCADE ON UPDATE CASCADE,
        "count"                 INT4            NOT NULL,
        CONSTRAINT "initiative_ids_not_equal" CHECK (
          "winning_initiative_id" != "losing_initiative_id" OR
          ( ("winning_initiative_id" NOTNULL AND "losing_initiative_id" ISNULL) OR
            ("winning_initiative_id" ISNULL AND "losing_initiative_id" NOTNULL) ) ) );
CREATE UNIQUE INDEX "battle_winning_losing_idx" ON "battle" ("issue_id", "winning_initiative_id", "losing_initiative_id");
CREATE UNIQUE INDEX "battle_winning_null_idx" ON "battle" ("issue_id", "winning_initiative_id") WHERE "losing_initiative_id" ISNULL;
CREATE UNIQUE INDEX "battle_null_losing_idx" ON "battle" ("issue_id", "losing_initiative_id") WHERE "winning_initiative_id" ISNULL;

COMMENT ON TABLE "battle" IS 'Number of members preferring one initiative to another; Filled by "battle_view" when closing an issue; NULL as initiative_id denotes virtual "status-quo" initiative';


CREATE TABLE "ignored_initiative" (
        PRIMARY KEY ("member_id", "initiative_id"),
        "member_id"             INT4            REFERENCES "member" ("id") ON DELETE CASCADE ON UPDATE CASCADE,
        "initiative_id"         INT4            REFERENCES "initiative" ("id") ON DELETE CASCADE ON UPDATE CASCADE );
CREATE INDEX "ignored_initiative_initiative_id_idx" ON "ignored_initiative" ("initiative_id");

COMMENT ON TABLE "ignored_initiative" IS 'An entry in this table denotes that the member does not wish to receive notifications for the given initiative';


CREATE TABLE "initiative_setting" (
        PRIMARY KEY ("member_id", "key", "initiative_id"),
        "member_id"             INT4            REFERENCES "member" ("id") ON DELETE CASCADE ON UPDATE CASCADE,
        "key"                   TEXT            NOT NULL,
        "initiative_id"         INT4            REFERENCES "initiative" ("id") ON DELETE CASCADE ON UPDATE CASCADE,
        "value"                 TEXT            NOT NULL );

COMMENT ON TABLE "initiative_setting" IS 'Place for frontend to store initiative specific settings of members as strings';


CREATE TABLE "draft" (
        UNIQUE ("initiative_id", "id"),  -- index needed for foreign-key on table "supporter"
        "initiative_id"         INT4            NOT NULL REFERENCES "initiative" ("id") ON DELETE CASCADE ON UPDATE CASCADE,
        "id"                    SERIAL8         PRIMARY KEY,
        "created"               TIMESTAMPTZ     NOT NULL DEFAULT now(),
        "author_id"             INT4            NOT NULL REFERENCES "member" ("id") ON DELETE RESTRICT ON UPDATE CASCADE,
        "formatting_engine"     TEXT,
        "content"               TEXT            NOT NULL,
        "external_reference"    TEXT,
        "text_search_data"      TSVECTOR );
CREATE INDEX "draft_created_idx" ON "draft" ("created");
CREATE INDEX "draft_author_id_created_idx" ON "draft" ("author_id", "created");
CREATE INDEX "draft_text_search_data_idx" ON "draft" USING gin ("text_search_data");
CREATE TRIGGER "update_text_search_data"
  BEFORE INSERT OR UPDATE ON "draft"
  FOR EACH ROW EXECUTE PROCEDURE
  tsvector_update_trigger('text_search_data', 'pg_catalog.simple', "content");

COMMENT ON TABLE "draft" IS 'Drafts of initiatives to solve issues; Frontends must ensure that new drafts for initiatives of half_frozen, fully_frozen or closed issues can''t be created.';

COMMENT ON COLUMN "draft"."formatting_engine"  IS 'Allows different formatting engines (i.e. wiki formats) to be used';
COMMENT ON COLUMN "draft"."content"            IS 'Text of the draft in a format depending on the field "formatting_engine"';
COMMENT ON COLUMN "draft"."external_reference" IS 'Opaque data field to store an external reference';


CREATE TABLE "rendered_draft" (
        PRIMARY KEY ("draft_id", "format"),
        "draft_id"              INT8            REFERENCES "draft" ("id") ON DELETE CASCADE ON UPDATE CASCADE,
        "format"                TEXT,
        "content"               TEXT            NOT NULL );

COMMENT ON TABLE "rendered_draft" IS 'This table may be used by frontends to cache "rendered" drafts (e.g. HTML output generated from wiki text)';


CREATE TABLE "suggestion" (
        UNIQUE ("initiative_id", "id"),  -- index needed for foreign-key on table "opinion"
        "initiative_id"         INT4            NOT NULL REFERENCES "initiative" ("id") ON DELETE CASCADE ON UPDATE CASCADE,
        "id"                    SERIAL8         PRIMARY KEY,
        "draft_id"              INT8            NOT NULL,
        FOREIGN KEY ("initiative_id", "draft_id") REFERENCES "draft" ("initiative_id", "id") ON DELETE NO ACTION ON UPDATE CASCADE,
        "created"               TIMESTAMPTZ     NOT NULL DEFAULT now(),
        "author_id"             INT4            NOT NULL REFERENCES "member" ("id") ON DELETE RESTRICT ON UPDATE CASCADE,
        "name"                  TEXT            NOT NULL,
        "formatting_engine"     TEXT,
        "content"               TEXT            NOT NULL DEFAULT '',
        "external_reference"    TEXT,
        "text_search_data"      TSVECTOR,
        "minus2_unfulfilled_count" INT4,
        "minus2_fulfilled_count"   INT4,
        "minus1_unfulfilled_count" INT4,
        "minus1_fulfilled_count"   INT4,
        "plus1_unfulfilled_count"  INT4,
        "plus1_fulfilled_count"    INT4,
        "plus2_unfulfilled_count"  INT4,
        "plus2_fulfilled_count"    INT4,
        "proportional_order"    INT4 );
CREATE INDEX "suggestion_created_idx" ON "suggestion" ("created");
CREATE INDEX "suggestion_author_id_created_idx" ON "suggestion" ("author_id", "created");
CREATE INDEX "suggestion_text_search_data_idx" ON "suggestion" USING gin ("text_search_data");
CREATE TRIGGER "update_text_search_data"
  BEFORE INSERT OR UPDATE ON "suggestion"
  FOR EACH ROW EXECUTE PROCEDURE
  tsvector_update_trigger('text_search_data', 'pg_catalog.simple',
    "name", "content");

COMMENT ON TABLE "suggestion" IS 'Suggestions to initiators, to change the current draft; must not be deleted explicitly, as they vanish automatically if the last opinion is deleted';

COMMENT ON COLUMN "suggestion"."draft_id"                 IS 'Draft, which the author has seen when composing the suggestion; should always be set by a frontend, but defaults to current draft of the initiative (implemented by trigger "default_for_draft_id")';
COMMENT ON COLUMN "suggestion"."external_reference"       IS 'Opaque data field to store an external reference';
COMMENT ON COLUMN "suggestion"."minus2_unfulfilled_count" IS 'Calculated from table "direct_supporter_snapshot", not requiring informed supporters';
COMMENT ON COLUMN "suggestion"."minus2_fulfilled_count"   IS 'Calculated from table "direct_supporter_snapshot", not requiring informed supporters';
COMMENT ON COLUMN "suggestion"."minus1_unfulfilled_count" IS 'Calculated from table "direct_supporter_snapshot", not requiring informed supporters';
COMMENT ON COLUMN "suggestion"."minus1_fulfilled_count"   IS 'Calculated from table "direct_supporter_snapshot", not requiring informed supporters';
COMMENT ON COLUMN "suggestion"."plus1_unfulfilled_count"  IS 'Calculated from table "direct_supporter_snapshot", not requiring informed supporters';
COMMENT ON COLUMN "suggestion"."plus1_fulfilled_count"    IS 'Calculated from table "direct_supporter_snapshot", not requiring informed supporters';
COMMENT ON COLUMN "suggestion"."plus2_unfulfilled_count"  IS 'Calculated from table "direct_supporter_snapshot", not requiring informed supporters';
COMMENT ON COLUMN "suggestion"."plus2_fulfilled_count"    IS 'Calculated from table "direct_supporter_snapshot", not requiring informed supporters';
COMMENT ON COLUMN "suggestion"."proportional_order"       IS 'To be used for sorting suggestions within an initiative; NULL values sort last; updated by "lf_update_suggestion_order"';


CREATE TABLE "rendered_suggestion" (
        PRIMARY KEY ("suggestion_id", "format"),
        "suggestion_id"         INT8            REFERENCES "suggestion" ("id") ON DELETE CASCADE ON UPDATE CASCADE,
        "format"                TEXT,
        "content"               TEXT            NOT NULL );

COMMENT ON TABLE "rendered_suggestion" IS 'This table may be used by frontends to cache "rendered" drafts (e.g. HTML output generated from wiki text)';


CREATE TABLE "suggestion_setting" (
        PRIMARY KEY ("member_id", "key", "suggestion_id"),
        "member_id"             INT4            REFERENCES "member" ("id") ON DELETE CASCADE ON UPDATE CASCADE,
        "key"                   TEXT            NOT NULL,
        "suggestion_id"         INT8            REFERENCES "suggestion" ("id") ON DELETE CASCADE ON UPDATE CASCADE,
        "value"                 TEXT            NOT NULL );

COMMENT ON TABLE "suggestion_setting" IS 'Place for frontend to store suggestion specific settings of members as strings';


CREATE TABLE "privilege" (
        PRIMARY KEY ("unit_id", "member_id"),
        "unit_id"               INT4            REFERENCES "unit" ("id") ON DELETE CASCADE ON UPDATE CASCADE,
        "member_id"             INT4            REFERENCES "member" ("id") ON DELETE CASCADE ON UPDATE CASCADE,
        "admin_manager"         BOOLEAN         NOT NULL DEFAULT FALSE,
        "unit_manager"          BOOLEAN         NOT NULL DEFAULT FALSE,
        "area_manager"          BOOLEAN         NOT NULL DEFAULT FALSE,
        "member_manager"        BOOLEAN         NOT NULL DEFAULT FALSE,
        "initiative_right"      BOOLEAN         NOT NULL DEFAULT TRUE,
        "voting_right"          BOOLEAN         NOT NULL DEFAULT TRUE,
        "polling_right"         BOOLEAN         NOT NULL DEFAULT FALSE );

COMMENT ON TABLE "privilege" IS 'Members rights related to each unit';

COMMENT ON COLUMN "privilege"."admin_manager"    IS 'Grant/revoke any privileges to/from other members';
COMMENT ON COLUMN "privilege"."unit_manager"     IS 'Create and disable sub units';
COMMENT ON COLUMN "privilege"."area_manager"     IS 'Create and disable areas and set area parameters';
COMMENT ON COLUMN "privilege"."member_manager"   IS 'Adding/removing members from the unit, granting or revoking "initiative_right" and "voting_right"';
COMMENT ON COLUMN "privilege"."initiative_right" IS 'Right to create an initiative';
COMMENT ON COLUMN "privilege"."voting_right"     IS 'Right to support initiatives, create and rate suggestions, and to vote';
COMMENT ON COLUMN "privilege"."polling_right"    IS 'Right to create issues with policies having the "policy"."polling" flag set, and to add initiatives having the "initiative"."polling" flag set to those issues';


CREATE TABLE "membership" (
        PRIMARY KEY ("area_id", "member_id"),
        "area_id"               INT4            REFERENCES "area" ("id") ON DELETE CASCADE ON UPDATE CASCADE,
        "member_id"             INT4            REFERENCES "member" ("id") ON DELETE CASCADE ON UPDATE CASCADE );
CREATE INDEX "membership_member_id_idx" ON "membership" ("member_id");

COMMENT ON TABLE "membership" IS 'Interest of members in topic areas';


CREATE TABLE "interest" (
        PRIMARY KEY ("issue_id", "member_id"),
        "issue_id"              INT4            REFERENCES "issue" ("id") ON DELETE CASCADE ON UPDATE CASCADE,
        "member_id"             INT4            REFERENCES "member" ("id") ON DELETE CASCADE ON UPDATE CASCADE );
CREATE INDEX "interest_member_id_idx" ON "interest" ("member_id");

COMMENT ON TABLE "interest" IS 'Interest of members in a particular issue; Frontends must ensure that interest for fully_frozen or closed issues is not added or removed.';


CREATE TABLE "initiator" (
        PRIMARY KEY ("initiative_id", "member_id"),
        "initiative_id"         INT4            REFERENCES "initiative" ("id") ON DELETE CASCADE ON UPDATE CASCADE,
        "member_id"             INT4            REFERENCES "member" ("id") ON DELETE CASCADE ON UPDATE CASCADE,
        "accepted"              BOOLEAN );
CREATE INDEX "initiator_member_id_idx" ON "initiator" ("member_id");

COMMENT ON TABLE "initiator" IS 'Members who are allowed to post new drafts; Frontends must ensure that initiators are not added or removed from half_frozen, fully_frozen or closed initiatives.';

COMMENT ON COLUMN "initiator"."accepted" IS 'If "accepted" is NULL, then the member was invited to be a co-initiator, but has not answered yet. If it is TRUE, the member has accepted the invitation, if it is FALSE, the member has rejected the invitation.';


CREATE TABLE "supporter" (
        "issue_id"              INT4            NOT NULL,
        PRIMARY KEY ("initiative_id", "member_id"),
        "initiative_id"         INT4,
        "member_id"             INT4,
        "draft_id"              INT8            NOT NULL,
        FOREIGN KEY ("issue_id", "member_id") REFERENCES "interest" ("issue_id", "member_id") ON DELETE CASCADE ON UPDATE CASCADE,
        FOREIGN KEY ("initiative_id", "draft_id") REFERENCES "draft" ("initiative_id", "id") ON DELETE NO ACTION ON UPDATE CASCADE );
CREATE INDEX "supporter_member_id_idx" ON "supporter" ("member_id");

COMMENT ON TABLE "supporter" IS 'Members who support an initiative (conditionally); Frontends must ensure that supporters are not added or removed from fully_frozen or closed initiatives.';

COMMENT ON COLUMN "supporter"."issue_id" IS 'WARNING: No index: For selections use column "initiative_id" and join via table "initiative" where neccessary';
COMMENT ON COLUMN "supporter"."draft_id" IS 'Latest seen draft; should always be set by a frontend, but defaults to current draft of the initiative (implemented by trigger "default_for_draft_id")';


CREATE TABLE "opinion" (
        "initiative_id"         INT4            NOT NULL,
        PRIMARY KEY ("suggestion_id", "member_id"),
        "suggestion_id"         INT8,
        "member_id"             INT4,
        "degree"                INT2            NOT NULL CHECK ("degree" >= -2 AND "degree" <= 2 AND "degree" != 0),
        "fulfilled"             BOOLEAN         NOT NULL DEFAULT FALSE,
        FOREIGN KEY ("initiative_id", "suggestion_id") REFERENCES "suggestion" ("initiative_id", "id") ON DELETE CASCADE ON UPDATE CASCADE,
        FOREIGN KEY ("initiative_id", "member_id") REFERENCES "supporter" ("initiative_id", "member_id") ON DELETE CASCADE ON UPDATE CASCADE );
CREATE INDEX "opinion_member_id_initiative_id_idx" ON "opinion" ("member_id", "initiative_id");

COMMENT ON TABLE "opinion" IS 'Opinion on suggestions (criticism related to initiatives); Frontends must ensure that opinions are not created modified or deleted when related to fully_frozen or closed issues.';

COMMENT ON COLUMN "opinion"."degree" IS '2 = fulfillment required for support; 1 = fulfillment desired; -1 = fulfillment unwanted; -2 = fulfillment cancels support';


CREATE TYPE "delegation_scope" AS ENUM ('unit', 'area', 'issue');

COMMENT ON TYPE "delegation_scope" IS 'Scope for delegations: ''unit'', ''area'', or ''issue'' (order is relevant)';


CREATE TABLE "delegation" (
        "id"                    SERIAL8         PRIMARY KEY,
        "truster_id"            INT4            NOT NULL REFERENCES "member" ("id") ON DELETE CASCADE ON UPDATE CASCADE,
        "trustee_id"            INT4            REFERENCES "member" ("id") ON DELETE CASCADE ON UPDATE CASCADE,
        "scope"              "delegation_scope" NOT NULL,
        "unit_id"               INT4            REFERENCES "unit" ("id") ON DELETE CASCADE ON UPDATE CASCADE,
        "area_id"               INT4            REFERENCES "area" ("id") ON DELETE CASCADE ON UPDATE CASCADE,
        "issue_id"              INT4            REFERENCES "issue" ("id") ON DELETE CASCADE ON UPDATE CASCADE,
        CONSTRAINT "cant_delegate_to_yourself" CHECK ("truster_id" != "trustee_id"),
        CONSTRAINT "no_unit_delegation_to_null"
          CHECK ("trustee_id" NOTNULL OR "scope" != 'unit'),
        CONSTRAINT "area_id_and_issue_id_set_according_to_scope" CHECK (
          ("scope" = 'unit'  AND "unit_id" NOTNULL AND "area_id" ISNULL  AND "issue_id" ISNULL ) OR
          ("scope" = 'area'  AND "unit_id" ISNULL  AND "area_id" NOTNULL AND "issue_id" ISNULL ) OR
          ("scope" = 'issue' AND "unit_id" ISNULL  AND "area_id" ISNULL  AND "issue_id" NOTNULL) ),
        UNIQUE ("unit_id", "truster_id"),
        UNIQUE ("area_id", "truster_id"),
        UNIQUE ("issue_id", "truster_id") );
CREATE INDEX "delegation_truster_id_idx" ON "delegation" ("truster_id");
CREATE INDEX "delegation_trustee_id_idx" ON "delegation" ("trustee_id");

COMMENT ON TABLE "delegation" IS 'Delegation of vote-weight to other members';

COMMENT ON COLUMN "delegation"."unit_id"  IS 'Reference to unit, if delegation is unit-wide, otherwise NULL';
COMMENT ON COLUMN "delegation"."area_id"  IS 'Reference to area, if delegation is area-wide, otherwise NULL';
COMMENT ON COLUMN "delegation"."issue_id" IS 'Reference to issue, if delegation is issue-wide, otherwise NULL';


CREATE TABLE "direct_population_snapshot" (
        PRIMARY KEY ("issue_id", "event", "member_id"),
        "issue_id"              INT4            REFERENCES "issue" ("id") ON DELETE CASCADE ON UPDATE CASCADE,
        "event"                 "snapshot_event",
        "member_id"             INT4            REFERENCES "member" ("id") ON DELETE RESTRICT ON UPDATE RESTRICT,
        "weight"                INT4 );
CREATE INDEX "direct_population_snapshot_member_id_idx" ON "direct_population_snapshot" ("member_id");

COMMENT ON TABLE "direct_population_snapshot" IS 'Snapshot of active members having either a "membership" in the "area" or an "interest" in the "issue"; for corrections refer to column "issue_notice" of "issue" table';

COMMENT ON COLUMN "direct_population_snapshot"."event"  IS 'Reason for snapshot, see "snapshot_event" type for details';
COMMENT ON COLUMN "direct_population_snapshot"."weight" IS 'Weight of member (1 or higher) according to "delegating_population_snapshot"';


CREATE TABLE "delegating_population_snapshot" (
        PRIMARY KEY ("issue_id", "event", "member_id"),
        "issue_id"              INT4            REFERENCES "issue" ("id") ON DELETE CASCADE ON UPDATE CASCADE,
        "event"                "snapshot_event",
        "member_id"             INT4            REFERENCES "member" ("id") ON DELETE RESTRICT ON UPDATE RESTRICT,
        "weight"                INT4,
        "scope"              "delegation_scope" NOT NULL,
        "delegate_member_ids"   INT4[]          NOT NULL );
CREATE INDEX "delegating_population_snapshot_member_id_idx" ON "delegating_population_snapshot" ("member_id");

COMMENT ON TABLE "direct_population_snapshot" IS 'Delegations increasing the weight of entries in the "direct_population_snapshot" table; for corrections refer to column "issue_notice" of "issue" table';

COMMENT ON COLUMN "delegating_population_snapshot"."event"               IS 'Reason for snapshot, see "snapshot_event" type for details';
COMMENT ON COLUMN "delegating_population_snapshot"."member_id"           IS 'Delegating member';
COMMENT ON COLUMN "delegating_population_snapshot"."weight"              IS 'Intermediate weight';
COMMENT ON COLUMN "delegating_population_snapshot"."delegate_member_ids" IS 'Chain of members who act as delegates; last entry referes to "member_id" column of table "direct_population_snapshot"';


CREATE TABLE "direct_interest_snapshot" (
        PRIMARY KEY ("issue_id", "event", "member_id"),
        "issue_id"              INT4            REFERENCES "issue" ("id") ON DELETE CASCADE ON UPDATE CASCADE,
        "event"                 "snapshot_event",
        "member_id"             INT4            REFERENCES "member" ("id") ON DELETE RESTRICT ON UPDATE RESTRICT,
        "weight"                INT4 );
CREATE INDEX "direct_interest_snapshot_member_id_idx" ON "direct_interest_snapshot" ("member_id");

COMMENT ON TABLE "direct_interest_snapshot" IS 'Snapshot of active members having an "interest" in the "issue"; for corrections refer to column "issue_notice" of "issue" table';

COMMENT ON COLUMN "direct_interest_snapshot"."event"            IS 'Reason for snapshot, see "snapshot_event" type for details';
COMMENT ON COLUMN "direct_interest_snapshot"."weight"           IS 'Weight of member (1 or higher) according to "delegating_interest_snapshot"';


CREATE TABLE "delegating_interest_snapshot" (
        PRIMARY KEY ("issue_id", "event", "member_id"),
        "issue_id"         INT4                 REFERENCES "issue" ("id") ON DELETE CASCADE ON UPDATE CASCADE,
        "event"                "snapshot_event",
        "member_id"             INT4            REFERENCES "member" ("id") ON DELETE RESTRICT ON UPDATE RESTRICT,
        "weight"                INT4,
        "scope"              "delegation_scope" NOT NULL,
        "delegate_member_ids"   INT4[]          NOT NULL );
CREATE INDEX "delegating_interest_snapshot_member_id_idx" ON "delegating_interest_snapshot" ("member_id");

COMMENT ON TABLE "delegating_interest_snapshot" IS 'Delegations increasing the weight of entries in the "direct_interest_snapshot" table; for corrections refer to column "issue_notice" of "issue" table';

COMMENT ON COLUMN "delegating_interest_snapshot"."event"               IS 'Reason for snapshot, see "snapshot_event" type for details';
COMMENT ON COLUMN "delegating_interest_snapshot"."member_id"           IS 'Delegating member';
COMMENT ON COLUMN "delegating_interest_snapshot"."weight"              IS 'Intermediate weight';
COMMENT ON COLUMN "delegating_interest_snapshot"."delegate_member_ids" IS 'Chain of members who act as delegates; last entry referes to "member_id" column of table "direct_interest_snapshot"';


CREATE TABLE "direct_supporter_snapshot" (
        "issue_id"              INT4            NOT NULL,
        PRIMARY KEY ("initiative_id", "event", "member_id"),
        "initiative_id"         INT4,
        "event"                 "snapshot_event",
        "member_id"             INT4            REFERENCES "member" ("id") ON DELETE RESTRICT ON UPDATE RESTRICT,
        "draft_id"              INT8            NOT NULL,
        "informed"              BOOLEAN         NOT NULL,
        "satisfied"             BOOLEAN         NOT NULL,
        FOREIGN KEY ("issue_id", "initiative_id") REFERENCES "initiative" ("issue_id", "id") ON DELETE CASCADE ON UPDATE CASCADE,
        FOREIGN KEY ("initiative_id", "draft_id") REFERENCES "draft" ("initiative_id", "id") ON DELETE NO ACTION ON UPDATE CASCADE,
        FOREIGN KEY ("issue_id", "event", "member_id") REFERENCES "direct_interest_snapshot" ("issue_id", "event", "member_id") ON DELETE CASCADE ON UPDATE CASCADE );
CREATE INDEX "direct_supporter_snapshot_member_id_idx" ON "direct_supporter_snapshot" ("member_id");

COMMENT ON TABLE "direct_supporter_snapshot" IS 'Snapshot of supporters of initiatives (weight is stored in "direct_interest_snapshot"); for corrections refer to column "issue_notice" of "issue" table';

COMMENT ON COLUMN "direct_supporter_snapshot"."issue_id"  IS 'WARNING: No index: For selections use column "initiative_id" and join via table "initiative" where neccessary';
COMMENT ON COLUMN "direct_supporter_snapshot"."event"     IS 'Reason for snapshot, see "snapshot_event" type for details';
COMMENT ON COLUMN "direct_supporter_snapshot"."informed"  IS 'Supporter has seen the latest draft of the initiative';
COMMENT ON COLUMN "direct_supporter_snapshot"."satisfied" IS 'Supporter has no "critical_opinion"s';


CREATE TABLE "non_voter" (
        PRIMARY KEY ("issue_id", "member_id"),
        "issue_id"              INT4            REFERENCES "issue" ("id") ON DELETE CASCADE ON UPDATE CASCADE,
        "member_id"             INT4            REFERENCES "member" ("id") ON DELETE CASCADE ON UPDATE CASCADE );
CREATE INDEX "non_voter_member_id_idx" ON "non_voter" ("member_id");

COMMENT ON TABLE "non_voter" IS 'Members who decided to not vote directly on an issue';


CREATE TABLE "direct_voter" (
        PRIMARY KEY ("issue_id", "member_id"),
        "issue_id"              INT4            REFERENCES "issue" ("id") ON DELETE CASCADE ON UPDATE CASCADE,
        "member_id"             INT4            REFERENCES "member" ("id") ON DELETE RESTRICT ON UPDATE RESTRICT,
        "weight"                INT4,
        "comment_changed"       TIMESTAMPTZ,
        "formatting_engine"     TEXT,
        "comment"               TEXT,
        "text_search_data"      TSVECTOR );
CREATE INDEX "direct_voter_member_id_idx" ON "direct_voter" ("member_id");
CREATE INDEX "direct_voter_text_search_data_idx" ON "direct_voter" USING gin ("text_search_data");
CREATE TRIGGER "update_text_search_data"
  BEFORE INSERT OR UPDATE ON "direct_voter"
  FOR EACH ROW EXECUTE PROCEDURE
  tsvector_update_trigger('text_search_data', 'pg_catalog.simple', "comment");

COMMENT ON TABLE "direct_voter" IS 'Members having directly voted for/against initiatives of an issue; frontends must ensure that no voters are added or removed to/from this table when the issue has been closed; for corrections refer to column "issue_notice" of "issue" table';

COMMENT ON COLUMN "direct_voter"."weight"            IS 'Weight of member (1 or higher) according to "delegating_voter" table';
COMMENT ON COLUMN "direct_voter"."comment_changed"   IS 'Shall be set on comment change, to indicate a comment being modified after voting has been finished; Automatically set to NULL after voting phase; Automatically set to NULL by trigger, if "comment" is set to NULL';
COMMENT ON COLUMN "direct_voter"."formatting_engine" IS 'Allows different formatting engines (i.e. wiki formats) to be used for "direct_voter"."comment"; Automatically set to NULL by trigger, if "comment" is set to NULL';
COMMENT ON COLUMN "direct_voter"."comment"           IS 'Is to be set or updated by the frontend, if comment was inserted or updated AFTER the issue has been closed. Otherwise it shall be set to NULL.';


CREATE TABLE "rendered_voter_comment" (
        PRIMARY KEY ("issue_id", "member_id", "format"),
        FOREIGN KEY ("issue_id", "member_id")
          REFERENCES "direct_voter" ("issue_id", "member_id")
          ON DELETE CASCADE ON UPDATE CASCADE,
        "issue_id"              INT4,
        "member_id"             INT4,
        "format"                TEXT,
        "content"               TEXT            NOT NULL );

COMMENT ON TABLE "rendered_voter_comment" IS 'This table may be used by frontends to cache "rendered" voter comments (e.g. HTML output generated from wiki text)';


CREATE TABLE "delegating_voter" (
        PRIMARY KEY ("issue_id", "member_id"),
        "issue_id"              INT4            REFERENCES "issue" ("id") ON DELETE CASCADE ON UPDATE CASCADE,
        "member_id"             INT4            REFERENCES "member" ("id") ON DELETE RESTRICT ON UPDATE RESTRICT,
        "weight"                INT4,
        "scope"              "delegation_scope" NOT NULL,
        "delegate_member_ids"   INT4[]          NOT NULL );
CREATE INDEX "delegating_voter_member_id_idx" ON "delegating_voter" ("member_id");

COMMENT ON TABLE "delegating_voter" IS 'Delegations increasing the weight of entries in the "direct_voter" table; for corrections refer to column "issue_notice" of "issue" table';

COMMENT ON COLUMN "delegating_voter"."member_id"           IS 'Delegating member';
COMMENT ON COLUMN "delegating_voter"."weight"              IS 'Intermediate weight';
COMMENT ON COLUMN "delegating_voter"."delegate_member_ids" IS 'Chain of members who act as delegates; last entry referes to "member_id" column of table "direct_voter"';


CREATE TABLE "vote" (
        "issue_id"              INT4            NOT NULL,
        PRIMARY KEY ("initiative_id", "member_id"),
        "initiative_id"         INT4,
        "member_id"             INT4,
        "grade"                 INT4            NOT NULL,
        "first_preference"      BOOLEAN,
        FOREIGN KEY ("issue_id", "initiative_id") REFERENCES "initiative" ("issue_id", "id") ON DELETE CASCADE ON UPDATE CASCADE,
        FOREIGN KEY ("issue_id", "member_id") REFERENCES "direct_voter" ("issue_id", "member_id") ON DELETE CASCADE ON UPDATE CASCADE,
        CONSTRAINT "first_preference_flag_only_set_on_positive_grades"
          CHECK ("grade" > 0 OR "first_preference" ISNULL) );
CREATE INDEX "vote_member_id_idx" ON "vote" ("member_id");

COMMENT ON TABLE "vote" IS 'Manual and delegated votes without abstentions; frontends must ensure that no votes are added modified or removed when the issue has been closed; for corrections refer to column "issue_notice" of "issue" table';

COMMENT ON COLUMN "vote"."issue_id"         IS 'WARNING: No index: For selections use column "initiative_id" and join via table "initiative" where neccessary';
COMMENT ON COLUMN "vote"."grade"            IS 'Values smaller than zero mean reject, values greater than zero mean acceptance, zero or missing row means abstention. Preferences are expressed by different positive or negative numbers.';
COMMENT ON COLUMN "vote"."first_preference" IS 'Value is automatically set after voting is finished. For positive grades, this value is set to true for the highest (i.e. best) grade.';


CREATE TYPE "event_type" AS ENUM (
        'issue_state_changed',
        'initiative_created_in_new_issue',
        'initiative_created_in_existing_issue',
        'initiative_revoked',
        'new_draft_created',
        'suggestion_created');

COMMENT ON TYPE "event_type" IS 'Type used for column "event" of table "event"';


CREATE TABLE "event" (
        "id"                    SERIAL8         PRIMARY KEY,
        "occurrence"            TIMESTAMPTZ     NOT NULL DEFAULT now(),
        "event"                 "event_type"    NOT NULL,
        "member_id"             INT4            REFERENCES "member" ("id") ON DELETE RESTRICT ON UPDATE CASCADE,
        "issue_id"              INT4            REFERENCES "issue" ("id") ON DELETE CASCADE ON UPDATE CASCADE,
        "state"                 "issue_state",
        "initiative_id"         INT4,
        "draft_id"              INT8,
        "suggestion_id"         INT8,
        FOREIGN KEY ("issue_id", "initiative_id")
          REFERENCES "initiative" ("issue_id", "id")
          ON DELETE CASCADE ON UPDATE CASCADE,
        FOREIGN KEY ("initiative_id", "draft_id")
          REFERENCES "draft" ("initiative_id", "id")
          ON DELETE CASCADE ON UPDATE CASCADE,
        FOREIGN KEY ("initiative_id", "suggestion_id")
          REFERENCES "suggestion" ("initiative_id", "id")
          ON DELETE CASCADE ON UPDATE CASCADE,
        CONSTRAINT "null_constr_for_issue_state_changed" CHECK (
          "event" != 'issue_state_changed' OR (
            "member_id"     ISNULL  AND
            "issue_id"      NOTNULL AND
            "state"         NOTNULL AND
            "initiative_id" ISNULL  AND
            "draft_id"      ISNULL  AND
            "suggestion_id" ISNULL  )),
        CONSTRAINT "null_constr_for_initiative_creation_or_revocation_or_new_draft" CHECK (
          "event" NOT IN (
            'initiative_created_in_new_issue',
            'initiative_created_in_existing_issue',
            'initiative_revoked',
            'new_draft_created'
          ) OR (
            "member_id"     NOTNULL AND
            "issue_id"      NOTNULL AND
            "state"         NOTNULL AND
            "initiative_id" NOTNULL AND
            "draft_id"      NOTNULL AND
            "suggestion_id" ISNULL  )),
        CONSTRAINT "null_constr_for_suggestion_creation" CHECK (
          "event" != 'suggestion_created' OR (
            "member_id"     NOTNULL AND
            "issue_id"      NOTNULL AND
            "state"         NOTNULL AND
            "initiative_id" NOTNULL AND
            "draft_id"      ISNULL  AND
            "suggestion_id" NOTNULL )) );
CREATE INDEX "event_occurrence_idx" ON "event" ("occurrence");

COMMENT ON TABLE "event" IS 'Event table, automatically filled by triggers';

COMMENT ON COLUMN "event"."occurrence" IS 'Point in time, when event occurred';
COMMENT ON COLUMN "event"."event"      IS 'Type of event (see TYPE "event_type")';
COMMENT ON COLUMN "event"."member_id"  IS 'Member who caused the event, if applicable';
COMMENT ON COLUMN "event"."state"      IS 'If issue_id is set: state of affected issue; If state changed: new state';


CREATE TABLE "notification_event_sent" (
        "event_id"              INT8            NOT NULL );
CREATE UNIQUE INDEX "notification_event_sent_singleton_idx" ON "notification_event_sent" ((1));

COMMENT ON TABLE "notification_event_sent" IS 'This table stores one row with the last event_id, for which notifications have been sent out';
COMMENT ON INDEX "notification_event_sent_singleton_idx" IS 'This index ensures that "notification_event_sent" only contains one row maximum.';


CREATE TABLE "notification_initiative_sent" (
        PRIMARY KEY ("member_id", "initiative_id"),
        "member_id"             INT4            REFERENCES "member" ("id") ON DELETE CASCADE ON UPDATE CASCADE,
        "initiative_id"         INT4            REFERENCES "initiative" ("id") ON DELETE CASCADE ON UPDATE CASCADE,
        "last_draft_id"         INT8            NOT NULL,
        "last_suggestion_id"    INT8 );
CREATE INDEX "notification_initiative_sent_initiative_idx" ON "notification_initiative_sent" ("initiative_id");

COMMENT ON TABLE "notification_initiative_sent" IS 'Information which initiatives have been promoted to a member in a scheduled notification mail';

COMMENT ON COLUMN "notification_initiative_sent"."last_draft_id"      IS 'Current (i.e. last) draft_id when initiative had been promoted';
COMMENT ON COLUMN "notification_initiative_sent"."last_suggestion_id" IS 'Current (i.e. last) draft_id when initiative had been promoted';


CREATE TABLE "newsletter" (
        "id"                    SERIAL4         PRIMARY KEY,
        "published"             TIMESTAMPTZ     NOT NULL,
        "unit_id"               INT4            REFERENCES "unit" ("id") ON DELETE CASCADE ON UPDATE CASCADE,
        "include_all_members"   BOOLEAN         NOT NULL,
        "sent"                  TIMESTAMPTZ,
        "subject"               TEXT            NOT NULL,
        "content"               TEXT            NOT NULL );
CREATE INDEX "newsletter_unit_id_idx" ON "newsletter" ("unit_id", "published");
CREATE INDEX "newsletter_all_units_published_idx" ON "newsletter" ("published") WHERE "unit_id" ISNULL;
CREATE INDEX "newsletter_published_idx" ON "newsletter" ("published");

COMMENT ON TABLE "newsletter" IS 'Contains newsletters created by administrators to be sent out and for further reference';

COMMENT ON COLUMN "newsletter"."published"           IS 'Timestamp when the newsletter is to be sent out (and made available in the frontend)';
COMMENT ON COLUMN "newsletter"."unit_id"             IS 'If set, only members with voting right in the given unit are considered to be recipients';
COMMENT ON COLUMN "newsletter"."include_all_members" IS 'TRUE = include all members regardless of their ''disable_notifications'' setting';
COMMENT ON COLUMN "newsletter"."sent"                IS 'Timestamp when the newsletter has been mailed out';
COMMENT ON COLUMN "newsletter"."subject"             IS 'Subject line (e.g. to be used for the email)';
COMMENT ON COLUMN "newsletter"."content"             IS 'Plain text content of the newsletter';



----------------------------------------------
-- Writing of history entries and event log --
----------------------------------------------


CREATE FUNCTION "write_member_history_trigger"()
  RETURNS TRIGGER
  LANGUAGE 'plpgsql' VOLATILE AS $$
    BEGIN
      IF
        ( NEW."active" != OLD."active" OR
          NEW."name"   != OLD."name" ) AND
        OLD."activated" NOTNULL
      THEN
        INSERT INTO "member_history"
          ("member_id", "active", "name")
          VALUES (NEW."id", OLD."active", OLD."name");
      END IF;
      RETURN NULL;
    END;
  $$;

CREATE TRIGGER "write_member_history"
  AFTER UPDATE ON "member" FOR EACH ROW EXECUTE PROCEDURE
  "write_member_history_trigger"();

COMMENT ON FUNCTION "write_member_history_trigger"()  IS 'Implementation of trigger "write_member_history" on table "member"';
COMMENT ON TRIGGER "write_member_history" ON "member" IS 'When changing certain fields of a member, create a history entry in "member_history" table';


CREATE FUNCTION "write_event_issue_state_changed_trigger"()
  RETURNS TRIGGER
  LANGUAGE 'plpgsql' VOLATILE AS $$
    BEGIN
      IF NEW."state" != OLD."state" THEN
        INSERT INTO "event" ("event", "issue_id", "state")
          VALUES ('issue_state_changed', NEW."id", NEW."state");
      END IF;
      RETURN NULL;
    END;
  $$;

CREATE TRIGGER "write_event_issue_state_changed"
  AFTER UPDATE ON "issue" FOR EACH ROW EXECUTE PROCEDURE
  "write_event_issue_state_changed_trigger"();

COMMENT ON FUNCTION "write_event_issue_state_changed_trigger"() IS 'Implementation of trigger "write_event_issue_state_changed" on table "issue"';
COMMENT ON TRIGGER "write_event_issue_state_changed" ON "issue" IS 'Create entry in "event" table on "state" change';


CREATE FUNCTION "write_event_initiative_or_draft_created_trigger"()
  RETURNS TRIGGER
  LANGUAGE 'plpgsql' VOLATILE AS $$
    DECLARE
      "initiative_row" "initiative"%ROWTYPE;
      "issue_row"      "issue"%ROWTYPE;
      "event_v"        "event_type";
    BEGIN
      SELECT * INTO "initiative_row" FROM "initiative"
        WHERE "id" = NEW."initiative_id";
      SELECT * INTO "issue_row" FROM "issue"
        WHERE "id" = "initiative_row"."issue_id";
      IF EXISTS (
        SELECT NULL FROM "draft"
        WHERE "initiative_id" = NEW."initiative_id"
        AND "id" != NEW."id"
      ) THEN
        "event_v" := 'new_draft_created';
      ELSE
        IF EXISTS (
          SELECT NULL FROM "initiative"
          WHERE "issue_id" = "initiative_row"."issue_id"
          AND "id" != "initiative_row"."id"
        ) THEN
          "event_v" := 'initiative_created_in_existing_issue';
        ELSE
          "event_v" := 'initiative_created_in_new_issue';
        END IF;
      END IF;
      INSERT INTO "event" (
          "event", "member_id",
          "issue_id", "state", "initiative_id", "draft_id"
        ) VALUES (
          "event_v",
          NEW."author_id",
          "initiative_row"."issue_id",
          "issue_row"."state",
          "initiative_row"."id",
          NEW."id" );
      RETURN NULL;
    END;
  $$;

CREATE TRIGGER "write_event_initiative_or_draft_created"
  AFTER INSERT ON "draft" FOR EACH ROW EXECUTE PROCEDURE
  "write_event_initiative_or_draft_created_trigger"();

COMMENT ON FUNCTION "write_event_initiative_or_draft_created_trigger"() IS 'Implementation of trigger "write_event_initiative_or_draft_created" on table "issue"';
COMMENT ON TRIGGER "write_event_initiative_or_draft_created" ON "draft" IS 'Create entry in "event" table on draft creation';


CREATE FUNCTION "write_event_initiative_revoked_trigger"()
  RETURNS TRIGGER
  LANGUAGE 'plpgsql' VOLATILE AS $$
    DECLARE
      "issue_row"  "issue"%ROWTYPE;
      "draft_id_v" "draft"."id"%TYPE;
    BEGIN
      IF OLD."revoked" ISNULL AND NEW."revoked" NOTNULL THEN
        SELECT * INTO "issue_row" FROM "issue"
          WHERE "id" = NEW."issue_id";
        SELECT "id" INTO "draft_id_v" FROM "current_draft"
          WHERE "initiative_id" = NEW."id";
        INSERT INTO "event" (
            "event", "member_id", "issue_id", "state", "initiative_id", "draft_id"
          ) VALUES (
            'initiative_revoked',
            NEW."revoked_by_member_id",
            NEW."issue_id",
            "issue_row"."state",
            NEW."id",
            "draft_id_v");
      END IF;
      RETURN NULL;
    END;
  $$;

CREATE TRIGGER "write_event_initiative_revoked"
  AFTER UPDATE ON "initiative" FOR EACH ROW EXECUTE PROCEDURE
  "write_event_initiative_revoked_trigger"();

COMMENT ON FUNCTION "write_event_initiative_revoked_trigger"()      IS 'Implementation of trigger "write_event_initiative_revoked" on table "issue"';
COMMENT ON TRIGGER "write_event_initiative_revoked" ON "initiative" IS 'Create entry in "event" table, when an initiative is revoked';


CREATE FUNCTION "write_event_suggestion_created_trigger"()
  RETURNS TRIGGER
  LANGUAGE 'plpgsql' VOLATILE AS $$
    DECLARE
      "initiative_row" "initiative"%ROWTYPE;
      "issue_row"      "issue"%ROWTYPE;
    BEGIN
      SELECT * INTO "initiative_row" FROM "initiative"
        WHERE "id" = NEW."initiative_id";
      SELECT * INTO "issue_row" FROM "issue"
        WHERE "id" = "initiative_row"."issue_id";
      INSERT INTO "event" (
          "event", "member_id",
          "issue_id", "state", "initiative_id", "suggestion_id"
        ) VALUES (
          'suggestion_created',
          NEW."author_id",
          "initiative_row"."issue_id",
          "issue_row"."state",
          "initiative_row"."id",
          NEW."id" );
      RETURN NULL;
    END;
  $$;

CREATE TRIGGER "write_event_suggestion_created"
  AFTER INSERT ON "suggestion" FOR EACH ROW EXECUTE PROCEDURE
  "write_event_suggestion_created_trigger"();

COMMENT ON FUNCTION "write_event_suggestion_created_trigger"()      IS 'Implementation of trigger "write_event_suggestion_created" on table "issue"';
COMMENT ON TRIGGER "write_event_suggestion_created" ON "suggestion" IS 'Create entry in "event" table on suggestion creation';



----------------------------
-- Additional constraints --
----------------------------


CREATE FUNCTION "issue_requires_first_initiative_trigger"()
  RETURNS TRIGGER
  LANGUAGE 'plpgsql' VOLATILE AS $$
    BEGIN
      IF NOT EXISTS (
        SELECT NULL FROM "initiative" WHERE "issue_id" = NEW."id"
      ) THEN
        RAISE EXCEPTION 'Cannot create issue without an initial initiative.' USING
          ERRCODE = 'integrity_constraint_violation',
          HINT    = 'Create issue, initiative, and draft within the same transaction.';
      END IF;
      RETURN NULL;
    END;
  $$;

CREATE CONSTRAINT TRIGGER "issue_requires_first_initiative"
  AFTER INSERT OR UPDATE ON "issue" DEFERRABLE INITIALLY DEFERRED
  FOR EACH ROW EXECUTE PROCEDURE
  "issue_requires_first_initiative_trigger"();

COMMENT ON FUNCTION "issue_requires_first_initiative_trigger"() IS 'Implementation of trigger "issue_requires_first_initiative" on table "issue"';
COMMENT ON TRIGGER "issue_requires_first_initiative" ON "issue" IS 'Ensure that new issues have at least one initiative';


CREATE FUNCTION "last_initiative_deletes_issue_trigger"()
  RETURNS TRIGGER
  LANGUAGE 'plpgsql' VOLATILE AS $$
    DECLARE
      "reference_lost" BOOLEAN;
    BEGIN
      IF TG_OP = 'DELETE' THEN
        "reference_lost" := TRUE;
      ELSE
        "reference_lost" := NEW."issue_id" != OLD."issue_id";
      END IF;
      IF
        "reference_lost" AND NOT EXISTS (
          SELECT NULL FROM "initiative" WHERE "issue_id" = OLD."issue_id"
        )
      THEN
        DELETE FROM "issue" WHERE "id" = OLD."issue_id";
      END IF;
      RETURN NULL;
    END;
  $$;

CREATE CONSTRAINT TRIGGER "last_initiative_deletes_issue"
  AFTER UPDATE OR DELETE ON "initiative" DEFERRABLE INITIALLY DEFERRED
  FOR EACH ROW EXECUTE PROCEDURE
  "last_initiative_deletes_issue_trigger"();

COMMENT ON FUNCTION "last_initiative_deletes_issue_trigger"()      IS 'Implementation of trigger "last_initiative_deletes_issue" on table "initiative"';
COMMENT ON TRIGGER "last_initiative_deletes_issue" ON "initiative" IS 'Removing the last initiative of an issue deletes the issue';


CREATE FUNCTION "initiative_requires_first_draft_trigger"()
  RETURNS TRIGGER
  LANGUAGE 'plpgsql' VOLATILE AS $$
    BEGIN
      IF NOT EXISTS (
        SELECT NULL FROM "draft" WHERE "initiative_id" = NEW."id"
      ) THEN
        RAISE EXCEPTION 'Cannot create initiative without an initial draft.' USING
          ERRCODE = 'integrity_constraint_violation',
          HINT    = 'Create issue, initiative and draft within the same transaction.';
      END IF;
      RETURN NULL;
    END;
  $$;

CREATE CONSTRAINT TRIGGER "initiative_requires_first_draft"
  AFTER INSERT OR UPDATE ON "initiative" DEFERRABLE INITIALLY DEFERRED
  FOR EACH ROW EXECUTE PROCEDURE
  "initiative_requires_first_draft_trigger"();

COMMENT ON FUNCTION "initiative_requires_first_draft_trigger"()      IS 'Implementation of trigger "initiative_requires_first_draft" on table "initiative"';
COMMENT ON TRIGGER "initiative_requires_first_draft" ON "initiative" IS 'Ensure that new initiatives have at least one draft';


CREATE FUNCTION "last_draft_deletes_initiative_trigger"()
  RETURNS TRIGGER
  LANGUAGE 'plpgsql' VOLATILE AS $$
    DECLARE
      "reference_lost" BOOLEAN;
    BEGIN
      IF TG_OP = 'DELETE' THEN
        "reference_lost" := TRUE;
      ELSE
        "reference_lost" := NEW."initiative_id" != OLD."initiative_id";
      END IF;
      IF
        "reference_lost" AND NOT EXISTS (
          SELECT NULL FROM "draft" WHERE "initiative_id" = OLD."initiative_id"
        )
      THEN
        DELETE FROM "initiative" WHERE "id" = OLD."initiative_id";
      END IF;
      RETURN NULL;
    END;
  $$;

CREATE CONSTRAINT TRIGGER "last_draft_deletes_initiative"
  AFTER UPDATE OR DELETE ON "draft" DEFERRABLE INITIALLY DEFERRED
  FOR EACH ROW EXECUTE PROCEDURE
  "last_draft_deletes_initiative_trigger"();

COMMENT ON FUNCTION "last_draft_deletes_initiative_trigger"() IS 'Implementation of trigger "last_draft_deletes_initiative" on table "draft"';
COMMENT ON TRIGGER "last_draft_deletes_initiative" ON "draft" IS 'Removing the last draft of an initiative deletes the initiative';


CREATE FUNCTION "suggestion_requires_first_opinion_trigger"()
  RETURNS TRIGGER
  LANGUAGE 'plpgsql' VOLATILE AS $$
    BEGIN
      IF NOT EXISTS (
        SELECT NULL FROM "opinion" WHERE "suggestion_id" = NEW."id"
      ) THEN
        RAISE EXCEPTION 'Cannot create a suggestion without an opinion.' USING
          ERRCODE = 'integrity_constraint_violation',
          HINT    = 'Create suggestion and opinion within the same transaction.';
      END IF;
      RETURN NULL;
    END;
  $$;

CREATE CONSTRAINT TRIGGER "suggestion_requires_first_opinion"
  AFTER INSERT OR UPDATE ON "suggestion" DEFERRABLE INITIALLY DEFERRED
  FOR EACH ROW EXECUTE PROCEDURE
  "suggestion_requires_first_opinion_trigger"();

COMMENT ON FUNCTION "suggestion_requires_first_opinion_trigger"()      IS 'Implementation of trigger "suggestion_requires_first_opinion" on table "suggestion"';
COMMENT ON TRIGGER "suggestion_requires_first_opinion" ON "suggestion" IS 'Ensure that new suggestions have at least one opinion';


CREATE FUNCTION "last_opinion_deletes_suggestion_trigger"()
  RETURNS TRIGGER
  LANGUAGE 'plpgsql' VOLATILE AS $$
    DECLARE
      "reference_lost" BOOLEAN;
    BEGIN
      IF TG_OP = 'DELETE' THEN
        "reference_lost" := TRUE;
      ELSE
        "reference_lost" := NEW."suggestion_id" != OLD."suggestion_id";
      END IF;
      IF
        "reference_lost" AND NOT EXISTS (
          SELECT NULL FROM "opinion" WHERE "suggestion_id" = OLD."suggestion_id"
        )
      THEN
        DELETE FROM "suggestion" WHERE "id" = OLD."suggestion_id";
      END IF;
      RETURN NULL;
    END;
  $$;

CREATE CONSTRAINT TRIGGER "last_opinion_deletes_suggestion"
  AFTER UPDATE OR DELETE ON "opinion" DEFERRABLE INITIALLY DEFERRED
  FOR EACH ROW EXECUTE PROCEDURE
  "last_opinion_deletes_suggestion_trigger"();

COMMENT ON FUNCTION "last_opinion_deletes_suggestion_trigger"()   IS 'Implementation of trigger "last_opinion_deletes_suggestion" on table "opinion"';
COMMENT ON TRIGGER "last_opinion_deletes_suggestion" ON "opinion" IS 'Removing the last opinion of a suggestion deletes the suggestion';


CREATE FUNCTION "non_voter_deletes_direct_voter_trigger"()
  RETURNS TRIGGER
  LANGUAGE 'plpgsql' VOLATILE AS $$
    BEGIN
      DELETE FROM "direct_voter"
        WHERE "issue_id" = NEW."issue_id" AND "member_id" = NEW."member_id";
      RETURN NULL;
    END;
  $$;

CREATE TRIGGER "non_voter_deletes_direct_voter"
  AFTER INSERT OR UPDATE ON "non_voter"
  FOR EACH ROW EXECUTE PROCEDURE
  "non_voter_deletes_direct_voter_trigger"();

COMMENT ON FUNCTION "non_voter_deletes_direct_voter_trigger"()     IS 'Implementation of trigger "non_voter_deletes_direct_voter" on table "non_voter"';
COMMENT ON TRIGGER "non_voter_deletes_direct_voter" ON "non_voter" IS 'An entry in the "non_voter" table deletes an entry in the "direct_voter" table (and vice versa due to trigger "direct_voter_deletes_non_voter" on table "direct_voter")';


CREATE FUNCTION "direct_voter_deletes_non_voter_trigger"()
  RETURNS TRIGGER
  LANGUAGE 'plpgsql' VOLATILE AS $$
    BEGIN
      DELETE FROM "non_voter"
        WHERE "issue_id" = NEW."issue_id" AND "member_id" = NEW."member_id";
      RETURN NULL;
    END;
  $$;

CREATE TRIGGER "direct_voter_deletes_non_voter"
  AFTER INSERT OR UPDATE ON "direct_voter"
  FOR EACH ROW EXECUTE PROCEDURE
  "direct_voter_deletes_non_voter_trigger"();

COMMENT ON FUNCTION "direct_voter_deletes_non_voter_trigger"()        IS 'Implementation of trigger "direct_voter_deletes_non_voter" on table "direct_voter"';
COMMENT ON TRIGGER "direct_voter_deletes_non_voter" ON "direct_voter" IS 'An entry in the "direct_voter" table deletes an entry in the "non_voter" table (and vice versa due to trigger "non_voter_deletes_direct_voter" on table "non_voter")';


CREATE FUNCTION "voter_comment_fields_only_set_when_voter_comment_is_set_trigger"()
  RETURNS TRIGGER
  LANGUAGE 'plpgsql' VOLATILE AS $$
    BEGIN
      IF NEW."comment" ISNULL THEN
        NEW."comment_changed" := NULL;
        NEW."formatting_engine" := NULL;
      END IF;
      RETURN NEW;
    END;
  $$;

CREATE TRIGGER "voter_comment_fields_only_set_when_voter_comment_is_set"
  BEFORE INSERT OR UPDATE ON "direct_voter"
  FOR EACH ROW EXECUTE PROCEDURE
  "voter_comment_fields_only_set_when_voter_comment_is_set_trigger"();

COMMENT ON FUNCTION "voter_comment_fields_only_set_when_voter_comment_is_set_trigger"() IS 'Implementation of trigger "voter_comment_fields_only_set_when_voter_comment_is_set" ON table "direct_voter"';
COMMENT ON TRIGGER "voter_comment_fields_only_set_when_voter_comment_is_set" ON "direct_voter" IS 'If "comment" is set to NULL, then other comment related fields are also set to NULL.';


---------------------------------------------------------------
-- Ensure that votes are not modified when issues are closed --
---------------------------------------------------------------

-- NOTE: Frontends should ensure this anyway, but in case of programming
-- errors the following triggers ensure data integrity.


CREATE FUNCTION "forbid_changes_on_closed_issue_trigger"()
  RETURNS TRIGGER
  LANGUAGE 'plpgsql' VOLATILE AS $$
    DECLARE
      "issue_id_v" "issue"."id"%TYPE;
      "issue_row"  "issue"%ROWTYPE;
    BEGIN
      IF EXISTS (
        SELECT NULL FROM "temporary_transaction_data"
        WHERE "txid" = txid_current()
        AND "key" = 'override_protection_triggers'
        AND "value" = TRUE::TEXT
      ) THEN
        RETURN NULL;
      END IF;
      IF TG_OP = 'DELETE' THEN
        "issue_id_v" := OLD."issue_id";
      ELSE
        "issue_id_v" := NEW."issue_id";
      END IF;
      SELECT INTO "issue_row" * FROM "issue"
        WHERE "id" = "issue_id_v" FOR SHARE;
      IF (
        "issue_row"."closed" NOTNULL OR (
          "issue_row"."state" = 'voting' AND
          "issue_row"."phase_finished" NOTNULL
        )
      ) THEN
        IF
          TG_RELID = 'direct_voter'::regclass AND
          TG_OP = 'UPDATE'
        THEN
          IF
            OLD."issue_id"  = NEW."issue_id"  AND
            OLD."member_id" = NEW."member_id" AND
            OLD."weight" = NEW."weight"
          THEN
            RETURN NULL;  -- allows changing of voter comment
          END IF;
        END IF;
        RAISE EXCEPTION 'Tried to modify data after voting has been closed.' USING
          ERRCODE = 'integrity_constraint_violation';
      END IF;
      RETURN NULL;
    END;
  $$;

CREATE TRIGGER "forbid_changes_on_closed_issue"
  AFTER INSERT OR UPDATE OR DELETE ON "direct_voter"
  FOR EACH ROW EXECUTE PROCEDURE
  "forbid_changes_on_closed_issue_trigger"();

CREATE TRIGGER "forbid_changes_on_closed_issue"
  AFTER INSERT OR UPDATE OR DELETE ON "delegating_voter"
  FOR EACH ROW EXECUTE PROCEDURE
  "forbid_changes_on_closed_issue_trigger"();

CREATE TRIGGER "forbid_changes_on_closed_issue"
  AFTER INSERT OR UPDATE OR DELETE ON "vote"
  FOR EACH ROW EXECUTE PROCEDURE
  "forbid_changes_on_closed_issue_trigger"();

COMMENT ON FUNCTION "forbid_changes_on_closed_issue_trigger"()            IS 'Implementation of triggers "forbid_changes_on_closed_issue" on tables "direct_voter", "delegating_voter" and "vote"';
COMMENT ON TRIGGER "forbid_changes_on_closed_issue" ON "direct_voter"     IS 'Ensures that frontends can''t tamper with votings of closed issues, in case of programming errors';
COMMENT ON TRIGGER "forbid_changes_on_closed_issue" ON "delegating_voter" IS 'Ensures that frontends can''t tamper with votings of closed issues, in case of programming errors';
COMMENT ON TRIGGER "forbid_changes_on_closed_issue" ON "vote"             IS 'Ensures that frontends can''t tamper with votings of closed issues, in case of programming errors';



--------------------------------------------------------------------
-- Auto-retrieval of fields only needed for referential integrity --
--------------------------------------------------------------------


CREATE FUNCTION "autofill_issue_id_trigger"()
  RETURNS TRIGGER
  LANGUAGE 'plpgsql' VOLATILE AS $$
    BEGIN
      IF NEW."issue_id" ISNULL THEN
        SELECT "issue_id" INTO NEW."issue_id"
          FROM "initiative" WHERE "id" = NEW."initiative_id";
      END IF;
      RETURN NEW;
    END;
  $$;

CREATE TRIGGER "autofill_issue_id" BEFORE INSERT ON "supporter"
  FOR EACH ROW EXECUTE PROCEDURE "autofill_issue_id_trigger"();

CREATE TRIGGER "autofill_issue_id" BEFORE INSERT ON "vote"
  FOR EACH ROW EXECUTE PROCEDURE "autofill_issue_id_trigger"();

COMMENT ON FUNCTION "autofill_issue_id_trigger"()     IS 'Implementation of triggers "autofill_issue_id" on tables "supporter" and "vote"';
COMMENT ON TRIGGER "autofill_issue_id" ON "supporter" IS 'Set "issue_id" field automatically, if NULL';
COMMENT ON TRIGGER "autofill_issue_id" ON "vote"      IS 'Set "issue_id" field automatically, if NULL';


CREATE FUNCTION "autofill_initiative_id_trigger"()
  RETURNS TRIGGER
  LANGUAGE 'plpgsql' VOLATILE AS $$
    BEGIN
      IF NEW."initiative_id" ISNULL THEN
        SELECT "initiative_id" INTO NEW."initiative_id"
          FROM "suggestion" WHERE "id" = NEW."suggestion_id";
      END IF;
      RETURN NEW;
    END;
  $$;

CREATE TRIGGER "autofill_initiative_id" BEFORE INSERT ON "opinion"
  FOR EACH ROW EXECUTE PROCEDURE "autofill_initiative_id_trigger"();

COMMENT ON FUNCTION "autofill_initiative_id_trigger"()   IS 'Implementation of trigger "autofill_initiative_id" on table "opinion"';
COMMENT ON TRIGGER "autofill_initiative_id" ON "opinion" IS 'Set "initiative_id" field automatically, if NULL';



-----------------------------------------------------
-- Automatic calculation of certain default values --
-----------------------------------------------------


CREATE FUNCTION "copy_timings_trigger"()
  RETURNS TRIGGER
  LANGUAGE 'plpgsql' VOLATILE AS $$
    DECLARE
      "policy_row" "policy"%ROWTYPE;
    BEGIN
      SELECT * INTO "policy_row" FROM "policy"
        WHERE "id" = NEW."policy_id";
      IF NEW."min_admission_time" ISNULL THEN
        NEW."min_admission_time" := "policy_row"."min_admission_time";
      END IF;
      IF NEW."max_admission_time" ISNULL THEN
        NEW."max_admission_time" := "policy_row"."max_admission_time";
      END IF;
      IF NEW."discussion_time" ISNULL THEN
        NEW."discussion_time" := "policy_row"."discussion_time";
      END IF;
      IF NEW."verification_time" ISNULL THEN
        NEW."verification_time" := "policy_row"."verification_time";
      END IF;
      IF NEW."voting_time" ISNULL THEN
        NEW."voting_time" := "policy_row"."voting_time";
      END IF;
      RETURN NEW;
    END;
  $$;

CREATE TRIGGER "copy_timings" BEFORE INSERT OR UPDATE ON "issue"
  FOR EACH ROW EXECUTE PROCEDURE "copy_timings_trigger"();

COMMENT ON FUNCTION "copy_timings_trigger"() IS 'Implementation of trigger "copy_timings" on table "issue"';
COMMENT ON TRIGGER "copy_timings" ON "issue" IS 'If timing fields are NULL, copy values from policy.';


CREATE FUNCTION "default_for_draft_id_trigger"()
  RETURNS TRIGGER
  LANGUAGE 'plpgsql' VOLATILE AS $$
    BEGIN
      IF NEW."draft_id" ISNULL THEN
        SELECT "id" INTO NEW."draft_id" FROM "current_draft"
          WHERE "initiative_id" = NEW."initiative_id";
      END IF;
      RETURN NEW;
    END;
  $$;

CREATE TRIGGER "default_for_draft_id" BEFORE INSERT OR UPDATE ON "suggestion"
  FOR EACH ROW EXECUTE PROCEDURE "default_for_draft_id_trigger"();
CREATE TRIGGER "default_for_draft_id" BEFORE INSERT OR UPDATE ON "supporter"
  FOR EACH ROW EXECUTE PROCEDURE "default_for_draft_id_trigger"();

COMMENT ON FUNCTION "default_for_draft_id_trigger"() IS 'Implementation of trigger "default_for_draft" on tables "supporter" and "suggestion"';
COMMENT ON TRIGGER "default_for_draft_id" ON "suggestion" IS 'If "draft_id" is NULL, then use the current draft of the initiative as default';
COMMENT ON TRIGGER "default_for_draft_id" ON "supporter"  IS 'If "draft_id" is NULL, then use the current draft of the initiative as default';



----------------------------------------
-- Automatic creation of dependencies --
----------------------------------------


CREATE FUNCTION "autocreate_interest_trigger"()
  RETURNS TRIGGER
  LANGUAGE 'plpgsql' VOLATILE AS $$
    BEGIN
      IF NOT EXISTS (
        SELECT NULL FROM "initiative" JOIN "interest"
        ON "initiative"."issue_id" = "interest"."issue_id"
        WHERE "initiative"."id" = NEW."initiative_id"
        AND "interest"."member_id" = NEW."member_id"
      ) THEN
        BEGIN
          INSERT INTO "interest" ("issue_id", "member_id")
            SELECT "issue_id", NEW."member_id"
            FROM "initiative" WHERE "id" = NEW."initiative_id";
        EXCEPTION WHEN unique_violation THEN END;
      END IF;
      RETURN NEW;
    END;
  $$;

CREATE TRIGGER "autocreate_interest" BEFORE INSERT ON "supporter"
  FOR EACH ROW EXECUTE PROCEDURE "autocreate_interest_trigger"();

COMMENT ON FUNCTION "autocreate_interest_trigger"()     IS 'Implementation of trigger "autocreate_interest" on table "supporter"';
COMMENT ON TRIGGER "autocreate_interest" ON "supporter" IS 'Supporting an initiative implies interest in the issue, thus automatically creates an entry in the "interest" table';


CREATE FUNCTION "autocreate_supporter_trigger"()
  RETURNS TRIGGER
  LANGUAGE 'plpgsql' VOLATILE AS $$
    BEGIN
      IF NOT EXISTS (
        SELECT NULL FROM "suggestion" JOIN "supporter"
        ON "suggestion"."initiative_id" = "supporter"."initiative_id"
        WHERE "suggestion"."id" = NEW."suggestion_id"
        AND "supporter"."member_id" = NEW."member_id"
      ) THEN
        BEGIN
          INSERT INTO "supporter" ("initiative_id", "member_id")
            SELECT "initiative_id", NEW."member_id"
            FROM "suggestion" WHERE "id" = NEW."suggestion_id";
        EXCEPTION WHEN unique_violation THEN END;
      END IF;
      RETURN NEW;
    END;
  $$;

CREATE TRIGGER "autocreate_supporter" BEFORE INSERT ON "opinion"
  FOR EACH ROW EXECUTE PROCEDURE "autocreate_supporter_trigger"();

COMMENT ON FUNCTION "autocreate_supporter_trigger"()   IS 'Implementation of trigger "autocreate_supporter" on table "opinion"';
COMMENT ON TRIGGER "autocreate_supporter" ON "opinion" IS 'Opinions can only be added for supported initiatives. This trigger automatrically creates an entry in the "supporter" table, if not existent yet.';



------------------------------------------
-- Views and helper functions for views --
------------------------------------------


CREATE VIEW "member_eligible_to_be_notified" AS
  SELECT * FROM "member"
  WHERE "activated" NOTNULL AND "locked" = FALSE;

COMMENT ON VIEW "member_eligible_to_be_notified" IS 'Filtered "member" table containing only activated and non-locked members (used as helper view for "member_to_notify" and "newsletter_to_send")';


CREATE VIEW "member_to_notify" AS
  SELECT * FROM "member_eligible_to_be_notified"
  WHERE "disable_notifications" = FALSE;

COMMENT ON VIEW "member_to_notify" IS 'Filtered "member" table containing only members that are eligible to and wish to receive notifications; NOTE: "notify_email" may still be NULL and might need to be checked by frontend (this allows other means of messaging)';


CREATE VIEW "unit_delegation" AS
  SELECT
    "unit"."id" AS "unit_id",
    "delegation"."id",
    "delegation"."truster_id",
    "delegation"."trustee_id",
    "delegation"."scope"
  FROM "unit"
  JOIN "delegation"
    ON "delegation"."unit_id" = "unit"."id"
  JOIN "member"
    ON "delegation"."truster_id" = "member"."id"
  JOIN "privilege"
    ON "delegation"."unit_id" = "privilege"."unit_id"
    AND "delegation"."truster_id" = "privilege"."member_id"
  WHERE "member"."active" AND "privilege"."voting_right";

COMMENT ON VIEW "unit_delegation" IS 'Unit delegations where trusters are active and have voting right';


CREATE VIEW "area_delegation" AS
  SELECT DISTINCT ON ("area"."id", "delegation"."truster_id")
    "area"."id" AS "area_id",
    "delegation"."id",
    "delegation"."truster_id",
    "delegation"."trustee_id",
    "delegation"."scope"
  FROM "area"
  JOIN "delegation"
    ON "delegation"."unit_id" = "area"."unit_id"
    OR "delegation"."area_id" = "area"."id"
  JOIN "member"
    ON "delegation"."truster_id" = "member"."id"
  JOIN "privilege"
    ON "area"."unit_id" = "privilege"."unit_id"
    AND "delegation"."truster_id" = "privilege"."member_id"
  WHERE "member"."active" AND "privilege"."voting_right"
  ORDER BY
    "area"."id",
    "delegation"."truster_id",
    "delegation"."scope" DESC;

COMMENT ON VIEW "area_delegation" IS 'Area delegations where trusters are active and have voting right';


CREATE VIEW "issue_delegation" AS
  SELECT DISTINCT ON ("issue"."id", "delegation"."truster_id")
    "issue"."id" AS "issue_id",
    "delegation"."id",
    "delegation"."truster_id",
    "delegation"."trustee_id",
    "delegation"."scope"
  FROM "issue"
  JOIN "area"
    ON "area"."id" = "issue"."area_id"
  JOIN "delegation"
    ON "delegation"."unit_id" = "area"."unit_id"
    OR "delegation"."area_id" = "area"."id"
    OR "delegation"."issue_id" = "issue"."id"
  JOIN "member"
    ON "delegation"."truster_id" = "member"."id"
  JOIN "privilege"
    ON "area"."unit_id" = "privilege"."unit_id"
    AND "delegation"."truster_id" = "privilege"."member_id"
  WHERE "member"."active" AND "privilege"."voting_right"
  ORDER BY
    "issue"."id",
    "delegation"."truster_id",
    "delegation"."scope" DESC;

COMMENT ON VIEW "issue_delegation" IS 'Issue delegations where trusters are active and have voting right';


CREATE FUNCTION "membership_weight_with_skipping"
  ( "area_id_p"         "area"."id"%TYPE,
    "member_id_p"       "member"."id"%TYPE,
    "skip_member_ids_p" INT4[] )  -- "member"."id"%TYPE[]
  RETURNS INT4
  LANGUAGE 'plpgsql' STABLE AS $$
    DECLARE
      "sum_v"          INT4;
      "delegation_row" "area_delegation"%ROWTYPE;
    BEGIN
      "sum_v" := 1;
      FOR "delegation_row" IN
        SELECT "area_delegation".*
        FROM "area_delegation" LEFT JOIN "membership"
        ON "membership"."area_id" = "area_id_p"
        AND "membership"."member_id" = "area_delegation"."truster_id"
        WHERE "area_delegation"."area_id" = "area_id_p"
        AND "area_delegation"."trustee_id" = "member_id_p"
        AND "membership"."member_id" ISNULL
      LOOP
        IF NOT
          "skip_member_ids_p" @> ARRAY["delegation_row"."truster_id"]
        THEN
          "sum_v" := "sum_v" + "membership_weight_with_skipping"(
            "area_id_p",
            "delegation_row"."truster_id",
            "skip_member_ids_p" || "delegation_row"."truster_id"
          );
        END IF;
      END LOOP;
      RETURN "sum_v";
    END;
  $$;

COMMENT ON FUNCTION "membership_weight_with_skipping"
  ( "area"."id"%TYPE,
    "member"."id"%TYPE,
    INT4[] )
  IS 'Helper function for "membership_weight" function';


CREATE FUNCTION "membership_weight"
  ( "area_id_p"         "area"."id"%TYPE,
    "member_id_p"       "member"."id"%TYPE )  -- "member"."id"%TYPE[]
  RETURNS INT4
  LANGUAGE 'plpgsql' STABLE AS $$
    BEGIN
      RETURN "membership_weight_with_skipping"(
        "area_id_p",
        "member_id_p",
        ARRAY["member_id_p"]
      );
    END;
  $$;

COMMENT ON FUNCTION "membership_weight"
  ( "area"."id"%TYPE,
    "member"."id"%TYPE )
  IS 'Calculates the potential voting weight of a member in a given area';


CREATE VIEW "member_count_view" AS
  SELECT count(1) AS "total_count" FROM "member" WHERE "active";

COMMENT ON VIEW "member_count_view" IS 'View used to update "member_count" table';


CREATE VIEW "unit_member_count" AS
  SELECT
    "unit"."id" AS "unit_id",
    count("member"."id") AS "member_count"
  FROM "unit"
  LEFT JOIN "privilege"
  ON "privilege"."unit_id" = "unit"."id" 
  AND "privilege"."voting_right"
  LEFT JOIN "member"
  ON "member"."id" = "privilege"."member_id"
  AND "member"."active"
  GROUP BY "unit"."id";

COMMENT ON VIEW "unit_member_count" IS 'View used to update "member_count" column of "unit" table';


CREATE VIEW "area_member_count" AS
  SELECT
    "area"."id" AS "area_id",
    count("member"."id") AS "direct_member_count",
    coalesce(
      sum(
        CASE WHEN "member"."id" NOTNULL THEN
          "membership_weight"("area"."id", "member"."id")
        ELSE 0 END
      )
    ) AS "member_weight"
  FROM "area"
  LEFT JOIN "membership"
  ON "area"."id" = "membership"."area_id"
  LEFT JOIN "privilege"
  ON "privilege"."unit_id" = "area"."unit_id"
  AND "privilege"."member_id" = "membership"."member_id"
  AND "privilege"."voting_right"
  LEFT JOIN "member"
  ON "member"."id" = "privilege"."member_id"  -- NOTE: no membership here!
  AND "member"."active"
  GROUP BY "area"."id";

COMMENT ON VIEW "area_member_count" IS 'View used to update "direct_member_count" and "member_weight" columns of table "area"';


CREATE VIEW "opening_draft" AS
  SELECT "draft".* FROM (
    SELECT
      "initiative"."id" AS "initiative_id",
      min("draft"."id") AS "draft_id"
    FROM "initiative" JOIN "draft"
    ON "initiative"."id" = "draft"."initiative_id"
    GROUP BY "initiative"."id"
  ) AS "subquery"
  JOIN "draft" ON "subquery"."draft_id" = "draft"."id";

COMMENT ON VIEW "opening_draft" IS 'First drafts of all initiatives';


CREATE VIEW "current_draft" AS
  SELECT "draft".* FROM (
    SELECT
      "initiative"."id" AS "initiative_id",
      max("draft"."id") AS "draft_id"
    FROM "initiative" JOIN "draft"
    ON "initiative"."id" = "draft"."initiative_id"
    GROUP BY "initiative"."id"
  ) AS "subquery"
  JOIN "draft" ON "subquery"."draft_id" = "draft"."id";

COMMENT ON VIEW "current_draft" IS 'All latest drafts for each initiative';


CREATE VIEW "critical_opinion" AS
  SELECT * FROM "opinion"
  WHERE ("degree" = 2 AND "fulfilled" = FALSE)
  OR ("degree" = -2 AND "fulfilled" = TRUE);

COMMENT ON VIEW "critical_opinion" IS 'Opinions currently causing dissatisfaction';


CREATE VIEW "issue_supporter_in_admission_state" AS
  SELECT DISTINCT  -- TODO: DISTINCT needed?
    "area"."unit_id",
    "issue"."area_id",
    "issue"."id" AS "issue_id",
    "supporter"."member_id",
    "direct_interest_snapshot"."weight"
  FROM "issue"
  JOIN "area" ON "area"."id" = "issue"."area_id"
  JOIN "supporter" ON "supporter"."issue_id" = "issue"."id"
  JOIN "direct_interest_snapshot"
    ON  "direct_interest_snapshot"."issue_id" = "issue"."id"
    AND "direct_interest_snapshot"."event" = "issue"."latest_snapshot_event"
    AND "direct_interest_snapshot"."member_id" = "supporter"."member_id"
  WHERE "issue"."state" = 'admission'::"issue_state";

COMMENT ON VIEW "issue_supporter_in_admission_state" IS 'Helper view for "lf_update_issue_order" to allow a (proportional) ordering of issues within an area';


CREATE VIEW "initiative_suggestion_order_calculation" AS
  SELECT
    "initiative"."id" AS "initiative_id",
    ("issue"."closed" NOTNULL OR "issue"."fully_frozen" NOTNULL) AS "final"
  FROM "initiative" JOIN "issue"
  ON "initiative"."issue_id" = "issue"."id"
  WHERE ("issue"."closed" ISNULL AND "issue"."fully_frozen" ISNULL)
  OR ("initiative"."final_suggestion_order_calculated" = FALSE);

COMMENT ON VIEW "initiative_suggestion_order_calculation" IS 'Initiatives, where the "proportional_order" of its suggestions has to be calculated';

COMMENT ON COLUMN "initiative_suggestion_order_calculation"."final" IS 'Set to TRUE, if the issue is fully frozen or closed, and the calculation has to be done only once for one last time';


CREATE VIEW "individual_suggestion_ranking" AS
  SELECT
    "opinion"."initiative_id",
    "opinion"."member_id",
    "direct_interest_snapshot"."weight",
    CASE WHEN
      ("opinion"."degree" = 2 AND "opinion"."fulfilled" = FALSE) OR
      ("opinion"."degree" = -2 AND "opinion"."fulfilled" = TRUE)
    THEN 1 ELSE
      CASE WHEN
        ("opinion"."degree" = 1 AND "opinion"."fulfilled" = FALSE) OR
        ("opinion"."degree" = -1 AND "opinion"."fulfilled" = TRUE)
      THEN 2 ELSE
        CASE WHEN
          ("opinion"."degree" = 2 AND "opinion"."fulfilled" = TRUE) OR
          ("opinion"."degree" = -2 AND "opinion"."fulfilled" = FALSE)
        THEN 3 ELSE 4 END
      END
    END AS "preference",
    "opinion"."suggestion_id"
  FROM "opinion"
  JOIN "initiative" ON "initiative"."id" = "opinion"."initiative_id"
  JOIN "issue" ON "issue"."id" = "initiative"."issue_id"
  JOIN "direct_interest_snapshot"
    ON  "direct_interest_snapshot"."issue_id" = "issue"."id"
    AND "direct_interest_snapshot"."event" = "issue"."latest_snapshot_event"
    AND "direct_interest_snapshot"."member_id" = "opinion"."member_id";

COMMENT ON VIEW "individual_suggestion_ranking" IS 'Helper view for "lf_update_suggestion_order" to allow a proportional ordering of suggestions within an initiative';


CREATE VIEW "battle_participant" AS
    SELECT "initiative"."id", "initiative"."issue_id"
    FROM "issue" JOIN "initiative"
    ON "issue"."id" = "initiative"."issue_id"
    WHERE "initiative"."admitted"
  UNION ALL
    SELECT NULL, "id" AS "issue_id"
    FROM "issue";

COMMENT ON VIEW "battle_participant" IS 'Helper view for "battle_view" containing admitted initiatives plus virtual "status-quo" initiative denoted by NULL reference';


CREATE VIEW "battle_view" AS
  SELECT
    "issue"."id" AS "issue_id",
    "winning_initiative"."id" AS "winning_initiative_id",
    "losing_initiative"."id" AS "losing_initiative_id",
    sum(
      CASE WHEN
        coalesce("better_vote"."grade", 0) >
        coalesce("worse_vote"."grade", 0)
      THEN "direct_voter"."weight" ELSE 0 END
    ) AS "count"
  FROM "issue"
  LEFT JOIN "direct_voter"
  ON "issue"."id" = "direct_voter"."issue_id"
  JOIN "battle_participant" AS "winning_initiative"
    ON "issue"."id" = "winning_initiative"."issue_id"
  JOIN "battle_participant" AS "losing_initiative"
    ON "issue"."id" = "losing_initiative"."issue_id"
  LEFT JOIN "vote" AS "better_vote"
    ON "direct_voter"."member_id" = "better_vote"."member_id"
    AND "winning_initiative"."id" = "better_vote"."initiative_id"
  LEFT JOIN "vote" AS "worse_vote"
    ON "direct_voter"."member_id" = "worse_vote"."member_id"
    AND "losing_initiative"."id" = "worse_vote"."initiative_id"
  WHERE "issue"."state" = 'voting'
  AND "issue"."phase_finished" NOTNULL
  AND (
    "winning_initiative"."id" != "losing_initiative"."id" OR
    ( ("winning_initiative"."id" NOTNULL AND "losing_initiative"."id" ISNULL) OR
      ("winning_initiative"."id" ISNULL AND "losing_initiative"."id" NOTNULL) ) )
  GROUP BY
    "issue"."id",
    "winning_initiative"."id",
    "losing_initiative"."id";

COMMENT ON VIEW "battle_view" IS 'Number of members preferring one initiative (or status-quo) to another initiative (or status-quo); Used to fill "battle" table';


CREATE VIEW "expired_session" AS
  SELECT * FROM "session" WHERE now() > "expiry";

CREATE RULE "delete" AS ON DELETE TO "expired_session" DO INSTEAD
  DELETE FROM "session" WHERE "ident" = OLD."ident";

COMMENT ON VIEW "expired_session" IS 'View containing all expired sessions where DELETE is possible';
COMMENT ON RULE "delete" ON "expired_session" IS 'Rule allowing DELETE on rows in "expired_session" view, i.e. DELETE FROM "expired_session"';


CREATE VIEW "open_issue" AS
  SELECT * FROM "issue" WHERE "closed" ISNULL;

COMMENT ON VIEW "open_issue" IS 'All open issues';


CREATE VIEW "member_contingent" AS
  SELECT
    "member"."id" AS "member_id",
    "contingent"."polling",
    "contingent"."time_frame",
    CASE WHEN "contingent"."text_entry_limit" NOTNULL THEN
      (
        SELECT count(1) FROM "draft"
        JOIN "initiative" ON "initiative"."id" = "draft"."initiative_id"
        WHERE "draft"."author_id" = "member"."id"
        AND "initiative"."polling" = "contingent"."polling"
        AND "draft"."created" > now() - "contingent"."time_frame"
      ) + (
        SELECT count(1) FROM "suggestion"
        JOIN "initiative" ON "initiative"."id" = "suggestion"."initiative_id"
        WHERE "suggestion"."author_id" = "member"."id"
        AND "contingent"."polling" = FALSE
        AND "suggestion"."created" > now() - "contingent"."time_frame"
      )
    ELSE NULL END AS "text_entry_count",
    "contingent"."text_entry_limit",
    CASE WHEN "contingent"."initiative_limit" NOTNULL THEN (
      SELECT count(1) FROM "opening_draft" AS "draft"
        JOIN "initiative" ON "initiative"."id" = "draft"."initiative_id"
      WHERE "draft"."author_id" = "member"."id"
      AND "initiative"."polling" = "contingent"."polling"
      AND "draft"."created" > now() - "contingent"."time_frame"
    ) ELSE NULL END AS "initiative_count",
    "contingent"."initiative_limit"
  FROM "member" CROSS JOIN "contingent";

COMMENT ON VIEW "member_contingent" IS 'Actual counts of text entries and initiatives are calculated per member for each limit in the "contingent" table.';

COMMENT ON COLUMN "member_contingent"."text_entry_count" IS 'Only calculated when "text_entry_limit" is not null in the same row';
COMMENT ON COLUMN "member_contingent"."initiative_count" IS 'Only calculated when "initiative_limit" is not null in the same row';


CREATE VIEW "member_contingent_left" AS
  SELECT
    "member_id",
    "polling",
    max("text_entry_limit" - "text_entry_count") AS "text_entries_left",
    max("initiative_limit" - "initiative_count") AS "initiatives_left"
  FROM "member_contingent" GROUP BY "member_id", "polling";

COMMENT ON VIEW "member_contingent_left" IS 'Amount of text entries or initiatives which can be posted now instantly by a member. This view should be used by a frontend to determine, if the contingent for posting is exhausted.';


CREATE VIEW "event_for_notification" AS
  SELECT
    "member"."id" AS "recipient_id",
    "event".*
  FROM "member" CROSS JOIN "event"
  JOIN "issue" ON "issue"."id" = "event"."issue_id"
  JOIN "area" ON "area"."id" = "issue"."area_id"
  LEFT JOIN "privilege" ON
    "privilege"."member_id" = "member"."id" AND
    "privilege"."unit_id" = "area"."unit_id" AND
    "privilege"."voting_right" = TRUE
  LEFT JOIN "subscription" ON
    "subscription"."member_id" = "member"."id" AND
    "subscription"."unit_id" = "area"."unit_id"
  LEFT JOIN "ignored_area" ON
    "ignored_area"."member_id" = "member"."id" AND
    "ignored_area"."area_id" = "issue"."area_id"
  LEFT JOIN "interest" ON
    "interest"."member_id" = "member"."id" AND
    "interest"."issue_id" = "event"."issue_id"
  LEFT JOIN "supporter" ON
    "supporter"."member_id" = "member"."id" AND
    "supporter"."initiative_id" = "event"."initiative_id"
  WHERE ("privilege"."member_id" NOTNULL OR "subscription"."member_id" NOTNULL)
  AND ("ignored_area"."member_id" ISNULL OR "interest"."member_id" NOTNULL)
  AND (
    "event"."event" = 'issue_state_changed'::"event_type" OR
    ( "event"."event" = 'initiative_revoked'::"event_type" AND
      "supporter"."member_id" NOTNULL ) );

COMMENT ON VIEW "event_for_notification" IS 'Entries of the "event" table which are of interest for a particular notification mail recipient';

COMMENT ON COLUMN "event_for_notification"."recipient_id" IS 'member_id of the recipient of a notification mail';


CREATE VIEW "updated_initiative" AS
  SELECT
    "supporter"."member_id" AS "recipient_id",
    FALSE AS "featured",
    "supporter"."initiative_id"
  FROM "supporter"
  JOIN "initiative" ON "supporter"."initiative_id" = "initiative"."id"
  JOIN "issue" ON "issue"."id" = "initiative"."issue_id"
  LEFT JOIN "notification_initiative_sent" AS "sent" ON
    "sent"."member_id" = "supporter"."member_id" AND
    "sent"."initiative_id" = "supporter"."initiative_id"
  LEFT JOIN "ignored_initiative" ON
    "ignored_initiative"."member_id" = "supporter"."member_id" AND
    "ignored_initiative"."initiative_id" = "supporter"."initiative_id"
  WHERE "issue"."state" IN ('admission', 'discussion')
  AND "initiative"."revoked" ISNULL
  AND "ignored_initiative"."member_id" ISNULL
  AND (
    EXISTS (
      SELECT NULL FROM "draft"
      LEFT JOIN "ignored_member" ON
        "ignored_member"."member_id" = "supporter"."member_id" AND
        "ignored_member"."other_member_id" = "draft"."author_id"
      WHERE "draft"."initiative_id" = "supporter"."initiative_id"
      AND "draft"."id" > "supporter"."draft_id"
      AND "ignored_member"."member_id" ISNULL
    ) OR EXISTS (
      SELECT NULL FROM "suggestion"
      LEFT JOIN "opinion" ON
        "opinion"."member_id" = "supporter"."member_id" AND
        "opinion"."suggestion_id" = "suggestion"."id"
      LEFT JOIN "ignored_member" ON
        "ignored_member"."member_id" = "supporter"."member_id" AND
        "ignored_member"."other_member_id" = "suggestion"."author_id"
      WHERE "suggestion"."initiative_id" = "supporter"."initiative_id"
      AND "opinion"."member_id" ISNULL
      AND COALESCE("suggestion"."id" > "sent"."last_suggestion_id", TRUE)
      AND "ignored_member"."member_id" ISNULL
    )
  );

COMMENT ON VIEW "updated_initiative" IS 'Helper view for view "updated_or_featured_initiative"';


CREATE FUNCTION "featured_initiative"
  ( "recipient_id_p" "member"."id"%TYPE,
    "area_id_p"      "area"."id"%TYPE )
  RETURNS SETOF "initiative"."id"%TYPE
  LANGUAGE 'plpgsql' STABLE AS $$
    DECLARE
      "counter_v"         "member"."notification_counter"%TYPE;
      "sample_size_v"     "member"."notification_sample_size"%TYPE;
      "initiative_id_ary" INT4[];  --"initiative"."id"%TYPE[]
      "match_v"           BOOLEAN;
      "member_id_v"       "member"."id"%TYPE;
      "seed_v"            TEXT;
      "initiative_id_v"   "initiative"."id"%TYPE;
    BEGIN
      SELECT "notification_counter", "notification_sample_size"
        INTO "counter_v", "sample_size_v"
        FROM "member" WHERE "id" = "recipient_id_p";
      IF COALESCE("sample_size_v" <= 0, TRUE) THEN
        RETURN;
      END IF;
      "initiative_id_ary" := '{}';
      LOOP
        "match_v" := FALSE;
        FOR "member_id_v", "seed_v" IN
          SELECT * FROM (
            SELECT DISTINCT
              "supporter"."member_id",
              md5(
                "recipient_id_p" || '-' ||
                "counter_v"      || '-' ||
                "area_id_p"      || '-' ||
                "supporter"."member_id"
              ) AS "seed"
            FROM "supporter"
            JOIN "initiative" ON "initiative"."id" = "supporter"."initiative_id"
            JOIN "issue" ON "issue"."id" = "initiative"."issue_id"
            WHERE "supporter"."member_id" != "recipient_id_p"
            AND "issue"."area_id" = "area_id_p"
            AND "issue"."state" IN ('admission', 'discussion', 'verification')
          ) AS "subquery"
          ORDER BY "seed"
        LOOP
          SELECT "initiative"."id" INTO "initiative_id_v"
            FROM "initiative"
            JOIN "issue" ON "issue"."id" = "initiative"."issue_id"
            JOIN "area" ON "area"."id" = "issue"."area_id"
            JOIN "supporter" ON "supporter"."initiative_id" = "initiative"."id"
            LEFT JOIN "supporter" AS "self_support" ON
              "self_support"."initiative_id" = "initiative"."id" AND
              "self_support"."member_id" = "recipient_id_p"
            LEFT JOIN "privilege" ON
              "privilege"."member_id" = "recipient_id_p" AND
              "privilege"."unit_id" = "area"."unit_id" AND
              "privilege"."voting_right" = TRUE
            LEFT JOIN "subscription" ON
              "subscription"."member_id" = "recipient_id_p" AND
              "subscription"."unit_id" = "area"."unit_id"
            LEFT JOIN "ignored_initiative" ON
              "ignored_initiative"."member_id" = "recipient_id_p" AND
              "ignored_initiative"."initiative_id" = "initiative"."id"
            WHERE "supporter"."member_id" = "member_id_v"
            AND "issue"."area_id" = "area_id_p"
            AND "issue"."state" IN ('admission', 'discussion', 'verification')
            AND "initiative"."revoked" ISNULL
            AND "self_support"."member_id" ISNULL
            AND NOT "initiative_id_ary" @> ARRAY["initiative"."id"]
            AND (
              "privilege"."member_id" NOTNULL OR
              "subscription"."member_id" NOTNULL )
            AND "ignored_initiative"."member_id" ISNULL
            AND NOT EXISTS (
              SELECT NULL FROM "draft"
              JOIN "ignored_member" ON
                "ignored_member"."member_id" = "recipient_id_p" AND
                "ignored_member"."other_member_id" = "draft"."author_id"
              WHERE "draft"."initiative_id" = "initiative"."id"
            )
            ORDER BY md5("seed_v" || '-' || "initiative"."id")
            LIMIT 1;
          IF FOUND THEN
            "match_v" := TRUE;
            RETURN NEXT "initiative_id_v";
            IF array_length("initiative_id_ary", 1) + 1 >= "sample_size_v" THEN
              RETURN;
            END IF;
            "initiative_id_ary" := "initiative_id_ary" || "initiative_id_v";
          END IF;
        END LOOP;
        EXIT WHEN NOT "match_v";
      END LOOP;
      RETURN;
    END;
  $$;

COMMENT ON FUNCTION "featured_initiative"
  ( "recipient_id_p" "member"."id"%TYPE,
    "area_id_p"      "area"."id"%TYPE )
  IS 'Helper function for view "updated_or_featured_initiative"';


CREATE VIEW "updated_or_featured_initiative" AS
  SELECT
    "subquery".*,
    NOT EXISTS (
      SELECT NULL FROM "initiative" AS "better_initiative"
      WHERE "better_initiative"."issue_id" = "initiative"."issue_id"
      AND
        ( COALESCE("better_initiative"."supporter_count", -1),
          -"better_initiative"."id" ) >
        ( COALESCE("initiative"."supporter_count", -1),
          -"initiative"."id" )
    ) AS "leading"
  FROM (
    SELECT * FROM "updated_initiative"
    UNION ALL
    SELECT
      "member"."id" AS "recipient_id",
      TRUE AS "featured",
      "featured_initiative_id" AS "initiative_id"
    FROM "member" CROSS JOIN "area"
    CROSS JOIN LATERAL
      "featured_initiative"("member"."id", "area"."id") AS "featured_initiative_id"
    JOIN "initiative" ON "initiative"."id" = "featured_initiative_id"
  ) AS "subquery"
  JOIN "initiative" ON "initiative"."id" = "subquery"."initiative_id";

COMMENT ON VIEW "updated_or_featured_initiative" IS 'Initiatives to be included in a scheduled notification mail because (a) they have been updated or (b) they are featured';

COMMENT ON COLUMN "updated_or_featured_initiative"."recipient_id" IS '"id" of the member who receives the notification mail';
COMMENT ON COLUMN "updated_or_featured_initiative"."featured" IS 'TRUE if the initiative has been included because it was selected by the "featured_initiative" algorithm (see source of function "featured_initiative")';
COMMENT ON COLUMN "updated_or_featured_initiative"."initiative_id" IS '"id" of the initiative to be included in the notification mail';
COMMENT ON COLUMN "updated_or_featured_initiative"."leading" IS 'TRUE if the initiative has the highest "supporter_count" in the issue';


CREATE VIEW "leading_complement_initiative" AS
  SELECT * FROM (
    SELECT DISTINCT ON ("uf_initiative"."recipient_id", "initiative"."issue_id")
      "uf_initiative"."recipient_id",
      FALSE AS "featured",
      "uf_initiative"."initiative_id",
      TRUE AS "leading"
    FROM "updated_or_featured_initiative" AS "uf_initiative"
    JOIN "initiative" AS "uf_initiative_full" ON
      "uf_initiative_full"."id" = "uf_initiative"."initiative_id"
    JOIN "initiative" ON
      "initiative"."issue_id" = "uf_initiative_full"."issue_id"
    WHERE "initiative"."revoked" ISNULL
    ORDER BY
      "uf_initiative"."recipient_id",
      "initiative"."issue_id",
      "initiative"."supporter_count" DESC,
      "initiative"."id"
  ) AS "subquery"
  WHERE NOT EXISTS (
    SELECT NULL FROM "updated_or_featured_initiative" AS "other"
    WHERE "other"."recipient_id" = "subquery"."recipient_id"
    AND "other"."initiative_id" = "subquery"."initiative_id"
  );

COMMENT ON VIEW "leading_complement_initiative" IS 'Helper view for view "unfiltered_initiative_for_notification" in order to always include the most supported initiative of an issue';
COMMENT ON COLUMN "leading_complement_initiative"."featured" IS 'Always FALSE in this view';
COMMENT ON COLUMN "leading_complement_initiative"."initiative_id" IS '"id" of the initiative to be included in the notification mail';
COMMENT ON COLUMN "leading_complement_initiative"."leading" IS 'Always TRUE in this view';


CREATE VIEW "unfiltered_initiative_for_notification" AS
  SELECT
    "subquery".*,
    "supporter"."member_id" NOTNULL AS "supported",
    CASE WHEN "supporter"."member_id" NOTNULL THEN
      EXISTS (
        SELECT NULL FROM "draft"
        WHERE "draft"."initiative_id" = "subquery"."initiative_id"
        AND "draft"."id" > "supporter"."draft_id"
      )
    ELSE
      EXISTS (
        SELECT NULL FROM "draft"
        WHERE "draft"."initiative_id" = "subquery"."initiative_id"
        AND COALESCE("draft"."id" > "sent"."last_draft_id", TRUE)
      )
    END AS "new_draft",
    CASE WHEN "supporter"."member_id" NOTNULL THEN
      ( SELECT count(1) FROM "suggestion"
        LEFT JOIN "opinion" ON
          "opinion"."member_id" = "supporter"."member_id" AND
          "opinion"."suggestion_id" = "suggestion"."id"
        WHERE "suggestion"."initiative_id" = "subquery"."initiative_id"
        AND "opinion"."member_id" ISNULL
        AND COALESCE("suggestion"."id" > "sent"."last_suggestion_id", TRUE)
      )
    ELSE
      ( SELECT count(1) FROM "suggestion"
        WHERE "suggestion"."initiative_id" = "subquery"."initiative_id"
        AND COALESCE("suggestion"."id" > "sent"."last_suggestion_id", TRUE)
      )
    END AS "new_suggestion_count"
  FROM (
    SELECT * FROM "updated_or_featured_initiative"
    UNION ALL
    SELECT * FROM "leading_complement_initiative"
  ) AS "subquery"
  LEFT JOIN "supporter" ON
    "supporter"."member_id" = "subquery"."recipient_id" AND
    "supporter"."initiative_id" = "subquery"."initiative_id"
  LEFT JOIN "notification_initiative_sent" AS "sent" ON
    "sent"."member_id" = "subquery"."recipient_id" AND
    "sent"."initiative_id" = "subquery"."initiative_id";

COMMENT ON VIEW "unfiltered_initiative_for_notification" IS 'Helper view which simply combines the views "updated_or_featured_initiative" and "leading_complement_initiative" and adds columns "supported", "new_draft", and "new_suggestion_count';

COMMENT ON COLUMN "unfiltered_initiative_for_notification"."supported"            IS 'TRUE if initiative is supported by the recipient';
COMMENT ON COLUMN "unfiltered_initiative_for_notification"."new_draft"            IS 'TRUE if a new draft exists (using the "draft_id" column of the "supporter" table in case of "supported" initiatives and the "last_draft_id" column of the "notification_initiative_sent" table in all other cases)';
COMMENT ON COLUMN "unfiltered_initiative_for_notification"."new_suggestion_count" IS 'Number of new suggestions (using the "last_suggestion_id" column of the "notification_initiative_sent" table while ignoring suggestions with an "opinion")';


CREATE VIEW "initiative_for_notification" AS
  SELECT "unfiltered1".*
  FROM "unfiltered_initiative_for_notification" "unfiltered1"
  JOIN "initiative" AS "initiative1" ON
    "initiative1"."id" = "unfiltered1"."initiative_id"
  JOIN "issue" AS "issue1" ON "issue1"."id" = "initiative1"."issue_id"
  WHERE EXISTS (
    SELECT NULL
    FROM "unfiltered_initiative_for_notification" "unfiltered2"
    JOIN "initiative" AS "initiative2" ON
      "initiative2"."id" = "unfiltered2"."initiative_id"
    JOIN "issue" AS "issue2" ON "issue2"."id" = "initiative2"."issue_id"
    WHERE "unfiltered1"."recipient_id" = "unfiltered2"."recipient_id"
    AND "issue1"."area_id" = "issue2"."area_id"
    AND ("unfiltered2"."new_draft" OR "unfiltered2"."new_suggestion_count" > 0 )
  );

COMMENT ON VIEW "initiative_for_notification" IS 'Initiatives to be included in a scheduled notification mail';

COMMENT ON COLUMN "initiative_for_notification"."recipient_id"         IS '"id" of the member who receives the notification mail';
COMMENT ON COLUMN "initiative_for_notification"."featured"             IS 'TRUE if the initiative has been included because it was selected by the "featured_initiative" algorithm (see source of function "featured_initiative")';
COMMENT ON COLUMN "initiative_for_notification"."initiative_id"        IS '"id" of the initiative to be included in the notification mail';
COMMENT ON COLUMN "initiative_for_notification"."leading"              IS 'TRUE if the initiative has the highest "supporter_count" in the issue';
COMMENT ON COLUMN "initiative_for_notification"."supported"            IS 'TRUE if initiative is supported by the recipient';
COMMENT ON COLUMN "initiative_for_notification"."new_draft"            IS 'TRUE if a new draft exists (using the "draft_id" column of the "supporter" table in case of "supported" initiatives and the "last_draft_id" column of the "notification_initiative_sent" table in all other cases)';
COMMENT ON COLUMN "initiative_for_notification"."new_suggestion_count" IS 'Number of new suggestions (using the "last_suggestion_id" column of the "notification_initiative_sent" table while ignoring suggestions with an "opinion")';


CREATE VIEW "scheduled_notification_to_send" AS
  SELECT * FROM (
    SELECT
      "id" AS "recipient_id",
      now() - CASE WHEN "notification_dow" ISNULL THEN
        ( "notification_sent"::DATE + CASE
          WHEN EXTRACT(HOUR FROM "notification_sent") < "notification_hour"
          THEN 0 ELSE 1 END
        )::TIMESTAMP + '1 hour'::INTERVAL * "notification_hour"
      ELSE
        ( "notification_sent"::DATE +
          ( 7 + "notification_dow" -
            EXTRACT(DOW FROM
              ( "notification_sent"::DATE + CASE
                WHEN EXTRACT(HOUR FROM "notification_sent") < "notification_hour"
                THEN 0 ELSE 1 END
              )::TIMESTAMP + '1 hour'::INTERVAL * "notification_hour"
            )::INTEGER
          ) % 7 +
          CASE
            WHEN EXTRACT(HOUR FROM "notification_sent") < "notification_hour"
            THEN 0 ELSE 1
          END
        )::TIMESTAMP + '1 hour'::INTERVAL * "notification_hour"
      END AS "pending"
    FROM (
      SELECT
        "id",
        COALESCE("notification_sent", "activated") AS "notification_sent",
        "notification_dow",
        "notification_hour"
      FROM "member_to_notify"
      WHERE "notification_hour" NOTNULL
    ) AS "subquery1"
  ) AS "subquery2"
  WHERE "pending" > '0'::INTERVAL;

COMMENT ON VIEW "scheduled_notification_to_send" IS 'Set of members where a scheduled notification mail is pending';

COMMENT ON COLUMN "scheduled_notification_to_send"."recipient_id" IS '"id" of the member who needs to receive a notification mail';
COMMENT ON COLUMN "scheduled_notification_to_send"."pending"      IS 'Duration for which the notification mail has already been pending';


CREATE VIEW "newsletter_to_send" AS
  SELECT
    "member"."id" AS "recipient_id",
    "newsletter"."id" AS "newsletter_id",
    "newsletter"."published"
  FROM "newsletter" CROSS JOIN "member_eligible_to_be_notified" AS "member"
  LEFT JOIN "privilege" ON
    "privilege"."member_id" = "member"."id" AND
    "privilege"."unit_id" = "newsletter"."unit_id" AND
    "privilege"."voting_right" = TRUE
  LEFT JOIN "subscription" ON
    "subscription"."member_id" = "member"."id" AND
    "subscription"."unit_id" = "newsletter"."unit_id"
  WHERE "newsletter"."published" <= now()
  AND "newsletter"."sent" ISNULL
  AND (
    "member"."disable_notifications" = FALSE OR
    "newsletter"."include_all_members" = TRUE )
  AND (
    "newsletter"."unit_id" ISNULL OR
    "privilege"."member_id" NOTNULL OR
    "subscription"."member_id" NOTNULL );

COMMENT ON VIEW "newsletter_to_send" IS 'List of "newsletter_id"s for each member that are due to be sent out';

COMMENT ON COLUMN "newsletter"."published" IS 'Timestamp when the newsletter was supposed to be sent out (can be used for ordering)';



------------------------------------------------------
-- Row set returning function for delegation chains --
------------------------------------------------------


CREATE TYPE "delegation_chain_loop_tag" AS ENUM
  ('first', 'intermediate', 'last', 'repetition');

COMMENT ON TYPE "delegation_chain_loop_tag" IS 'Type for loop tags in "delegation_chain_row" type';


CREATE TYPE "delegation_chain_row" AS (
        "index"                 INT4,
        "member_id"             INT4,
        "member_valid"          BOOLEAN,
        "participation"         BOOLEAN,
        "overridden"            BOOLEAN,
        "scope_in"              "delegation_scope",
        "scope_out"             "delegation_scope",
        "disabled_out"          BOOLEAN,
        "loop"                  "delegation_chain_loop_tag" );

COMMENT ON TYPE "delegation_chain_row" IS 'Type of rows returned by "delegation_chain" function';

COMMENT ON COLUMN "delegation_chain_row"."index"         IS 'Index starting with 0 and counting up';
COMMENT ON COLUMN "delegation_chain_row"."participation" IS 'In case of delegation chains for issues: interest, for areas: membership, for global delegation chains: always null';
COMMENT ON COLUMN "delegation_chain_row"."overridden"    IS 'True, if an entry with lower index has "participation" set to true';
COMMENT ON COLUMN "delegation_chain_row"."scope_in"      IS 'Scope of used incoming delegation';
COMMENT ON COLUMN "delegation_chain_row"."scope_out"     IS 'Scope of used outgoing delegation';
COMMENT ON COLUMN "delegation_chain_row"."disabled_out"  IS 'Outgoing delegation is explicitly disabled by a delegation with trustee_id set to NULL';
COMMENT ON COLUMN "delegation_chain_row"."loop"          IS 'Not null, if member is part of a loop, see "delegation_chain_loop_tag" type';


CREATE FUNCTION "delegation_chain_for_closed_issue"
  ( "member_id_p"           "member"."id"%TYPE,
    "issue_id_p"            "issue"."id"%TYPE )
  RETURNS SETOF "delegation_chain_row"
  LANGUAGE 'plpgsql' STABLE AS $$
    DECLARE
      "output_row"           "delegation_chain_row";
      "direct_voter_row"     "direct_voter"%ROWTYPE;
      "delegating_voter_row" "delegating_voter"%ROWTYPE;
    BEGIN
      "output_row"."index"         := 0;
      "output_row"."member_id"     := "member_id_p";
      "output_row"."member_valid"  := TRUE;
      "output_row"."participation" := FALSE;
      "output_row"."overridden"    := FALSE;
      "output_row"."disabled_out"  := FALSE;
      LOOP
        SELECT INTO "direct_voter_row" * FROM "direct_voter"
          WHERE "issue_id" = "issue_id_p"
          AND "member_id" = "output_row"."member_id";
        IF "direct_voter_row"."member_id" NOTNULL THEN
          "output_row"."participation" := TRUE;
          "output_row"."scope_out"     := NULL;
          "output_row"."disabled_out"  := NULL;
          RETURN NEXT "output_row";
          RETURN;
        END IF;
        SELECT INTO "delegating_voter_row" * FROM "delegating_voter"
          WHERE "issue_id" = "issue_id_p"
          AND "member_id" = "output_row"."member_id";
        IF "delegating_voter_row"."member_id" ISNULL THEN
          RETURN;
        END IF;
        "output_row"."scope_out" := "delegating_voter_row"."scope";
        RETURN NEXT "output_row";
        "output_row"."member_id" := "delegating_voter_row"."delegate_member_ids"[1];
        "output_row"."scope_in"  := "output_row"."scope_out";
      END LOOP;
    END;
  $$;

COMMENT ON FUNCTION "delegation_chain_for_closed_issue"
  ( "member"."id"%TYPE,
    "member"."id"%TYPE )
  IS 'Helper function for "delegation_chain" function, handling the special case of closed issues after voting';


CREATE FUNCTION "delegation_chain"
  ( "member_id_p"           "member"."id"%TYPE,
    "unit_id_p"             "unit"."id"%TYPE,
    "area_id_p"             "area"."id"%TYPE,
    "issue_id_p"            "issue"."id"%TYPE,
    "simulate_trustee_id_p" "member"."id"%TYPE DEFAULT NULL,
    "simulate_default_p"    BOOLEAN            DEFAULT FALSE )
  RETURNS SETOF "delegation_chain_row"
  LANGUAGE 'plpgsql' STABLE AS $$
    DECLARE
      "scope_v"            "delegation_scope";
      "unit_id_v"          "unit"."id"%TYPE;
      "area_id_v"          "area"."id"%TYPE;
      "issue_row"          "issue"%ROWTYPE;
      "visited_member_ids" INT4[];  -- "member"."id"%TYPE[]
      "loop_member_id_v"   "member"."id"%TYPE;
      "output_row"         "delegation_chain_row";
      "output_rows"        "delegation_chain_row"[];
      "simulate_v"         BOOLEAN;
      "simulate_here_v"    BOOLEAN;
      "delegation_row"     "delegation"%ROWTYPE;
      "row_count"          INT4;
      "i"                  INT4;
      "loop_v"             BOOLEAN;
    BEGIN
      IF "simulate_trustee_id_p" NOTNULL AND "simulate_default_p" THEN
        RAISE EXCEPTION 'Both "simulate_trustee_id_p" is set, and "simulate_default_p" is true';
      END IF;
      IF "simulate_trustee_id_p" NOTNULL OR "simulate_default_p" THEN
        "simulate_v" := TRUE;
      ELSE
        "simulate_v" := FALSE;
      END IF;
      IF
        "unit_id_p" NOTNULL AND
        "area_id_p" ISNULL AND
        "issue_id_p" ISNULL
      THEN
        "scope_v" := 'unit';
        "unit_id_v" := "unit_id_p";
      ELSIF
        "unit_id_p" ISNULL AND
        "area_id_p" NOTNULL AND
        "issue_id_p" ISNULL
      THEN
        "scope_v" := 'area';
        "area_id_v" := "area_id_p";
        SELECT "unit_id" INTO "unit_id_v"
          FROM "area" WHERE "id" = "area_id_v";
      ELSIF
        "unit_id_p" ISNULL AND
        "area_id_p" ISNULL AND
        "issue_id_p" NOTNULL
      THEN
        SELECT INTO "issue_row" * FROM "issue" WHERE "id" = "issue_id_p";
        IF "issue_row"."id" ISNULL THEN
          RETURN;
        END IF;
        IF "issue_row"."closed" NOTNULL THEN
          IF "simulate_v" THEN
            RAISE EXCEPTION 'Tried to simulate delegation chain for closed issue.';
          END IF;
          FOR "output_row" IN
            SELECT * FROM
            "delegation_chain_for_closed_issue"("member_id_p", "issue_id_p")
          LOOP
            RETURN NEXT "output_row";
          END LOOP;
          RETURN;
        END IF;
        "scope_v" := 'issue';
        SELECT "area_id" INTO "area_id_v"
          FROM "issue" WHERE "id" = "issue_id_p";
        SELECT "unit_id" INTO "unit_id_v"
          FROM "area"  WHERE "id" = "area_id_v";
      ELSE
        RAISE EXCEPTION 'Exactly one of unit_id_p, area_id_p, or issue_id_p must be NOTNULL.';
      END IF;
      "visited_member_ids" := '{}';
      "loop_member_id_v"   := NULL;
      "output_rows"        := '{}';
      "output_row"."index"         := 0;
      "output_row"."member_id"     := "member_id_p";
      "output_row"."member_valid"  := TRUE;
      "output_row"."participation" := FALSE;
      "output_row"."overridden"    := FALSE;
      "output_row"."disabled_out"  := FALSE;
      "output_row"."scope_out"     := NULL;
      LOOP
        IF "visited_member_ids" @> ARRAY["output_row"."member_id"] THEN
          "loop_member_id_v" := "output_row"."member_id";
        ELSE
          "visited_member_ids" :=
            "visited_member_ids" || "output_row"."member_id";
        END IF;
        IF "output_row"."participation" ISNULL THEN
          "output_row"."overridden" := NULL;
        ELSIF "output_row"."participation" THEN
          "output_row"."overridden" := TRUE;
        END IF;
        "output_row"."scope_in" := "output_row"."scope_out";
        "output_row"."member_valid" := EXISTS (
          SELECT NULL FROM "member" JOIN "privilege"
          ON "privilege"."member_id" = "member"."id"
          AND "privilege"."unit_id" = "unit_id_v"
          WHERE "id" = "output_row"."member_id"
          AND "member"."active" AND "privilege"."voting_right"
        );
        "simulate_here_v" := (
          "simulate_v" AND
          "output_row"."member_id" = "member_id_p"
        );
        "delegation_row" := ROW(NULL);
        IF "output_row"."member_valid" OR "simulate_here_v" THEN
          IF "scope_v" = 'unit' THEN
            IF NOT "simulate_here_v" THEN
              SELECT * INTO "delegation_row" FROM "delegation"
                WHERE "truster_id" = "output_row"."member_id"
                AND "unit_id" = "unit_id_v";
            END IF;
          ELSIF "scope_v" = 'area' THEN
            "output_row"."participation" := EXISTS (
              SELECT NULL FROM "membership"
              WHERE "area_id" = "area_id_p"
              AND "member_id" = "output_row"."member_id"
            );
            IF "simulate_here_v" THEN
              IF "simulate_trustee_id_p" ISNULL THEN
                SELECT * INTO "delegation_row" FROM "delegation"
                  WHERE "truster_id" = "output_row"."member_id"
                  AND "unit_id" = "unit_id_v";
              END IF;
            ELSE
              SELECT * INTO "delegation_row" FROM "delegation"
                WHERE "truster_id" = "output_row"."member_id"
                AND (
                  "unit_id" = "unit_id_v" OR
                  "area_id" = "area_id_v"
                )
                ORDER BY "scope" DESC;
            END IF;
          ELSIF "scope_v" = 'issue' THEN
            IF "issue_row"."fully_frozen" ISNULL THEN
              "output_row"."participation" := EXISTS (
                SELECT NULL FROM "interest"
                WHERE "issue_id" = "issue_id_p"
                AND "member_id" = "output_row"."member_id"
              );
            ELSE
              IF "output_row"."member_id" = "member_id_p" THEN
                "output_row"."participation" := EXISTS (
                  SELECT NULL FROM "direct_voter"
                  WHERE "issue_id" = "issue_id_p"
                  AND "member_id" = "output_row"."member_id"
                );
              ELSE
                "output_row"."participation" := NULL;
              END IF;
            END IF;
            IF "simulate_here_v" THEN
              IF "simulate_trustee_id_p" ISNULL THEN
                SELECT * INTO "delegation_row" FROM "delegation"
                  WHERE "truster_id" = "output_row"."member_id"
                  AND (
                    "unit_id" = "unit_id_v" OR
                    "area_id" = "area_id_v"
                  )
                  ORDER BY "scope" DESC;
              END IF;
            ELSE
              SELECT * INTO "delegation_row" FROM "delegation"
                WHERE "truster_id" = "output_row"."member_id"
                AND (
                  "unit_id" = "unit_id_v" OR
                  "area_id" = "area_id_v" OR
                  "issue_id" = "issue_id_p"
                )
                ORDER BY "scope" DESC;
            END IF;
          END IF;
        ELSE
          "output_row"."participation" := FALSE;
        END IF;
        IF "simulate_here_v" AND "simulate_trustee_id_p" NOTNULL THEN
          "output_row"."scope_out" := "scope_v";
          "output_rows" := "output_rows" || "output_row";
          "output_row"."member_id" := "simulate_trustee_id_p";
        ELSIF "delegation_row"."trustee_id" NOTNULL THEN
          "output_row"."scope_out" := "delegation_row"."scope";
          "output_rows" := "output_rows" || "output_row";
          "output_row"."member_id" := "delegation_row"."trustee_id";
        ELSIF "delegation_row"."scope" NOTNULL THEN
          "output_row"."scope_out" := "delegation_row"."scope";
          "output_row"."disabled_out" := TRUE;
          "output_rows" := "output_rows" || "output_row";
          EXIT;
        ELSE
          "output_row"."scope_out" := NULL;
          "output_rows" := "output_rows" || "output_row";
          EXIT;
        END IF;
        EXIT WHEN "loop_member_id_v" NOTNULL;
        "output_row"."index" := "output_row"."index" + 1;
      END LOOP;
      "row_count" := array_upper("output_rows", 1);
      "i"      := 1;
      "loop_v" := FALSE;
      LOOP
        "output_row" := "output_rows"["i"];
        EXIT WHEN "output_row" ISNULL;  -- NOTE: ISNULL and NOT ... NOTNULL produce different results!
        IF "loop_v" THEN
          IF "i" + 1 = "row_count" THEN
            "output_row"."loop" := 'last';
          ELSIF "i" = "row_count" THEN
            "output_row"."loop" := 'repetition';
          ELSE
            "output_row"."loop" := 'intermediate';
          END IF;
        ELSIF "output_row"."member_id" = "loop_member_id_v" THEN
          "output_row"."loop" := 'first';
          "loop_v" := TRUE;
        END IF;
        IF "scope_v" = 'unit' THEN
          "output_row"."participation" := NULL;
        END IF;
        RETURN NEXT "output_row";
        "i" := "i" + 1;
      END LOOP;
      RETURN;
    END;
  $$;

COMMENT ON FUNCTION "delegation_chain"
  ( "member"."id"%TYPE,
    "unit"."id"%TYPE,
    "area"."id"%TYPE,
    "issue"."id"%TYPE,
    "member"."id"%TYPE,
    BOOLEAN )
  IS 'Shows a delegation chain for unit, area, or issue; See "delegation_chain_row" type for more information';



---------------------------------------------------------
-- Single row returning function for delegation chains --
---------------------------------------------------------


CREATE TYPE "delegation_info_loop_type" AS ENUM
  ('own', 'first', 'first_ellipsis', 'other', 'other_ellipsis');

COMMENT ON TYPE "delegation_info_loop_type" IS 'Type of "delegation_loop" in "delegation_info_type"; ''own'' means loop to self, ''first'' means loop to first trustee, ''first_ellipsis'' means loop to ellipsis after first trustee, ''other'' means loop to other trustee, ''other_ellipsis'' means loop to ellipsis after other trustee''';


CREATE TYPE "delegation_info_type" AS (
        "own_participation"           BOOLEAN,
        "own_delegation_scope"        "delegation_scope",
        "first_trustee_id"            INT4,
        "first_trustee_participation" BOOLEAN,
        "first_trustee_ellipsis"      BOOLEAN,
        "other_trustee_id"            INT4,
        "other_trustee_participation" BOOLEAN,
        "other_trustee_ellipsis"      BOOLEAN,
        "delegation_loop"             "delegation_info_loop_type",
        "participating_member_id"     INT4 );

COMMENT ON TYPE "delegation_info_type" IS 'Type of result returned by "delegation_info" function; For meaning of "participation" check comment on "delegation_chain_row" type';

COMMENT ON COLUMN "delegation_info_type"."own_participation"           IS 'Member is directly participating';
COMMENT ON COLUMN "delegation_info_type"."own_delegation_scope"        IS 'Delegation scope of member';
COMMENT ON COLUMN "delegation_info_type"."first_trustee_id"            IS 'Direct trustee of member';
COMMENT ON COLUMN "delegation_info_type"."first_trustee_participation" IS 'Direct trustee of member is participating';
COMMENT ON COLUMN "delegation_info_type"."first_trustee_ellipsis"      IS 'Ellipsis in delegation chain after "first_trustee"';
COMMENT ON COLUMN "delegation_info_type"."other_trustee_id"            IS 'Another relevant trustee (due to participation)';
COMMENT ON COLUMN "delegation_info_type"."other_trustee_participation" IS 'Another trustee is participating (redundant field: if "other_trustee_id" is set, then "other_trustee_participation" is always TRUE, else "other_trustee_participation" is NULL)';
COMMENT ON COLUMN "delegation_info_type"."other_trustee_ellipsis"      IS 'Ellipsis in delegation chain after "other_trustee"';
COMMENT ON COLUMN "delegation_info_type"."delegation_loop"             IS 'Non-NULL value, if delegation chain contains a circle; See comment on "delegation_info_loop_type" for details';
COMMENT ON COLUMN "delegation_info_type"."participating_member_id"     IS 'First participating member in delegation chain';


CREATE FUNCTION "delegation_info"
  ( "member_id_p"           "member"."id"%TYPE,
    "unit_id_p"             "unit"."id"%TYPE,
    "area_id_p"             "area"."id"%TYPE,
    "issue_id_p"            "issue"."id"%TYPE,
    "simulate_trustee_id_p" "member"."id"%TYPE DEFAULT NULL,
    "simulate_default_p"    BOOLEAN            DEFAULT FALSE )
  RETURNS "delegation_info_type"
  LANGUAGE 'plpgsql' STABLE AS $$
    DECLARE
      "current_row" "delegation_chain_row";
      "result"      "delegation_info_type";
    BEGIN
      "result"."own_participation" := FALSE;
      FOR "current_row" IN
        SELECT * FROM "delegation_chain"(
          "member_id_p",
          "unit_id_p", "area_id_p", "issue_id_p",
          "simulate_trustee_id_p", "simulate_default_p")
      LOOP
        IF
          "result"."participating_member_id" ISNULL AND
          "current_row"."participation"
        THEN
          "result"."participating_member_id" := "current_row"."member_id";
        END IF;
        IF "current_row"."member_id" = "member_id_p" THEN
          "result"."own_participation"    := "current_row"."participation";
          "result"."own_delegation_scope" := "current_row"."scope_out";
          IF "current_row"."loop" = 'first' THEN
            "result"."delegation_loop" := 'own';
          END IF;
        ELSIF
          "current_row"."member_valid" AND
          ( "current_row"."loop" ISNULL OR
            "current_row"."loop" != 'repetition' )
        THEN
          IF "result"."first_trustee_id" ISNULL THEN
            "result"."first_trustee_id"            := "current_row"."member_id";
            "result"."first_trustee_participation" := "current_row"."participation";
            "result"."first_trustee_ellipsis"      := FALSE;
            IF "current_row"."loop" = 'first' THEN
              "result"."delegation_loop" := 'first';
            END IF;
          ELSIF "result"."other_trustee_id" ISNULL THEN
            IF "current_row"."participation" AND NOT "current_row"."overridden" THEN
              "result"."other_trustee_id"            := "current_row"."member_id";
              "result"."other_trustee_participation" := TRUE;
              "result"."other_trustee_ellipsis"      := FALSE;
              IF "current_row"."loop" = 'first' THEN
                "result"."delegation_loop" := 'other';
              END IF;
            ELSE
              "result"."first_trustee_ellipsis" := TRUE;
              IF "current_row"."loop" = 'first' THEN
                "result"."delegation_loop" := 'first_ellipsis';
              END IF;
            END IF;
          ELSE
            "result"."other_trustee_ellipsis" := TRUE;
            IF "current_row"."loop" = 'first' THEN
              "result"."delegation_loop" := 'other_ellipsis';
            END IF;
          END IF;
        END IF;
      END LOOP;
      RETURN "result";
    END;
  $$;

COMMENT ON FUNCTION "delegation_info"
  ( "member"."id"%TYPE,
    "unit"."id"%TYPE,
    "area"."id"%TYPE,
    "issue"."id"%TYPE,
    "member"."id"%TYPE,
    BOOLEAN )
  IS 'Notable information about a delegation chain for unit, area, or issue; See "delegation_info_type" for more information';



---------------------------
-- Transaction isolation --
---------------------------


CREATE FUNCTION "require_transaction_isolation"()
  RETURNS VOID
  LANGUAGE 'plpgsql' VOLATILE AS $$
    BEGIN
      IF
        current_setting('transaction_isolation') NOT IN
        ('repeatable read', 'serializable')
      THEN
        RAISE EXCEPTION 'Insufficient transaction isolation level' USING
          HINT = 'Consider using SET TRANSACTION ISOLATION LEVEL REPEATABLE READ.';
      END IF;
      RETURN;
    END;
  $$;

COMMENT ON FUNCTION "require_transaction_isolation"() IS 'Throws an exception, if transaction isolation level is too low to provide a consistent snapshot';


CREATE FUNCTION "dont_require_transaction_isolation"()
  RETURNS VOID
  LANGUAGE 'plpgsql' VOLATILE AS $$
    BEGIN
      IF
        current_setting('transaction_isolation') IN
        ('repeatable read', 'serializable')
      THEN
        RAISE WARNING 'Unneccessary transaction isolation level: %',
          current_setting('transaction_isolation');
      END IF;
      RETURN;
    END;
  $$;

COMMENT ON FUNCTION "dont_require_transaction_isolation"() IS 'Raises a warning, if transaction isolation level is higher than READ COMMITTED';



-------------------------
-- Notification system --
-------------------------

CREATE FUNCTION "get_initiatives_for_notification"
  ( "recipient_id_p" "member"."id"%TYPE )
  RETURNS SETOF "initiative_for_notification"
  LANGUAGE 'plpgsql' VOLATILE AS $$
    DECLARE
      "result_row"           "initiative_for_notification"%ROWTYPE;
      "last_draft_id_v"      "draft"."id"%TYPE;
      "last_suggestion_id_v" "suggestion"."id"%TYPE;
    BEGIN
      PERFORM "require_transaction_isolation"();
      PERFORM NULL FROM "member" WHERE "id" = "recipient_id_p" FOR UPDATE;
      FOR "result_row" IN
        SELECT * FROM "initiative_for_notification"
        WHERE "recipient_id" = "recipient_id_p"
      LOOP
        SELECT "id" INTO "last_draft_id_v" FROM "draft"
          WHERE "draft"."initiative_id" = "result_row"."initiative_id"
          ORDER BY "id" DESC LIMIT 1;
        SELECT "id" INTO "last_suggestion_id_v" FROM "suggestion"
          WHERE "suggestion"."initiative_id" = "result_row"."initiative_id"
          ORDER BY "id" DESC LIMIT 1;
        /* compatibility with PostgreSQL 9.1 */
        DELETE FROM "notification_initiative_sent"
          WHERE "member_id" = "recipient_id_p"
          AND "initiative_id" = "result_row"."initiative_id";
        INSERT INTO "notification_initiative_sent"
          ("member_id", "initiative_id", "last_draft_id", "last_suggestion_id")
          VALUES (
            "recipient_id_p",
            "result_row"."initiative_id",
            "last_draft_id_v",
            "last_suggestion_id_v" );
        /* TODO: use alternative code below, requires PostgreSQL 9.5 or higher
        INSERT INTO "notification_initiative_sent"
          ("member_id", "initiative_id", "last_draft_id", "last_suggestion_id")
          VALUES (
            "recipient_id_p",
            "result_row"."initiative_id",
            "last_draft_id_v",
            "last_suggestion_id_v" )
          ON CONFLICT ("member_id", "initiative_id") DO UPDATE SET
            "last_draft_id" = "last_draft_id_v",
            "last_suggestion_id" = "last_suggestion_id_v";
        */
        RETURN NEXT "result_row";
      END LOOP;
      DELETE FROM "notification_initiative_sent"
        USING "initiative", "issue"
        WHERE "notification_initiative_sent"."member_id" = "recipient_id_p"
        AND "initiative"."id" = "notification_initiative_sent"."initiative_id"
        AND "issue"."id" = "initiative"."issue_id"
        AND ( "issue"."closed" NOTNULL OR "issue"."fully_frozen" NOTNULL );
      UPDATE "member" SET
        "notification_counter" = "notification_counter" + 1,
        "notification_sent" = now()
        WHERE "id" = "recipient_id_p";
      RETURN;
    END;
  $$;

COMMENT ON FUNCTION "get_initiatives_for_notification"
  ( "member"."id"%TYPE )
  IS 'Returns rows from view "initiative_for_notification" for a given recipient while updating table "notification_initiative_sent" and columns "notification_counter" and "notification_sent" of "member" table';



------------------------------------------------------------------------
-- Regular tasks, except calculcation of snapshots and voting results --
------------------------------------------------------------------------


CREATE FUNCTION "check_activity"()
  RETURNS VOID
  LANGUAGE 'plpgsql' VOLATILE AS $$
    DECLARE
      "system_setting_row" "system_setting"%ROWTYPE;
    BEGIN
      PERFORM "dont_require_transaction_isolation"();
      SELECT * INTO "system_setting_row" FROM "system_setting";
      IF "system_setting_row"."member_ttl" NOTNULL THEN
        UPDATE "member" SET "active" = FALSE
          WHERE "active" = TRUE
          AND "last_activity" < (now() - "system_setting_row"."member_ttl")::DATE;
      END IF;
      RETURN;
    END;
  $$;

COMMENT ON FUNCTION "check_activity"() IS 'Deactivates members when "last_activity" is older than "system_setting"."member_ttl".';


CREATE FUNCTION "calculate_member_counts"()
  RETURNS VOID
  LANGUAGE 'plpgsql' VOLATILE AS $$
    BEGIN
      PERFORM "require_transaction_isolation"();
      DELETE FROM "member_count";
      INSERT INTO "member_count" ("total_count")
        SELECT "total_count" FROM "member_count_view";
      UPDATE "unit" SET "member_count" = "view"."member_count"
        FROM "unit_member_count" AS "view"
        WHERE "view"."unit_id" = "unit"."id";
      UPDATE "area" SET
        "direct_member_count" = "view"."direct_member_count",
        "member_weight"       = "view"."member_weight"
        FROM "area_member_count" AS "view"
        WHERE "view"."area_id" = "area"."id";
      RETURN;
    END;
  $$;

COMMENT ON FUNCTION "calculate_member_counts"() IS 'Updates "member_count" table and "member_count" column of table "area" by materializing data from views "member_count_view" and "area_member_count"';



------------------------------------
-- Calculation of harmonic weight --
------------------------------------


CREATE VIEW "remaining_harmonic_supporter_weight" AS
  SELECT
    "direct_interest_snapshot"."issue_id",
    "direct_interest_snapshot"."event",
    "direct_interest_snapshot"."member_id",
    "direct_interest_snapshot"."weight" AS "weight_num",
    count("initiative"."id") AS "weight_den"
  FROM "issue"
  JOIN "direct_interest_snapshot"
    ON "issue"."id" = "direct_interest_snapshot"."issue_id"
    AND "issue"."latest_snapshot_event" = "direct_interest_snapshot"."event"
  JOIN "initiative"
    ON "issue"."id" = "initiative"."issue_id"
    AND "initiative"."harmonic_weight" ISNULL
  JOIN "direct_supporter_snapshot"
    ON "initiative"."id" = "direct_supporter_snapshot"."initiative_id"
    AND "direct_interest_snapshot"."event" = "direct_supporter_snapshot"."event"
    AND "direct_interest_snapshot"."member_id" = "direct_supporter_snapshot"."member_id"
    AND (
      "direct_supporter_snapshot"."satisfied" = TRUE OR
      coalesce("initiative"."admitted", FALSE) = FALSE
    )
  GROUP BY
    "direct_interest_snapshot"."issue_id",
    "direct_interest_snapshot"."event",
    "direct_interest_snapshot"."member_id",
    "direct_interest_snapshot"."weight";

COMMENT ON VIEW "remaining_harmonic_supporter_weight" IS 'Helper view for function "set_harmonic_initiative_weights"';


CREATE VIEW "remaining_harmonic_initiative_weight_summands" AS
  SELECT
    "initiative"."issue_id",
    "initiative"."id" AS "initiative_id",
    "initiative"."admitted",
    sum("remaining_harmonic_supporter_weight"."weight_num") AS "weight_num",
    "remaining_harmonic_supporter_weight"."weight_den"
  FROM "remaining_harmonic_supporter_weight"
  JOIN "initiative"
    ON "remaining_harmonic_supporter_weight"."issue_id" = "initiative"."issue_id"
    AND "initiative"."harmonic_weight" ISNULL
  JOIN "direct_supporter_snapshot"
    ON "initiative"."id" = "direct_supporter_snapshot"."initiative_id"
    AND "remaining_harmonic_supporter_weight"."event" = "direct_supporter_snapshot"."event"
    AND "remaining_harmonic_supporter_weight"."member_id" = "direct_supporter_snapshot"."member_id"
    AND (
      "direct_supporter_snapshot"."satisfied" = TRUE OR
      coalesce("initiative"."admitted", FALSE) = FALSE
    )
  GROUP BY
    "initiative"."issue_id",
    "initiative"."id",
    "initiative"."admitted",
    "remaining_harmonic_supporter_weight"."weight_den";

COMMENT ON VIEW "remaining_harmonic_initiative_weight_summands" IS 'Helper view for function "set_harmonic_initiative_weights"';


CREATE VIEW "remaining_harmonic_initiative_weight_dummies" AS
  SELECT
    "issue_id",
    "id" AS "initiative_id",
    "admitted",
    0 AS "weight_num",
    1 AS "weight_den"
  FROM "initiative"
  WHERE "harmonic_weight" ISNULL;

COMMENT ON VIEW "remaining_harmonic_initiative_weight_dummies" IS 'Helper view for function "set_harmonic_initiative_weights" providing dummy weights of zero value, which are needed for corner cases where there are no supporters for an initiative at all';
    

CREATE FUNCTION "set_harmonic_initiative_weights"
  ( "issue_id_p" "issue"."id"%TYPE )
  RETURNS VOID
  LANGUAGE 'plpgsql' VOLATILE AS $$
    DECLARE
      "weight_row"   "remaining_harmonic_initiative_weight_summands"%ROWTYPE;
      "i"            INT4;
      "count_v"      INT4;
      "summand_v"    FLOAT;
      "id_ary"       INT4[];
      "weight_ary"   FLOAT[];
      "min_weight_v" FLOAT;
    BEGIN
      PERFORM "require_transaction_isolation"();
      UPDATE "initiative" SET "harmonic_weight" = NULL
        WHERE "issue_id" = "issue_id_p";
      LOOP
        "min_weight_v" := NULL;
        "i" := 0;
        "count_v" := 0;
        FOR "weight_row" IN
          SELECT * FROM "remaining_harmonic_initiative_weight_summands"
          WHERE "issue_id" = "issue_id_p"
          AND (
            coalesce("admitted", FALSE) = FALSE OR NOT EXISTS (
              SELECT NULL FROM "initiative"
              WHERE "issue_id" = "issue_id_p"
              AND "harmonic_weight" ISNULL
              AND coalesce("admitted", FALSE) = FALSE
            )
          )
          UNION ALL  -- needed for corner cases
          SELECT * FROM "remaining_harmonic_initiative_weight_dummies"
          WHERE "issue_id" = "issue_id_p"
          AND (
            coalesce("admitted", FALSE) = FALSE OR NOT EXISTS (
              SELECT NULL FROM "initiative"
              WHERE "issue_id" = "issue_id_p"
              AND "harmonic_weight" ISNULL
              AND coalesce("admitted", FALSE) = FALSE
            )
          )
          ORDER BY "initiative_id" DESC, "weight_den" DESC
          -- NOTE: non-admitted initiatives placed first (at last positions),
          --       latest initiatives treated worse in case of tie
        LOOP
          "summand_v" := "weight_row"."weight_num"::FLOAT / "weight_row"."weight_den"::FLOAT;
          IF "i" = 0 OR "weight_row"."initiative_id" != "id_ary"["i"] THEN
            "i" := "i" + 1;
            "count_v" := "i";
            "id_ary"["i"] := "weight_row"."initiative_id";
            "weight_ary"["i"] := "summand_v";
          ELSE
            "weight_ary"["i"] := "weight_ary"["i"] + "summand_v";
          END IF;
        END LOOP;
        EXIT WHEN "count_v" = 0;
        "i" := 1;
        LOOP
          "weight_ary"["i"] := "weight_ary"["i"]::NUMERIC(18,9)::NUMERIC(12,3);
          IF "min_weight_v" ISNULL OR "weight_ary"["i"] < "min_weight_v" THEN
            "min_weight_v" := "weight_ary"["i"];
          END IF;
          "i" := "i" + 1;
          EXIT WHEN "i" > "count_v";
        END LOOP;
        "i" := 1;
        LOOP
          IF "weight_ary"["i"] = "min_weight_v" THEN
            UPDATE "initiative" SET "harmonic_weight" = "min_weight_v"
              WHERE "id" = "id_ary"["i"];
            EXIT;
          END IF;
          "i" := "i" + 1;
        END LOOP;
      END LOOP;
      UPDATE "initiative" SET "harmonic_weight" = 0
        WHERE "issue_id" = "issue_id_p" AND "harmonic_weight" ISNULL;
    END;
  $$;

COMMENT ON FUNCTION "set_harmonic_initiative_weights"
  ( "issue"."id"%TYPE )
  IS 'Calculates and sets "harmonic_weight" of initiatives in a given issue';



------------------------------
-- Calculation of snapshots --
------------------------------


CREATE FUNCTION "weight_of_added_delegations_for_population_snapshot"
  ( "issue_id_p"            "issue"."id"%TYPE,
    "member_id_p"           "member"."id"%TYPE,
    "delegate_member_ids_p" "delegating_population_snapshot"."delegate_member_ids"%TYPE )
  RETURNS "direct_population_snapshot"."weight"%TYPE
  LANGUAGE 'plpgsql' VOLATILE AS $$
    DECLARE
      "issue_delegation_row"  "issue_delegation"%ROWTYPE;
      "delegate_member_ids_v" "delegating_population_snapshot"."delegate_member_ids"%TYPE;
      "weight_v"              INT4;
      "sub_weight_v"          INT4;
    BEGIN
      PERFORM "require_transaction_isolation"();
      "weight_v" := 0;
      FOR "issue_delegation_row" IN
        SELECT * FROM "issue_delegation"
        WHERE "trustee_id" = "member_id_p"
        AND "issue_id" = "issue_id_p"
      LOOP
        IF NOT EXISTS (
          SELECT NULL FROM "direct_population_snapshot"
          WHERE "issue_id" = "issue_id_p"
          AND "event" = 'periodic'
          AND "member_id" = "issue_delegation_row"."truster_id"
        ) AND NOT EXISTS (
          SELECT NULL FROM "delegating_population_snapshot"
          WHERE "issue_id" = "issue_id_p"
          AND "event" = 'periodic'
          AND "member_id" = "issue_delegation_row"."truster_id"
        ) THEN
          "delegate_member_ids_v" :=
            "member_id_p" || "delegate_member_ids_p";
          INSERT INTO "delegating_population_snapshot" (
              "issue_id",
              "event",
              "member_id",
              "scope",
              "delegate_member_ids"
            ) VALUES (
              "issue_id_p",
              'periodic',
              "issue_delegation_row"."truster_id",
              "issue_delegation_row"."scope",
              "delegate_member_ids_v"
            );
          "sub_weight_v" := 1 +
            "weight_of_added_delegations_for_population_snapshot"(
              "issue_id_p",
              "issue_delegation_row"."truster_id",
              "delegate_member_ids_v"
            );
          UPDATE "delegating_population_snapshot"
            SET "weight" = "sub_weight_v"
            WHERE "issue_id" = "issue_id_p"
            AND "event" = 'periodic'
            AND "member_id" = "issue_delegation_row"."truster_id";
          "weight_v" := "weight_v" + "sub_weight_v";
        END IF;
      END LOOP;
      RETURN "weight_v";
    END;
  $$;

COMMENT ON FUNCTION "weight_of_added_delegations_for_population_snapshot"
  ( "issue"."id"%TYPE,
    "member"."id"%TYPE,
    "delegating_population_snapshot"."delegate_member_ids"%TYPE )
  IS 'Helper function for "create_population_snapshot" function';


CREATE FUNCTION "create_population_snapshot"
  ( "issue_id_p" "issue"."id"%TYPE )
  RETURNS VOID
  LANGUAGE 'plpgsql' VOLATILE AS $$
    DECLARE
      "member_id_v" "member"."id"%TYPE;
    BEGIN
      PERFORM "require_transaction_isolation"();
      DELETE FROM "direct_population_snapshot"
        WHERE "issue_id" = "issue_id_p"
        AND "event" = 'periodic';
      DELETE FROM "delegating_population_snapshot"
        WHERE "issue_id" = "issue_id_p"
        AND "event" = 'periodic';
      INSERT INTO "direct_population_snapshot"
        ("issue_id", "event", "member_id")
        SELECT
          "issue_id_p"                 AS "issue_id",
          'periodic'::"snapshot_event" AS "event",
          "member"."id"                AS "member_id"
        FROM "issue"
        JOIN "area" ON "issue"."area_id" = "area"."id"
        JOIN "membership" ON "area"."id" = "membership"."area_id"
        JOIN "member" ON "membership"."member_id" = "member"."id"
        JOIN "privilege"
          ON "privilege"."unit_id" = "area"."unit_id"
          AND "privilege"."member_id" = "member"."id"
        WHERE "issue"."id" = "issue_id_p"
        AND "member"."active" AND "privilege"."voting_right"
        UNION
        SELECT
          "issue_id_p"                 AS "issue_id",
          'periodic'::"snapshot_event" AS "event",
          "member"."id"                AS "member_id"
        FROM "issue"
        JOIN "area" ON "issue"."area_id" = "area"."id"
        JOIN "interest" ON "issue"."id" = "interest"."issue_id"
        JOIN "member" ON "interest"."member_id" = "member"."id"
        JOIN "privilege"
          ON "privilege"."unit_id" = "area"."unit_id"
          AND "privilege"."member_id" = "member"."id"
        WHERE "issue"."id" = "issue_id_p"
        AND "member"."active" AND "privilege"."voting_right";
      FOR "member_id_v" IN
        SELECT "member_id" FROM "direct_population_snapshot"
        WHERE "issue_id" = "issue_id_p"
        AND "event" = 'periodic'
      LOOP
        UPDATE "direct_population_snapshot" SET
          "weight" = 1 +
            "weight_of_added_delegations_for_population_snapshot"(
              "issue_id_p",
              "member_id_v",
              '{}'
            )
          WHERE "issue_id" = "issue_id_p"
          AND "event" = 'periodic'
          AND "member_id" = "member_id_v";
      END LOOP;
      RETURN;
    END;
  $$;

COMMENT ON FUNCTION "create_population_snapshot"
  ( "issue"."id"%TYPE )
  IS 'This function creates a new ''periodic'' population snapshot for the given issue. It does neither lock any tables, nor updates precalculated values in other tables.';


CREATE FUNCTION "weight_of_added_delegations_for_interest_snapshot"
  ( "issue_id_p"            "issue"."id"%TYPE,
    "member_id_p"           "member"."id"%TYPE,
    "delegate_member_ids_p" "delegating_interest_snapshot"."delegate_member_ids"%TYPE )
  RETURNS "direct_interest_snapshot"."weight"%TYPE
  LANGUAGE 'plpgsql' VOLATILE AS $$
    DECLARE
      "issue_delegation_row"  "issue_delegation"%ROWTYPE;
      "delegate_member_ids_v" "delegating_interest_snapshot"."delegate_member_ids"%TYPE;
      "weight_v"              INT4;
      "sub_weight_v"          INT4;
    BEGIN
      PERFORM "require_transaction_isolation"();
      "weight_v" := 0;
      FOR "issue_delegation_row" IN
        SELECT * FROM "issue_delegation"
        WHERE "trustee_id" = "member_id_p"
        AND "issue_id" = "issue_id_p"
      LOOP
        IF NOT EXISTS (
          SELECT NULL FROM "direct_interest_snapshot"
          WHERE "issue_id" = "issue_id_p"
          AND "event" = 'periodic'
          AND "member_id" = "issue_delegation_row"."truster_id"
        ) AND NOT EXISTS (
          SELECT NULL FROM "delegating_interest_snapshot"
          WHERE "issue_id" = "issue_id_p"
          AND "event" = 'periodic'
          AND "member_id" = "issue_delegation_row"."truster_id"
        ) THEN
          "delegate_member_ids_v" :=
            "member_id_p" || "delegate_member_ids_p";
          INSERT INTO "delegating_interest_snapshot" (
              "issue_id",
              "event",
              "member_id",
              "scope",
              "delegate_member_ids"
            ) VALUES (
              "issue_id_p",
              'periodic',
              "issue_delegation_row"."truster_id",
              "issue_delegation_row"."scope",
              "delegate_member_ids_v"
            );
          "sub_weight_v" := 1 +
            "weight_of_added_delegations_for_interest_snapshot"(
              "issue_id_p",
              "issue_delegation_row"."truster_id",
              "delegate_member_ids_v"
            );
          UPDATE "delegating_interest_snapshot"
            SET "weight" = "sub_weight_v"
            WHERE "issue_id" = "issue_id_p"
            AND "event" = 'periodic'
            AND "member_id" = "issue_delegation_row"."truster_id";
          "weight_v" := "weight_v" + "sub_weight_v";
        END IF;
      END LOOP;
      RETURN "weight_v";
    END;
  $$;

COMMENT ON FUNCTION "weight_of_added_delegations_for_interest_snapshot"
  ( "issue"."id"%TYPE,
    "member"."id"%TYPE,
    "delegating_interest_snapshot"."delegate_member_ids"%TYPE )
  IS 'Helper function for "create_interest_snapshot" function';


CREATE FUNCTION "create_interest_snapshot"
  ( "issue_id_p" "issue"."id"%TYPE )
  RETURNS VOID
  LANGUAGE 'plpgsql' VOLATILE AS $$
    DECLARE
      "member_id_v" "member"."id"%TYPE;
    BEGIN
      PERFORM "require_transaction_isolation"();
      DELETE FROM "direct_interest_snapshot"
        WHERE "issue_id" = "issue_id_p"
        AND "event" = 'periodic';
      DELETE FROM "delegating_interest_snapshot"
        WHERE "issue_id" = "issue_id_p"
        AND "event" = 'periodic';
      DELETE FROM "direct_supporter_snapshot"
        USING "initiative"  -- NOTE: due to missing index on issue_id
        WHERE "initiative"."issue_id" = "issue_id_p"
        AND "direct_supporter_snapshot"."initiative_id" = "initiative"."id"
        AND "direct_supporter_snapshot"."event" = 'periodic';
      INSERT INTO "direct_interest_snapshot"
        ("issue_id", "event", "member_id")
        SELECT
          "issue_id_p"  AS "issue_id",
          'periodic'    AS "event",
          "member"."id" AS "member_id"
        FROM "issue"
        JOIN "area" ON "issue"."area_id" = "area"."id"
        JOIN "interest" ON "issue"."id" = "interest"."issue_id"
        JOIN "member" ON "interest"."member_id" = "member"."id"
        JOIN "privilege"
          ON "privilege"."unit_id" = "area"."unit_id"
          AND "privilege"."member_id" = "member"."id"
        WHERE "issue"."id" = "issue_id_p"
        AND "member"."active" AND "privilege"."voting_right";
      FOR "member_id_v" IN
        SELECT "member_id" FROM "direct_interest_snapshot"
        WHERE "issue_id" = "issue_id_p"
        AND "event" = 'periodic'
      LOOP
        UPDATE "direct_interest_snapshot" SET
          "weight" = 1 +
            "weight_of_added_delegations_for_interest_snapshot"(
              "issue_id_p",
              "member_id_v",
              '{}'
            )
          WHERE "issue_id" = "issue_id_p"
          AND "event" = 'periodic'
          AND "member_id" = "member_id_v";
      END LOOP;
      INSERT INTO "direct_supporter_snapshot"
        ( "issue_id", "initiative_id", "event", "member_id",
          "draft_id", "informed", "satisfied" )
        SELECT
          "issue_id_p"            AS "issue_id",
          "initiative"."id"       AS "initiative_id",
          'periodic'              AS "event",
          "supporter"."member_id" AS "member_id",
          "supporter"."draft_id"  AS "draft_id",
          "supporter"."draft_id" = "current_draft"."id" AS "informed",
          NOT EXISTS (
            SELECT NULL FROM "critical_opinion"
            WHERE "initiative_id" = "initiative"."id"
            AND "member_id" = "supporter"."member_id"
          ) AS "satisfied"
        FROM "initiative"
        JOIN "supporter"
        ON "supporter"."initiative_id" = "initiative"."id"
        JOIN "current_draft"
        ON "initiative"."id" = "current_draft"."initiative_id"
        JOIN "direct_interest_snapshot"
        ON "supporter"."member_id" = "direct_interest_snapshot"."member_id"
        AND "initiative"."issue_id" = "direct_interest_snapshot"."issue_id"
        AND "event" = 'periodic'
        WHERE "initiative"."issue_id" = "issue_id_p";
      RETURN;
    END;
  $$;

COMMENT ON FUNCTION "create_interest_snapshot"
  ( "issue"."id"%TYPE )
  IS 'This function creates a new ''periodic'' interest/supporter snapshot for the given issue. It does neither lock any tables, nor updates precalculated values in other tables.';


CREATE FUNCTION "create_snapshot"
  ( "issue_id_p" "issue"."id"%TYPE )
  RETURNS VOID
  LANGUAGE 'plpgsql' VOLATILE AS $$
    DECLARE
      "initiative_id_v"    "initiative"."id"%TYPE;
      "suggestion_id_v"    "suggestion"."id"%TYPE;
    BEGIN
      PERFORM "require_transaction_isolation"();
      PERFORM "create_population_snapshot"("issue_id_p");
      PERFORM "create_interest_snapshot"("issue_id_p");
      UPDATE "issue" SET
        "snapshot" = coalesce("phase_finished", now()),
        "latest_snapshot_event" = 'periodic',
        "population" = (
          SELECT coalesce(sum("weight"), 0)
          FROM "direct_population_snapshot"
          WHERE "issue_id" = "issue_id_p"
          AND "event" = 'periodic'
        )
        WHERE "id" = "issue_id_p";
      FOR "initiative_id_v" IN
        SELECT "id" FROM "initiative" WHERE "issue_id" = "issue_id_p"
      LOOP
        UPDATE "initiative" SET
          "supporter_count" = (
            SELECT coalesce(sum("di"."weight"), 0)
            FROM "direct_interest_snapshot" AS "di"
            JOIN "direct_supporter_snapshot" AS "ds"
            ON "di"."member_id" = "ds"."member_id"
            WHERE "di"."issue_id" = "issue_id_p"
            AND "di"."event" = 'periodic'
            AND "ds"."initiative_id" = "initiative_id_v"
            AND "ds"."event" = 'periodic'
          ),
          "informed_supporter_count" = (
            SELECT coalesce(sum("di"."weight"), 0)
            FROM "direct_interest_snapshot" AS "di"
            JOIN "direct_supporter_snapshot" AS "ds"
            ON "di"."member_id" = "ds"."member_id"
            WHERE "di"."issue_id" = "issue_id_p"
            AND "di"."event" = 'periodic'
            AND "ds"."initiative_id" = "initiative_id_v"
            AND "ds"."event" = 'periodic'
            AND "ds"."informed"
          ),
          "satisfied_supporter_count" = (
            SELECT coalesce(sum("di"."weight"), 0)
            FROM "direct_interest_snapshot" AS "di"
            JOIN "direct_supporter_snapshot" AS "ds"
            ON "di"."member_id" = "ds"."member_id"
            WHERE "di"."issue_id" = "issue_id_p"
            AND "di"."event" = 'periodic'
            AND "ds"."initiative_id" = "initiative_id_v"
            AND "ds"."event" = 'periodic'
            AND "ds"."satisfied"
          ),
          "satisfied_informed_supporter_count" = (
            SELECT coalesce(sum("di"."weight"), 0)
            FROM "direct_interest_snapshot" AS "di"
            JOIN "direct_supporter_snapshot" AS "ds"
            ON "di"."member_id" = "ds"."member_id"
            WHERE "di"."issue_id" = "issue_id_p"
            AND "di"."event" = 'periodic'
            AND "ds"."initiative_id" = "initiative_id_v"
            AND "ds"."event" = 'periodic'
            AND "ds"."informed"
            AND "ds"."satisfied"
          )
          WHERE "id" = "initiative_id_v";
        FOR "suggestion_id_v" IN
          SELECT "id" FROM "suggestion"
          WHERE "initiative_id" = "initiative_id_v"
        LOOP
          UPDATE "suggestion" SET
            "minus2_unfulfilled_count" = (
              SELECT coalesce(sum("snapshot"."weight"), 0)
              FROM "issue" CROSS JOIN "opinion"
              JOIN "direct_interest_snapshot" AS "snapshot"
              ON "snapshot"."issue_id" = "issue"."id"
              AND "snapshot"."event" = "issue"."latest_snapshot_event"
              AND "snapshot"."member_id" = "opinion"."member_id"
              WHERE "issue"."id" = "issue_id_p"
              AND "opinion"."suggestion_id" = "suggestion_id_v"
              AND "opinion"."degree" = -2
              AND "opinion"."fulfilled" = FALSE
            ),
            "minus2_fulfilled_count" = (
              SELECT coalesce(sum("snapshot"."weight"), 0)
              FROM "issue" CROSS JOIN "opinion"
              JOIN "direct_interest_snapshot" AS "snapshot"
              ON "snapshot"."issue_id" = "issue"."id"
              AND "snapshot"."event" = "issue"."latest_snapshot_event"
              AND "snapshot"."member_id" = "opinion"."member_id"
              WHERE "issue"."id" = "issue_id_p"
              AND "opinion"."suggestion_id" = "suggestion_id_v"
              AND "opinion"."degree" = -2
              AND "opinion"."fulfilled" = TRUE
            ),
            "minus1_unfulfilled_count" = (
              SELECT coalesce(sum("snapshot"."weight"), 0)
              FROM "issue" CROSS JOIN "opinion"
              JOIN "direct_interest_snapshot" AS "snapshot"
              ON "snapshot"."issue_id" = "issue"."id"
              AND "snapshot"."event" = "issue"."latest_snapshot_event"
              AND "snapshot"."member_id" = "opinion"."member_id"
              WHERE "issue"."id" = "issue_id_p"
              AND "opinion"."suggestion_id" = "suggestion_id_v"
              AND "opinion"."degree" = -1
              AND "opinion"."fulfilled" = FALSE
            ),
            "minus1_fulfilled_count" = (
              SELECT coalesce(sum("snapshot"."weight"), 0)
              FROM "issue" CROSS JOIN "opinion"
              JOIN "direct_interest_snapshot" AS "snapshot"
              ON "snapshot"."issue_id" = "issue"."id"
              AND "snapshot"."event" = "issue"."latest_snapshot_event"
              AND "snapshot"."member_id" = "opinion"."member_id"
              WHERE "issue"."id" = "issue_id_p"
              AND "opinion"."suggestion_id" = "suggestion_id_v"
              AND "opinion"."degree" = -1
              AND "opinion"."fulfilled" = TRUE
            ),
            "plus1_unfulfilled_count" = (
              SELECT coalesce(sum("snapshot"."weight"), 0)
              FROM "issue" CROSS JOIN "opinion"
              JOIN "direct_interest_snapshot" AS "snapshot"
              ON "snapshot"."issue_id" = "issue"."id"
              AND "snapshot"."event" = "issue"."latest_snapshot_event"
              AND "snapshot"."member_id" = "opinion"."member_id"
              WHERE "issue"."id" = "issue_id_p"
              AND "opinion"."suggestion_id" = "suggestion_id_v"
              AND "opinion"."degree" = 1
              AND "opinion"."fulfilled" = FALSE
            ),
            "plus1_fulfilled_count" = (
              SELECT coalesce(sum("snapshot"."weight"), 0)
              FROM "issue" CROSS JOIN "opinion"
              JOIN "direct_interest_snapshot" AS "snapshot"
              ON "snapshot"."issue_id" = "issue"."id"
              AND "snapshot"."event" = "issue"."latest_snapshot_event"
              AND "snapshot"."member_id" = "opinion"."member_id"
              WHERE "issue"."id" = "issue_id_p"
              AND "opinion"."suggestion_id" = "suggestion_id_v"
              AND "opinion"."degree" = 1
              AND "opinion"."fulfilled" = TRUE
            ),
            "plus2_unfulfilled_count" = (
              SELECT coalesce(sum("snapshot"."weight"), 0)
              FROM "issue" CROSS JOIN "opinion"
              JOIN "direct_interest_snapshot" AS "snapshot"
              ON "snapshot"."issue_id" = "issue"."id"
              AND "snapshot"."event" = "issue"."latest_snapshot_event"
              AND "snapshot"."member_id" = "opinion"."member_id"
              WHERE "issue"."id" = "issue_id_p"
              AND "opinion"."suggestion_id" = "suggestion_id_v"
              AND "opinion"."degree" = 2
              AND "opinion"."fulfilled" = FALSE
            ),
            "plus2_fulfilled_count" = (
              SELECT coalesce(sum("snapshot"."weight"), 0)
              FROM "issue" CROSS JOIN "opinion"
              JOIN "direct_interest_snapshot" AS "snapshot"
              ON "snapshot"."issue_id" = "issue"."id"
              AND "snapshot"."event" = "issue"."latest_snapshot_event"
              AND "snapshot"."member_id" = "opinion"."member_id"
              WHERE "issue"."id" = "issue_id_p"
              AND "opinion"."suggestion_id" = "suggestion_id_v"
              AND "opinion"."degree" = 2
              AND "opinion"."fulfilled" = TRUE
            )
            WHERE "suggestion"."id" = "suggestion_id_v";
        END LOOP;
      END LOOP;
      RETURN;
    END;
  $$;

COMMENT ON FUNCTION "create_snapshot"
  ( "issue"."id"%TYPE )
  IS 'This function creates a complete new ''periodic'' snapshot of population, interest and support for the given issue. All involved tables are locked, and after completion precalculated values in the source tables are updated.';


CREATE FUNCTION "set_snapshot_event"
  ( "issue_id_p" "issue"."id"%TYPE,
    "event_p" "snapshot_event" )
  RETURNS VOID
  LANGUAGE 'plpgsql' VOLATILE AS $$
    DECLARE
      "event_v" "issue"."latest_snapshot_event"%TYPE;
    BEGIN
      PERFORM "require_transaction_isolation"();
      SELECT "latest_snapshot_event" INTO "event_v" FROM "issue"
        WHERE "id" = "issue_id_p" FOR UPDATE;
      UPDATE "issue" SET "latest_snapshot_event" = "event_p"
        WHERE "id" = "issue_id_p";
      UPDATE "direct_population_snapshot" SET "event" = "event_p"
        WHERE "issue_id" = "issue_id_p" AND "event" = "event_v";
      UPDATE "delegating_population_snapshot" SET "event" = "event_p"
        WHERE "issue_id" = "issue_id_p" AND "event" = "event_v";
      UPDATE "direct_interest_snapshot" SET "event" = "event_p"
        WHERE "issue_id" = "issue_id_p" AND "event" = "event_v";
      UPDATE "delegating_interest_snapshot" SET "event" = "event_p"
        WHERE "issue_id" = "issue_id_p" AND "event" = "event_v";
      UPDATE "direct_supporter_snapshot" SET "event" = "event_p"
        FROM "initiative"  -- NOTE: due to missing index on issue_id
        WHERE "initiative"."issue_id" = "issue_id_p"
        AND "direct_supporter_snapshot"."initiative_id" = "initiative"."id"
        AND "direct_supporter_snapshot"."event" = "event_v";
      RETURN;
    END;
  $$;

COMMENT ON FUNCTION "set_snapshot_event"
  ( "issue"."id"%TYPE,
    "snapshot_event" )
  IS 'Change "event" attribute of the previous ''periodic'' snapshot';



-----------------------
-- Counting of votes --
-----------------------


CREATE FUNCTION "weight_of_added_vote_delegations"
  ( "issue_id_p"            "issue"."id"%TYPE,
    "member_id_p"           "member"."id"%TYPE,
    "delegate_member_ids_p" "delegating_voter"."delegate_member_ids"%TYPE )
  RETURNS "direct_voter"."weight"%TYPE
  LANGUAGE 'plpgsql' VOLATILE AS $$
    DECLARE
      "issue_delegation_row"  "issue_delegation"%ROWTYPE;
      "delegate_member_ids_v" "delegating_voter"."delegate_member_ids"%TYPE;
      "weight_v"              INT4;
      "sub_weight_v"          INT4;
    BEGIN
      PERFORM "require_transaction_isolation"();
      "weight_v" := 0;
      FOR "issue_delegation_row" IN
        SELECT * FROM "issue_delegation"
        WHERE "trustee_id" = "member_id_p"
        AND "issue_id" = "issue_id_p"
      LOOP
        IF NOT EXISTS (
          SELECT NULL FROM "direct_voter"
          WHERE "member_id" = "issue_delegation_row"."truster_id"
          AND "issue_id" = "issue_id_p"
        ) AND NOT EXISTS (
          SELECT NULL FROM "delegating_voter"
          WHERE "member_id" = "issue_delegation_row"."truster_id"
          AND "issue_id" = "issue_id_p"
        ) THEN
          "delegate_member_ids_v" :=
            "member_id_p" || "delegate_member_ids_p";
          INSERT INTO "delegating_voter" (
              "issue_id",
              "member_id",
              "scope",
              "delegate_member_ids"
            ) VALUES (
              "issue_id_p",
              "issue_delegation_row"."truster_id",
              "issue_delegation_row"."scope",
              "delegate_member_ids_v"
            );
          "sub_weight_v" := 1 +
            "weight_of_added_vote_delegations"(
              "issue_id_p",
              "issue_delegation_row"."truster_id",
              "delegate_member_ids_v"
            );
          UPDATE "delegating_voter"
            SET "weight" = "sub_weight_v"
            WHERE "issue_id" = "issue_id_p"
            AND "member_id" = "issue_delegation_row"."truster_id";
          "weight_v" := "weight_v" + "sub_weight_v";
        END IF;
      END LOOP;
      RETURN "weight_v";
    END;
  $$;

COMMENT ON FUNCTION "weight_of_added_vote_delegations"
  ( "issue"."id"%TYPE,
    "member"."id"%TYPE,
    "delegating_voter"."delegate_member_ids"%TYPE )
  IS 'Helper function for "add_vote_delegations" function';


CREATE FUNCTION "add_vote_delegations"
  ( "issue_id_p" "issue"."id"%TYPE )
  RETURNS VOID
  LANGUAGE 'plpgsql' VOLATILE AS $$
    DECLARE
      "member_id_v" "member"."id"%TYPE;
    BEGIN
      PERFORM "require_transaction_isolation"();
      FOR "member_id_v" IN
        SELECT "member_id" FROM "direct_voter"
        WHERE "issue_id" = "issue_id_p"
      LOOP
        UPDATE "direct_voter" SET
          "weight" = "weight" + "weight_of_added_vote_delegations"(
            "issue_id_p",
            "member_id_v",
            '{}'
          )
          WHERE "member_id" = "member_id_v"
          AND "issue_id" = "issue_id_p";
      END LOOP;
      RETURN;
    END;
  $$;

COMMENT ON FUNCTION "add_vote_delegations"
  ( "issue_id_p" "issue"."id"%TYPE )
  IS 'Helper function for "close_voting" function';


CREATE FUNCTION "close_voting"("issue_id_p" "issue"."id"%TYPE)
  RETURNS VOID
  LANGUAGE 'plpgsql' VOLATILE AS $$
    DECLARE
      "area_id_v"   "area"."id"%TYPE;
      "unit_id_v"   "unit"."id"%TYPE;
      "member_id_v" "member"."id"%TYPE;
    BEGIN
      PERFORM "require_transaction_isolation"();
      SELECT "area_id" INTO "area_id_v" FROM "issue" WHERE "id" = "issue_id_p";
      SELECT "unit_id" INTO "unit_id_v" FROM "area"  WHERE "id" = "area_id_v";
      -- override protection triggers:
      INSERT INTO "temporary_transaction_data" ("key", "value")
        VALUES ('override_protection_triggers', TRUE::TEXT);
      -- delete timestamp of voting comment:
      UPDATE "direct_voter" SET "comment_changed" = NULL
        WHERE "issue_id" = "issue_id_p";
      -- delete delegating votes (in cases of manual reset of issue state):
      DELETE FROM "delegating_voter"
        WHERE "issue_id" = "issue_id_p";
      -- delete votes from non-privileged voters:
      DELETE FROM "direct_voter"
        USING (
          SELECT
            "direct_voter"."member_id"
          FROM "direct_voter"
          JOIN "member" ON "direct_voter"."member_id" = "member"."id"
          LEFT JOIN "privilege"
          ON "privilege"."unit_id" = "unit_id_v"
          AND "privilege"."member_id" = "direct_voter"."member_id"
          WHERE "direct_voter"."issue_id" = "issue_id_p" AND (
            "member"."active" = FALSE OR
            "privilege"."voting_right" ISNULL OR
            "privilege"."voting_right" = FALSE
          )
        ) AS "subquery"
        WHERE "direct_voter"."issue_id" = "issue_id_p"
        AND "direct_voter"."member_id" = "subquery"."member_id";
      -- consider delegations:
      UPDATE "direct_voter" SET "weight" = 1
        WHERE "issue_id" = "issue_id_p";
      PERFORM "add_vote_delegations"("issue_id_p");
      -- mark first preferences:
      UPDATE "vote" SET "first_preference" = "subquery"."first_preference"
        FROM (
          SELECT
            "vote"."initiative_id",
            "vote"."member_id",
            CASE WHEN "vote"."grade" > 0 THEN
              CASE WHEN "vote"."grade" = max("agg"."grade") THEN TRUE ELSE FALSE END
            ELSE NULL
            END AS "first_preference"
          FROM "vote"
          JOIN "initiative"  -- NOTE: due to missing index on issue_id
          ON "vote"."issue_id" = "initiative"."issue_id"
          JOIN "vote" AS "agg"
          ON "initiative"."id" = "agg"."initiative_id"
          AND "vote"."member_id" = "agg"."member_id"
          GROUP BY "vote"."initiative_id", "vote"."member_id", "vote"."grade"
        ) AS "subquery"
        WHERE "vote"."issue_id" = "issue_id_p"
        AND "vote"."initiative_id" = "subquery"."initiative_id"
        AND "vote"."member_id" = "subquery"."member_id";
      -- finish overriding protection triggers (avoids garbage):
      DELETE FROM "temporary_transaction_data"
        WHERE "key" = 'override_protection_triggers';
      -- materialize battle_view:
      -- NOTE: "closed" column of issue must be set at this point
      DELETE FROM "battle" WHERE "issue_id" = "issue_id_p";
      INSERT INTO "battle" (
        "issue_id",
        "winning_initiative_id", "losing_initiative_id",
        "count"
      ) SELECT
        "issue_id",
        "winning_initiative_id", "losing_initiative_id",
        "count"
        FROM "battle_view" WHERE "issue_id" = "issue_id_p";
      -- set voter count:
      UPDATE "issue" SET
        "voter_count" = (
          SELECT coalesce(sum("weight"), 0)
          FROM "direct_voter" WHERE "issue_id" = "issue_id_p"
        )
        WHERE "id" = "issue_id_p";
      -- copy "positive_votes" and "negative_votes" from "battle" table:
      -- NOTE: "first_preference_votes" is set to a default of 0 at this step
      UPDATE "initiative" SET
        "first_preference_votes" = 0,
        "positive_votes" = "battle_win"."count",
        "negative_votes" = "battle_lose"."count"
        FROM "battle" AS "battle_win", "battle" AS "battle_lose"
        WHERE
          "battle_win"."issue_id" = "issue_id_p" AND
          "battle_win"."winning_initiative_id" = "initiative"."id" AND
          "battle_win"."losing_initiative_id" ISNULL AND
          "battle_lose"."issue_id" = "issue_id_p" AND
          "battle_lose"."losing_initiative_id" = "initiative"."id" AND
          "battle_lose"."winning_initiative_id" ISNULL;
      -- calculate "first_preference_votes":
      -- NOTE: will only set values not equal to zero
      UPDATE "initiative" SET "first_preference_votes" = "subquery"."sum"
        FROM (
          SELECT "vote"."initiative_id", sum("direct_voter"."weight")
          FROM "vote" JOIN "direct_voter"
          ON "vote"."issue_id" = "direct_voter"."issue_id"
          AND "vote"."member_id" = "direct_voter"."member_id"
          WHERE "vote"."first_preference"
          GROUP BY "vote"."initiative_id"
        ) AS "subquery"
        WHERE "initiative"."issue_id" = "issue_id_p"
        AND "initiative"."admitted"
        AND "initiative"."id" = "subquery"."initiative_id";
    END;
  $$;

COMMENT ON FUNCTION "close_voting"
  ( "issue"."id"%TYPE )
  IS 'Closes the voting on an issue, and calculates positive and negative votes for each initiative; The ranking is not calculated yet, to keep the (locking) transaction short.';


CREATE FUNCTION "defeat_strength"
  ( "positive_votes_p"  INT4,
    "negative_votes_p"  INT4,
    "defeat_strength_p" "defeat_strength" )
  RETURNS INT8
  LANGUAGE 'plpgsql' IMMUTABLE AS $$
    BEGIN
      IF "defeat_strength_p" = 'simple'::"defeat_strength" THEN
        IF "positive_votes_p" > "negative_votes_p" THEN
          RETURN "positive_votes_p";
        ELSE
          RETURN 0;
        END IF;
      ELSE
        IF "positive_votes_p" > "negative_votes_p" THEN
          RETURN ("positive_votes_p"::INT8 << 31) - "negative_votes_p"::INT8;
        ELSIF "positive_votes_p" = "negative_votes_p" THEN
          RETURN 0;
        ELSE
          RETURN -1;
        END IF;
      END IF;
    END;
  $$;

COMMENT ON FUNCTION "defeat_strength"(INT4, INT4, "defeat_strength") IS 'Calculates defeat strength (INT8!) according to the "defeat_strength" option (see comment on type "defeat_strength")';


CREATE FUNCTION "secondary_link_strength"
  ( "initiative1_ord_p" INT4,
    "initiative2_ord_p" INT4,
    "tie_breaking_p"   "tie_breaking" )
  RETURNS INT8
  LANGUAGE 'plpgsql' IMMUTABLE AS $$
    BEGIN
      IF "initiative1_ord_p" = "initiative2_ord_p" THEN
        RAISE EXCEPTION 'Identical initiative ids passed to "secondary_link_strength" function (should not happen)';
      END IF;
      RETURN (
        CASE WHEN "tie_breaking_p" = 'simple'::"tie_breaking" THEN
          0
        ELSE
          CASE WHEN "initiative1_ord_p" < "initiative2_ord_p" THEN
            1::INT8 << 62
          ELSE 0 END
          +
          CASE WHEN "tie_breaking_p" = 'variant2'::"tie_breaking" THEN
            ("initiative2_ord_p"::INT8 << 31) - "initiative1_ord_p"::INT8
          ELSE
            "initiative2_ord_p"::INT8 - ("initiative1_ord_p"::INT8 << 31)
          END
        END
      );
    END;
  $$;

COMMENT ON FUNCTION "secondary_link_strength"(INT4, INT4, "tie_breaking") IS 'Calculates a secondary criterion for the defeat strength (tie-breaking of the links)';


CREATE TYPE "link_strength" AS (
        "primary"               INT8,
        "secondary"             INT8 );

COMMENT ON TYPE "link_strength" IS 'Type to store the defeat strength of a link between two candidates plus a secondary criterion to create unique link strengths between the candidates (needed for tie-breaking ''variant1'' and ''variant2'')';


CREATE FUNCTION "find_best_paths"("matrix_d" "link_strength"[][])
  RETURNS "link_strength"[][]
  LANGUAGE 'plpgsql' IMMUTABLE AS $$
    DECLARE
      "dimension_v" INT4;
      "matrix_p"    "link_strength"[][];
      "i"           INT4;
      "j"           INT4;
      "k"           INT4;
    BEGIN
      "dimension_v" := array_upper("matrix_d", 1);
      "matrix_p" := "matrix_d";
      "i" := 1;
      LOOP
        "j" := 1;
        LOOP
          IF "i" != "j" THEN
            "k" := 1;
            LOOP
              IF "i" != "k" AND "j" != "k" THEN
                IF "matrix_p"["j"]["i"] < "matrix_p"["i"]["k"] THEN
                  IF "matrix_p"["j"]["i"] > "matrix_p"["j"]["k"] THEN
                    "matrix_p"["j"]["k"] := "matrix_p"["j"]["i"];
                  END IF;
                ELSE
                  IF "matrix_p"["i"]["k"] > "matrix_p"["j"]["k"] THEN
                    "matrix_p"["j"]["k"] := "matrix_p"["i"]["k"];
                  END IF;
                END IF;
              END IF;
              EXIT WHEN "k" = "dimension_v";
              "k" := "k" + 1;
            END LOOP;
          END IF;
          EXIT WHEN "j" = "dimension_v";
          "j" := "j" + 1;
        END LOOP;
        EXIT WHEN "i" = "dimension_v";
        "i" := "i" + 1;
      END LOOP;
      RETURN "matrix_p";
    END;
  $$;

COMMENT ON FUNCTION "find_best_paths"("link_strength"[][]) IS 'Computes the strengths of the best beat-paths from a square matrix';


CREATE FUNCTION "calculate_ranks"("issue_id_p" "issue"."id"%TYPE)
  RETURNS VOID
  LANGUAGE 'plpgsql' VOLATILE AS $$
    DECLARE
      "issue_row"       "issue"%ROWTYPE;
      "policy_row"      "policy"%ROWTYPE;
      "dimension_v"     INT4;
      "matrix_a"        INT4[][];  -- absolute votes
      "matrix_d"        "link_strength"[][];  -- defeat strength (direct)
      "matrix_p"        "link_strength"[][];  -- defeat strength (best path)
      "matrix_t"        "link_strength"[][];  -- defeat strength (tie-breaking)
      "matrix_f"        BOOLEAN[][];  -- forbidden link (tie-breaking)
      "matrix_b"        BOOLEAN[][];  -- final order (who beats who)
      "i"               INT4;
      "j"               INT4;
      "m"               INT4;
      "n"               INT4;
      "battle_row"      "battle"%ROWTYPE;
      "rank_ary"        INT4[];
      "rank_v"          INT4;
      "initiative_id_v" "initiative"."id"%TYPE;
    BEGIN
      PERFORM "require_transaction_isolation"();
      SELECT * INTO "issue_row"
        FROM "issue" WHERE "id" = "issue_id_p";
      SELECT * INTO "policy_row"
        FROM "policy" WHERE "id" = "issue_row"."policy_id";
      SELECT count(1) INTO "dimension_v"
        FROM "battle_participant" WHERE "issue_id" = "issue_id_p";
      -- create "matrix_a" with absolute number of votes in pairwise
      -- comparison:
      "matrix_a" := array_fill(NULL::INT4, ARRAY["dimension_v", "dimension_v"]);
      "i" := 1;
      "j" := 2;
      FOR "battle_row" IN
        SELECT * FROM "battle" WHERE "issue_id" = "issue_id_p"
        ORDER BY
        "winning_initiative_id" NULLS FIRST,
        "losing_initiative_id" NULLS FIRST
      LOOP
        "matrix_a"["i"]["j"] := "battle_row"."count";
        IF "j" = "dimension_v" THEN
          "i" := "i" + 1;
          "j" := 1;
        ELSE
          "j" := "j" + 1;
          IF "j" = "i" THEN
            "j" := "j" + 1;
          END IF;
        END IF;
      END LOOP;
      IF "i" != "dimension_v" OR "j" != "dimension_v" + 1 THEN
        RAISE EXCEPTION 'Wrong battle count (should not happen)';
      END IF;
      -- store direct defeat strengths in "matrix_d" using "defeat_strength"
      -- and "secondary_link_strength" functions:
      "matrix_d" := array_fill(NULL::INT8, ARRAY["dimension_v", "dimension_v"]);
      "i" := 1;
      LOOP
        "j" := 1;
        LOOP
          IF "i" != "j" THEN
            "matrix_d"["i"]["j"] := (
              "defeat_strength"(
                "matrix_a"["i"]["j"],
                "matrix_a"["j"]["i"],
                "policy_row"."defeat_strength"
              ),
              "secondary_link_strength"(
                "i",
                "j",
                "policy_row"."tie_breaking"
              )
            )::"link_strength";
          END IF;
          EXIT WHEN "j" = "dimension_v";
          "j" := "j" + 1;
        END LOOP;
        EXIT WHEN "i" = "dimension_v";
        "i" := "i" + 1;
      END LOOP;
      -- find best paths:
      "matrix_p" := "find_best_paths"("matrix_d");
      -- create partial order:
      "matrix_b" := array_fill(NULL::BOOLEAN, ARRAY["dimension_v", "dimension_v"]);
      "i" := 1;
      LOOP
        "j" := "i" + 1;
        LOOP
          IF "i" != "j" THEN
            IF "matrix_p"["i"]["j"] > "matrix_p"["j"]["i"] THEN
              "matrix_b"["i"]["j"] := TRUE;
              "matrix_b"["j"]["i"] := FALSE;
            ELSIF "matrix_p"["i"]["j"] < "matrix_p"["j"]["i"] THEN
              "matrix_b"["i"]["j"] := FALSE;
              "matrix_b"["j"]["i"] := TRUE;
            END IF;
          END IF;
          EXIT WHEN "j" = "dimension_v";
          "j" := "j" + 1;
        END LOOP;
        EXIT WHEN "i" = "dimension_v" - 1;
        "i" := "i" + 1;
      END LOOP;
      -- tie-breaking by forbidding shared weakest links in beat-paths
      -- (unless "tie_breaking" is set to 'simple', in which case tie-breaking
      -- is performed later by initiative id):
      IF "policy_row"."tie_breaking" != 'simple'::"tie_breaking" THEN
        "m" := 1;
        LOOP
          "n" := "m" + 1;
          LOOP
            -- only process those candidates m and n, which are tied:
            IF "matrix_b"["m"]["n"] ISNULL THEN
              -- start with beat-paths prior tie-breaking:
              "matrix_t" := "matrix_p";
              -- start with all links allowed:
              "matrix_f" := array_fill(FALSE, ARRAY["dimension_v", "dimension_v"]);
              LOOP
                -- determine (and forbid) that link that is the weakest link
                -- in both the best path from candidate m to candidate n and
                -- from candidate n to candidate m:
                "i" := 1;
                <<forbid_one_link>>
                LOOP
                  "j" := 1;
                  LOOP
                    IF "i" != "j" THEN
                      IF "matrix_d"["i"]["j"] = "matrix_t"["m"]["n"] THEN
                        "matrix_f"["i"]["j"] := TRUE;
                        -- exit for performance reasons,
                        -- as exactly one link will be found:
                        EXIT forbid_one_link;
                      END IF;
                    END IF;
                    EXIT WHEN "j" = "dimension_v";
                    "j" := "j" + 1;
                  END LOOP;
                  IF "i" = "dimension_v" THEN
                    RAISE EXCEPTION 'Did not find shared weakest link for tie-breaking (should not happen)';
                  END IF;
                  "i" := "i" + 1;
                END LOOP;
                -- calculate best beat-paths while ignoring forbidden links:
                "i" := 1;
                LOOP
                  "j" := 1;
                  LOOP
                    IF "i" != "j" THEN
                      "matrix_t"["i"]["j"] := CASE
                         WHEN "matrix_f"["i"]["j"]
                         THEN ((-1::INT8) << 63, 0)::"link_strength"  -- worst possible value
                         ELSE "matrix_d"["i"]["j"] END;
                    END IF;
                    EXIT WHEN "j" = "dimension_v";
                    "j" := "j" + 1;
                  END LOOP;
                  EXIT WHEN "i" = "dimension_v";
                  "i" := "i" + 1;
                END LOOP;
                "matrix_t" := "find_best_paths"("matrix_t");
                -- extend partial order, if tie-breaking was successful:
                IF "matrix_t"["m"]["n"] > "matrix_t"["n"]["m"] THEN
                  "matrix_b"["m"]["n"] := TRUE;
                  "matrix_b"["n"]["m"] := FALSE;
                  EXIT;
                ELSIF "matrix_t"["m"]["n"] < "matrix_t"["n"]["m"] THEN
                  "matrix_b"["m"]["n"] := FALSE;
                  "matrix_b"["n"]["m"] := TRUE;
                  EXIT;
                END IF;
              END LOOP;
            END IF;
            EXIT WHEN "n" = "dimension_v";
            "n" := "n" + 1;
          END LOOP;
          EXIT WHEN "m" = "dimension_v" - 1;
          "m" := "m" + 1;
        END LOOP;
      END IF;
      -- store a unique ranking in "rank_ary":
      "rank_ary" := array_fill(NULL::INT4, ARRAY["dimension_v"]);
      "rank_v" := 1;
      LOOP
        "i" := 1;
        <<assign_next_rank>>
        LOOP
          IF "rank_ary"["i"] ISNULL THEN
            "j" := 1;
            LOOP
              IF
                "i" != "j" AND
                "rank_ary"["j"] ISNULL AND
                ( "matrix_b"["j"]["i"] OR
                  -- tie-breaking by "id"
                  ( "matrix_b"["j"]["i"] ISNULL AND
                    "j" < "i" ) )
              THEN
                -- someone else is better
                EXIT;
              END IF;
              IF "j" = "dimension_v" THEN
                -- noone is better
                "rank_ary"["i"] := "rank_v";
                EXIT assign_next_rank;
              END IF;
              "j" := "j" + 1;
            END LOOP;
          END IF;
          "i" := "i" + 1;
          IF "i" > "dimension_v" THEN
            RAISE EXCEPTION 'Schulze ranking does not compute (should not happen)';
          END IF;
        END LOOP;
        EXIT WHEN "rank_v" = "dimension_v";
        "rank_v" := "rank_v" + 1;
      END LOOP;
      -- write preliminary results:
      "i" := 2;  -- omit status quo with "i" = 1
      FOR "initiative_id_v" IN
        SELECT "id" FROM "initiative"
        WHERE "issue_id" = "issue_id_p" AND "admitted"
        ORDER BY "id"
      LOOP
        UPDATE "initiative" SET
          "direct_majority" =
            CASE WHEN "policy_row"."direct_majority_strict" THEN
              "positive_votes" * "policy_row"."direct_majority_den" >
              "policy_row"."direct_majority_num" * ("positive_votes"+"negative_votes")
            ELSE
              "positive_votes" * "policy_row"."direct_majority_den" >=
              "policy_row"."direct_majority_num" * ("positive_votes"+"negative_votes")
            END
            AND "positive_votes" >= "policy_row"."direct_majority_positive"
            AND "issue_row"."voter_count"-"negative_votes" >=
                "policy_row"."direct_majority_non_negative",
            "indirect_majority" =
            CASE WHEN "policy_row"."indirect_majority_strict" THEN
              "positive_votes" * "policy_row"."indirect_majority_den" >
              "policy_row"."indirect_majority_num" * ("positive_votes"+"negative_votes")
            ELSE
              "positive_votes" * "policy_row"."indirect_majority_den" >=
              "policy_row"."indirect_majority_num" * ("positive_votes"+"negative_votes")
            END
            AND "positive_votes" >= "policy_row"."indirect_majority_positive"
            AND "issue_row"."voter_count"-"negative_votes" >=
                "policy_row"."indirect_majority_non_negative",
          "schulze_rank"           = "rank_ary"["i"],
          "better_than_status_quo" = "rank_ary"["i"] < "rank_ary"[1],
          "worse_than_status_quo"  = "rank_ary"["i"] > "rank_ary"[1],
          "multistage_majority"    = "rank_ary"["i"] >= "rank_ary"[1],
          "reverse_beat_path"      = CASE WHEN "policy_row"."defeat_strength" = 'simple'::"defeat_strength"
                                     THEN NULL
                                     ELSE "matrix_p"[1]["i"]."primary" >= 0 END,
          "eligible"               = FALSE,
          "winner"                 = FALSE,
          "rank"                   = NULL  -- NOTE: in cases of manual reset of issue state
          WHERE "id" = "initiative_id_v";
        "i" := "i" + 1;
      END LOOP;
      IF "i" != "dimension_v" + 1 THEN
        RAISE EXCEPTION 'Wrong winner count (should not happen)';
      END IF;
      -- take indirect majorities into account:
      LOOP
        UPDATE "initiative" SET "indirect_majority" = TRUE
          FROM (
            SELECT "new_initiative"."id" AS "initiative_id"
            FROM "initiative" "old_initiative"
            JOIN "initiative" "new_initiative"
              ON "new_initiative"."issue_id" = "issue_id_p"
              AND "new_initiative"."indirect_majority" = FALSE
            JOIN "battle" "battle_win"
              ON "battle_win"."issue_id" = "issue_id_p"
              AND "battle_win"."winning_initiative_id" = "new_initiative"."id"
              AND "battle_win"."losing_initiative_id" = "old_initiative"."id"
            JOIN "battle" "battle_lose"
              ON "battle_lose"."issue_id" = "issue_id_p"
              AND "battle_lose"."losing_initiative_id" = "new_initiative"."id"
              AND "battle_lose"."winning_initiative_id" = "old_initiative"."id"
            WHERE "old_initiative"."issue_id" = "issue_id_p"
            AND "old_initiative"."indirect_majority" = TRUE
            AND CASE WHEN "policy_row"."indirect_majority_strict" THEN
              "battle_win"."count" * "policy_row"."indirect_majority_den" >
              "policy_row"."indirect_majority_num" *
              ("battle_win"."count"+"battle_lose"."count")
            ELSE
              "battle_win"."count" * "policy_row"."indirect_majority_den" >=
              "policy_row"."indirect_majority_num" *
              ("battle_win"."count"+"battle_lose"."count")
            END
            AND "battle_win"."count" >= "policy_row"."indirect_majority_positive"
            AND "issue_row"."voter_count"-"battle_lose"."count" >=
                "policy_row"."indirect_majority_non_negative"
          ) AS "subquery"
          WHERE "id" = "subquery"."initiative_id";
        EXIT WHEN NOT FOUND;
      END LOOP;
      -- set "multistage_majority" for remaining matching initiatives:
      UPDATE "initiative" SET "multistage_majority" = TRUE
        FROM (
          SELECT "losing_initiative"."id" AS "initiative_id"
          FROM "initiative" "losing_initiative"
          JOIN "initiative" "winning_initiative"
            ON "winning_initiative"."issue_id" = "issue_id_p"
            AND "winning_initiative"."admitted"
          JOIN "battle" "battle_win"
            ON "battle_win"."issue_id" = "issue_id_p"
            AND "battle_win"."winning_initiative_id" = "winning_initiative"."id"
            AND "battle_win"."losing_initiative_id" = "losing_initiative"."id"
          JOIN "battle" "battle_lose"
            ON "battle_lose"."issue_id" = "issue_id_p"
            AND "battle_lose"."losing_initiative_id" = "winning_initiative"."id"
            AND "battle_lose"."winning_initiative_id" = "losing_initiative"."id"
          WHERE "losing_initiative"."issue_id" = "issue_id_p"
          AND "losing_initiative"."admitted"
          AND "winning_initiative"."schulze_rank" <
              "losing_initiative"."schulze_rank"
          AND "battle_win"."count" > "battle_lose"."count"
          AND (
            "battle_win"."count" > "winning_initiative"."positive_votes" OR
            "battle_lose"."count" < "losing_initiative"."negative_votes" )
        ) AS "subquery"
        WHERE "id" = "subquery"."initiative_id";
      -- mark eligible initiatives:
      UPDATE "initiative" SET "eligible" = TRUE
        WHERE "issue_id" = "issue_id_p"
        AND "initiative"."direct_majority"
        AND "initiative"."indirect_majority"
        AND "initiative"."better_than_status_quo"
        AND (
          "policy_row"."no_multistage_majority" = FALSE OR
          "initiative"."multistage_majority" = FALSE )
        AND (
          "policy_row"."no_reverse_beat_path" = FALSE OR
          coalesce("initiative"."reverse_beat_path", FALSE) = FALSE );
      -- mark final winner:
      UPDATE "initiative" SET "winner" = TRUE
        FROM (
          SELECT "id" AS "initiative_id"
          FROM "initiative"
          WHERE "issue_id" = "issue_id_p" AND "eligible"
          ORDER BY
            "schulze_rank",
            "id"
          LIMIT 1
        ) AS "subquery"
        WHERE "id" = "subquery"."initiative_id";
      -- write (final) ranks:
      "rank_v" := 1;
      FOR "initiative_id_v" IN
        SELECT "id"
        FROM "initiative"
        WHERE "issue_id" = "issue_id_p" AND "admitted"
        ORDER BY
          "winner" DESC,
          "eligible" DESC,
          "schulze_rank",
          "id"
      LOOP
        UPDATE "initiative" SET "rank" = "rank_v"
          WHERE "id" = "initiative_id_v";
        "rank_v" := "rank_v" + 1;
      END LOOP;
      -- set schulze rank of status quo and mark issue as finished:
      UPDATE "issue" SET
        "status_quo_schulze_rank" = "rank_ary"[1],
        "state" =
          CASE WHEN EXISTS (
            SELECT NULL FROM "initiative"
            WHERE "issue_id" = "issue_id_p" AND "winner"
          ) THEN
            'finished_with_winner'::"issue_state"
          ELSE
            'finished_without_winner'::"issue_state"
          END,
        "closed" = "phase_finished",
        "phase_finished" = NULL
        WHERE "id" = "issue_id_p";
      RETURN;
    END;
  $$;

COMMENT ON FUNCTION "calculate_ranks"
  ( "issue"."id"%TYPE )
  IS 'Determine ranking (Votes have to be counted first)';



-----------------------------
-- Automatic state changes --
-----------------------------


CREATE TYPE "check_issue_persistence" AS (
        "state"                 "issue_state",
        "phase_finished"        BOOLEAN,
        "issue_revoked"         BOOLEAN,
        "snapshot_created"      BOOLEAN,
        "harmonic_weights_set"  BOOLEAN,
        "closed_voting"         BOOLEAN );

COMMENT ON TYPE "check_issue_persistence" IS 'Type of data returned by "check_issue" function, to be passed to subsequent calls of the same function';


CREATE FUNCTION "check_issue"
  ( "issue_id_p" "issue"."id"%TYPE,
    "persist"    "check_issue_persistence" )
  RETURNS "check_issue_persistence"
  LANGUAGE 'plpgsql' VOLATILE AS $$
    DECLARE
      "issue_row"      "issue"%ROWTYPE;
      "policy_row"     "policy"%ROWTYPE;
      "initiative_row" "initiative"%ROWTYPE;
      "state_v"        "issue_state";
    BEGIN
      PERFORM "require_transaction_isolation"();
      IF "persist" ISNULL THEN
        SELECT * INTO "issue_row" FROM "issue" WHERE "id" = "issue_id_p"
          FOR UPDATE;
        IF "issue_row"."closed" NOTNULL THEN
          RETURN NULL;
        END IF;
        "persist"."state" := "issue_row"."state";
        IF
          ( "issue_row"."state" = 'admission' AND now() >=
            "issue_row"."created" + "issue_row"."max_admission_time" ) OR
          ( "issue_row"."state" = 'discussion' AND now() >=
            "issue_row"."accepted" + "issue_row"."discussion_time" ) OR
          ( "issue_row"."state" = 'verification' AND now() >=
            "issue_row"."half_frozen" + "issue_row"."verification_time" ) OR
          ( "issue_row"."state" = 'voting' AND now() >=
            "issue_row"."fully_frozen" + "issue_row"."voting_time" )
        THEN
          "persist"."phase_finished" := TRUE;
        ELSE
          "persist"."phase_finished" := FALSE;
        END IF;
        IF
          NOT EXISTS (
            -- all initiatives are revoked
            SELECT NULL FROM "initiative"
            WHERE "issue_id" = "issue_id_p" AND "revoked" ISNULL
          ) AND (
            -- and issue has not been accepted yet
            "persist"."state" = 'admission' OR
            -- or verification time has elapsed
            ( "persist"."state" = 'verification' AND
              "persist"."phase_finished" ) OR
            -- or no initiatives have been revoked lately
            NOT EXISTS (
              SELECT NULL FROM "initiative"
              WHERE "issue_id" = "issue_id_p"
              AND now() < "revoked" + "issue_row"."verification_time"
            )
          )
        THEN
          "persist"."issue_revoked" := TRUE;
        ELSE
          "persist"."issue_revoked" := FALSE;
        END IF;
        IF "persist"."phase_finished" OR "persist"."issue_revoked" THEN
          UPDATE "issue" SET "phase_finished" = now()
            WHERE "id" = "issue_row"."id";
          RETURN "persist";
        ELSIF
          "persist"."state" IN ('admission', 'discussion', 'verification')
        THEN
          RETURN "persist";
        ELSE
          RETURN NULL;
        END IF;
      END IF;
      IF
        "persist"."state" IN ('admission', 'discussion', 'verification') AND
        coalesce("persist"."snapshot_created", FALSE) = FALSE
      THEN
        PERFORM "create_snapshot"("issue_id_p");
        "persist"."snapshot_created" = TRUE;
        IF "persist"."phase_finished" THEN
          IF "persist"."state" = 'admission' THEN
            PERFORM "set_snapshot_event"("issue_id_p", 'end_of_admission');
          ELSIF "persist"."state" = 'discussion' THEN
            PERFORM "set_snapshot_event"("issue_id_p", 'half_freeze');
          ELSIF "persist"."state" = 'verification' THEN
            PERFORM "set_snapshot_event"("issue_id_p", 'full_freeze');
            SELECT * INTO "issue_row" FROM "issue" WHERE "id" = "issue_id_p";
            SELECT * INTO "policy_row" FROM "policy"
              WHERE "id" = "issue_row"."policy_id";
            FOR "initiative_row" IN
              SELECT * FROM "initiative"
              WHERE "issue_id" = "issue_id_p" AND "revoked" ISNULL
              FOR UPDATE
            LOOP
              IF
                "initiative_row"."polling" OR (
                  "initiative_row"."satisfied_supporter_count" > 0 AND
                  "initiative_row"."satisfied_supporter_count" *
                  "policy_row"."initiative_quorum_den" >=
                  "issue_row"."population" * "policy_row"."initiative_quorum_num"
                )
              THEN
                UPDATE "initiative" SET "admitted" = TRUE
                  WHERE "id" = "initiative_row"."id";
              ELSE
                UPDATE "initiative" SET "admitted" = FALSE
                  WHERE "id" = "initiative_row"."id";
              END IF;
            END LOOP;
          END IF;
        END IF;
        RETURN "persist";
      END IF;
      IF
        "persist"."state" IN ('admission', 'discussion', 'verification') AND
        coalesce("persist"."harmonic_weights_set", FALSE) = FALSE
      THEN
        PERFORM "set_harmonic_initiative_weights"("issue_id_p");
        "persist"."harmonic_weights_set" = TRUE;
        IF
          "persist"."phase_finished" OR
          "persist"."issue_revoked" OR
          "persist"."state" = 'admission'
        THEN
          RETURN "persist";
        ELSE
          RETURN NULL;
        END IF;
      END IF;
      IF "persist"."issue_revoked" THEN
        IF "persist"."state" = 'admission' THEN
          "state_v" := 'canceled_revoked_before_accepted';
        ELSIF "persist"."state" = 'discussion' THEN
          "state_v" := 'canceled_after_revocation_during_discussion';
        ELSIF "persist"."state" = 'verification' THEN
          "state_v" := 'canceled_after_revocation_during_verification';
        END IF;
        UPDATE "issue" SET
          "state"          = "state_v",
          "closed"         = "phase_finished",
          "phase_finished" = NULL
          WHERE "id" = "issue_id_p";
        RETURN NULL;
      END IF;
      IF "persist"."state" = 'admission' THEN
        SELECT * INTO "issue_row" FROM "issue" WHERE "id" = "issue_id_p"
          FOR UPDATE;
        SELECT * INTO "policy_row"
          FROM "policy" WHERE "id" = "issue_row"."policy_id";
        IF 
          ( now() >=
            "issue_row"."created" + "issue_row"."min_admission_time" ) AND
          EXISTS (
            SELECT NULL FROM "initiative"
            WHERE "issue_id" = "issue_id_p"
            AND "supporter_count" > 0
            AND "supporter_count" * "policy_row"."issue_quorum_den"
            >= "issue_row"."population" * "policy_row"."issue_quorum_num"
          )
        THEN
          UPDATE "issue" SET
            "state"          = 'discussion',
            "accepted"       = coalesce("phase_finished", now()),
            "phase_finished" = NULL
            WHERE "id" = "issue_id_p";
        ELSIF "issue_row"."phase_finished" NOTNULL THEN
          UPDATE "issue" SET
            "state"          = 'canceled_issue_not_accepted',
            "closed"         = "phase_finished",
            "phase_finished" = NULL
            WHERE "id" = "issue_id_p";
        END IF;
        RETURN NULL;
      END IF;
      IF "persist"."phase_finished" THEN
        IF "persist"."state" = 'discussion' THEN
          UPDATE "issue" SET
            "state"          = 'verification',
            "half_frozen"    = "phase_finished",
            "phase_finished" = NULL
            WHERE "id" = "issue_id_p";
          RETURN NULL;
        END IF;
        IF "persist"."state" = 'verification' THEN
          SELECT * INTO "issue_row" FROM "issue" WHERE "id" = "issue_id_p"
            FOR UPDATE;
          SELECT * INTO "policy_row" FROM "policy"
            WHERE "id" = "issue_row"."policy_id";
          IF EXISTS (
            SELECT NULL FROM "initiative"
            WHERE "issue_id" = "issue_id_p" AND "admitted" = TRUE
          ) THEN
            UPDATE "issue" SET
              "state"          = 'voting',
              "fully_frozen"   = "phase_finished",
              "phase_finished" = NULL
              WHERE "id" = "issue_id_p";
          ELSE
            UPDATE "issue" SET
              "state"          = 'canceled_no_initiative_admitted',
              "fully_frozen"   = "phase_finished",
              "closed"         = "phase_finished",
              "phase_finished" = NULL
              WHERE "id" = "issue_id_p";
            -- NOTE: The following DELETE statements have effect only when
            --       issue state has been manipulated
            DELETE FROM "direct_voter"     WHERE "issue_id" = "issue_id_p";
            DELETE FROM "delegating_voter" WHERE "issue_id" = "issue_id_p";
            DELETE FROM "battle"           WHERE "issue_id" = "issue_id_p";
          END IF;
          RETURN NULL;
        END IF;
        IF "persist"."state" = 'voting' THEN
          IF coalesce("persist"."closed_voting", FALSE) = FALSE THEN
            PERFORM "close_voting"("issue_id_p");
            "persist"."closed_voting" = TRUE;
            RETURN "persist";
          END IF;
          PERFORM "calculate_ranks"("issue_id_p");
          RETURN NULL;
        END IF;
      END IF;
      RAISE WARNING 'should not happen';
      RETURN NULL;
    END;
  $$;

COMMENT ON FUNCTION "check_issue"
  ( "issue"."id"%TYPE,
    "check_issue_persistence" )
  IS 'Precalculate supporter counts etc. for a given issue, and check, if status change is required, and perform the status change when necessary; Function must be called multiple times with the previous result as second parameter, until the result is NULL (see source code of function "check_everything")';


CREATE FUNCTION "check_everything"()
  RETURNS VOID
  LANGUAGE 'plpgsql' VOLATILE AS $$
    DECLARE
      "issue_id_v" "issue"."id"%TYPE;
      "persist_v"  "check_issue_persistence";
    BEGIN
      RAISE WARNING 'Function "check_everything" should only be used for development and debugging purposes';
      DELETE FROM "expired_session";
      PERFORM "check_activity"();
      PERFORM "calculate_member_counts"();
      FOR "issue_id_v" IN SELECT "id" FROM "open_issue" LOOP
        "persist_v" := NULL;
        LOOP
          "persist_v" := "check_issue"("issue_id_v", "persist_v");
          EXIT WHEN "persist_v" ISNULL;
        END LOOP;
      END LOOP;
      RETURN;
    END;
  $$;

COMMENT ON FUNCTION "check_everything"() IS 'Amongst other regular tasks this function performs "check_issue" for every open issue. Use this function only for development and debugging purposes, as you may run into locking and/or serialization problems in productive environments.';



----------------------
-- Deletion of data --
----------------------


CREATE FUNCTION "clean_issue"("issue_id_p" "issue"."id"%TYPE)
  RETURNS VOID
  LANGUAGE 'plpgsql' VOLATILE AS $$
    BEGIN
      IF EXISTS (
        SELECT NULL FROM "issue" WHERE "id" = "issue_id_p" AND "cleaned" ISNULL
      ) THEN
        -- override protection triggers:
        INSERT INTO "temporary_transaction_data" ("key", "value")
          VALUES ('override_protection_triggers', TRUE::TEXT);
        -- clean data:
        DELETE FROM "delegating_voter"
          WHERE "issue_id" = "issue_id_p";
        DELETE FROM "direct_voter"
          WHERE "issue_id" = "issue_id_p";
        DELETE FROM "delegating_interest_snapshot"
          WHERE "issue_id" = "issue_id_p";
        DELETE FROM "direct_interest_snapshot"
          WHERE "issue_id" = "issue_id_p";
        DELETE FROM "delegating_population_snapshot"
          WHERE "issue_id" = "issue_id_p";
        DELETE FROM "direct_population_snapshot"
          WHERE "issue_id" = "issue_id_p";
        DELETE FROM "non_voter"
          WHERE "issue_id" = "issue_id_p";
        DELETE FROM "delegation"
          WHERE "issue_id" = "issue_id_p";
        DELETE FROM "supporter"
          USING "initiative"  -- NOTE: due to missing index on issue_id
          WHERE "initiative"."issue_id" = "issue_id_p"
          AND "supporter"."initiative_id" = "initiative_id";
        -- mark issue as cleaned:
        UPDATE "issue" SET "cleaned" = now() WHERE "id" = "issue_id_p";
        -- finish overriding protection triggers (avoids garbage):
        DELETE FROM "temporary_transaction_data"
          WHERE "key" = 'override_protection_triggers';
      END IF;
      RETURN;
    END;
  $$;

COMMENT ON FUNCTION "clean_issue"("issue"."id"%TYPE) IS 'Delete discussion data and votes belonging to an issue';


CREATE FUNCTION "delete_member"("member_id_p" "member"."id"%TYPE)
  RETURNS VOID
  LANGUAGE 'plpgsql' VOLATILE AS $$
    BEGIN
      UPDATE "member" SET
        "last_login"                   = NULL,
        "last_delegation_check"        = NULL,
        "login"                        = NULL,
        "password"                     = NULL,
        "authority"                    = NULL,
        "authority_uid"                = NULL,
        "authority_login"              = NULL,
        "locked"                       = TRUE,
        "active"                       = FALSE,
        "notify_email"                 = NULL,
        "notify_email_unconfirmed"     = NULL,
        "notify_email_secret"          = NULL,
        "notify_email_secret_expiry"   = NULL,
        "notify_email_lock_expiry"     = NULL,
        "disable_notifications"        = TRUE,
        "notification_counter"         = DEFAULT,
        "notification_sample_size"     = 0,
        "notification_dow"             = NULL,
        "notification_hour"            = NULL,
        "login_recovery_expiry"        = NULL,
        "password_reset_secret"        = NULL,
        "password_reset_secret_expiry" = NULL,
        "organizational_unit"          = NULL,
        "internal_posts"               = NULL,
        "realname"                     = NULL,
        "birthday"                     = NULL,
        "address"                      = NULL,
        "email"                        = NULL,
        "xmpp_address"                 = NULL,
        "website"                      = NULL,
        "phone"                        = NULL,
        "mobile_phone"                 = NULL,
        "profession"                   = NULL,
        "external_memberships"         = NULL,
        "external_posts"               = NULL,
        "statement"                    = NULL
        WHERE "id" = "member_id_p";
      -- "text_search_data" is updated by triggers
      DELETE FROM "setting"            WHERE "member_id" = "member_id_p";
      DELETE FROM "setting_map"        WHERE "member_id" = "member_id_p";
      DELETE FROM "member_relation_setting" WHERE "member_id" = "member_id_p";
      DELETE FROM "member_image"       WHERE "member_id" = "member_id_p";
      DELETE FROM "contact"            WHERE "member_id" = "member_id_p";
      DELETE FROM "ignored_member"     WHERE "member_id" = "member_id_p";
      DELETE FROM "session"            WHERE "member_id" = "member_id_p";
      DELETE FROM "area_setting"       WHERE "member_id" = "member_id_p";
      DELETE FROM "issue_setting"      WHERE "member_id" = "member_id_p";
      DELETE FROM "ignored_initiative" WHERE "member_id" = "member_id_p";
      DELETE FROM "initiative_setting" WHERE "member_id" = "member_id_p";
      DELETE FROM "suggestion_setting" WHERE "member_id" = "member_id_p";
      DELETE FROM "membership"         WHERE "member_id" = "member_id_p";
      DELETE FROM "delegation"         WHERE "truster_id" = "member_id_p";
      DELETE FROM "non_voter"          WHERE "member_id" = "member_id_p";
      DELETE FROM "direct_voter" USING "issue"
        WHERE "direct_voter"."issue_id" = "issue"."id"
        AND "issue"."closed" ISNULL
        AND "member_id" = "member_id_p";
      RETURN;
    END;
  $$;

COMMENT ON FUNCTION "delete_member"("member_id_p" "member"."id"%TYPE) IS 'Deactivate member and clear certain settings and data of this member (data protection)';


CREATE FUNCTION "delete_private_data"()
  RETURNS VOID
  LANGUAGE 'plpgsql' VOLATILE AS $$
    BEGIN
      DELETE FROM "temporary_transaction_data";
      DELETE FROM "member" WHERE "activated" ISNULL;
      UPDATE "member" SET
        "invite_code"                  = NULL,
        "invite_code_expiry"           = NULL,
        "admin_comment"                = NULL,
        "last_login"                   = NULL,
        "last_delegation_check"        = NULL,
        "login"                        = NULL,
        "password"                     = NULL,
        "authority"                    = NULL,
        "authority_uid"                = NULL,
        "authority_login"              = NULL,
        "lang"                         = NULL,
        "notify_email"                 = NULL,
        "notify_email_unconfirmed"     = NULL,
        "notify_email_secret"          = NULL,
        "notify_email_secret_expiry"   = NULL,
        "notify_email_lock_expiry"     = NULL,
        "disable_notifications"        = TRUE,
        "notification_counter"         = DEFAULT,
        "notification_sample_size"     = 0,
        "notification_dow"             = NULL,
        "notification_hour"            = NULL,
        "login_recovery_expiry"        = NULL,
        "password_reset_secret"        = NULL,
        "password_reset_secret_expiry" = NULL,
        "organizational_unit"          = NULL,
        "internal_posts"               = NULL,
        "realname"                     = NULL,
        "birthday"                     = NULL,
        "address"                      = NULL,
        "email"                        = NULL,
        "xmpp_address"                 = NULL,
        "website"                      = NULL,
        "phone"                        = NULL,
        "mobile_phone"                 = NULL,
        "profession"                   = NULL,
        "external_memberships"         = NULL,
        "external_posts"               = NULL,
        "formatting_engine"            = NULL,
        "statement"                    = NULL;
      -- "text_search_data" is updated by triggers
      DELETE FROM "setting";
      DELETE FROM "setting_map";
      DELETE FROM "member_relation_setting";
      DELETE FROM "member_image";
      DELETE FROM "contact";
      DELETE FROM "ignored_member";
      DELETE FROM "session";
      DELETE FROM "area_setting";
      DELETE FROM "issue_setting";
      DELETE FROM "ignored_initiative";
      DELETE FROM "initiative_setting";
      DELETE FROM "suggestion_setting";
      DELETE FROM "non_voter";
      DELETE FROM "direct_voter" USING "issue"
        WHERE "direct_voter"."issue_id" = "issue"."id"
        AND "issue"."closed" ISNULL;
      RETURN;
    END;
  $$;

COMMENT ON FUNCTION "delete_private_data"() IS 'Used by lf_export script. DO NOT USE on productive database, but only on a copy! This function deletes all data which should not be publicly available, and can be used to create a database dump for publication. See source code to see which data is deleted. If you need a different behaviour, copy this function and modify lf_export accordingly, to avoid data-leaks after updating.';



COMMIT;
