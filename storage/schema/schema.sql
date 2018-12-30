BEGIN;



CREATE TABLE events_shard0000 (
  seq                           BIGSERIAL,
  aggregate_root_id             VARCHAR(512) NOT NULL,
  message_type                  VARCHAR(128) NOT NULL,
  message                       BYTEA NOT NULL
);

CREATE INDEX events_shard0000_seq
  ON events_shard0000 (seq);
CREATE INDEX events_shard0000_command_handler_lookup
  ON events_shard0000 (aggregate_root_id, seq);
CREATE INDEX events_shard0000_projections_lookup
  ON events_shard0000 (message_type, aggregate_root_id, seq);



CREATE TABLE event_processor_target_checkpoints (
  id                            VARCHAR(512) PRIMARY KEY,
  seq                           BIGINT
);

CREATE TABLE event_processor_source_checkpoints (
  id                            VARCHAR(512) PRIMARY KEY,
  seq                           BIGINT
);



CREATE TABLE users (
    id                          VARCHAR(512) NOT NULL UNIQUE,
    username                    VARCHAR(128) NOT NULL UNIQUE,
    password                    CHAR(60) NOT NULL,
    github_id                   VARCHAR(512) NOT NULL UNIQUE
);


--------------------------------------------------------------------------------
-- Github
--------------------------------------------------------------------------------


CREATE TABLE github_repository (
    github_id                   VARCHAR(128) NOT NULL UNIQUE,
    unit_id                     VARCHAR(512) NOT NULL UNIQUE,
    installation_id             BIGINT NOT NULL,
    owner                       VARCHAR(512) NOT NULL,
    name                        VARCHAR(512) NOT NULL
);

CREATE INDEX github_owner_name_lookup
    ON github_repository(owner, name);


COMMIT;
