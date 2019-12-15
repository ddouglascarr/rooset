-- NOTE: This file requires that sequence generators have not been used.
-- (All new rows need to start with id '1'.)

-- set transaction isolation level to be able to call "check_everything"() function


INSERT INTO "system_setting" ("member_ttl") VALUES ('31 days');

INSERT INTO "contingent" ("polling", "time_frame", "text_entry_limit", "initiative_limit") VALUES
  (FALSE, '60 minutes', 6, 10),
  (FALSE, '1 day', 60, 15),
  (FALSE, '1 week', 120, 20),
  (TRUE, '60 minutes', 6, 1),
  (TRUE, '1 day', 60, 10),
  (TRUE, '1 week', 120, 20);

INSERT INTO "member" ("id", "activated", "last_activity", "active", "login", "name") VALUES
  (1,  'now', 'now', TRUE, 'determined_poitras',  'Determined Poitras'),  -- id  1
  (2,  'now', 'now', TRUE, 'tender_hugle',  'Tender Hugle'),              -- id  2
  (3,  'now', 'now', TRUE, 'dreamy_almeida',  'Dreamy Almeida'),          -- id  3
  (4,  'now', 'now', TRUE, 'thirsty_swirles',  'Thirsty Swirles'),        -- id  4
  (5,  'now', 'now', TRUE, 'goofy_heisenberg',  'Goofy Heisenberg'),      -- id  5
  (6,  'now', 'now', TRUE, 'thirsty_babbage',  'Thirsty Babbage'),        -- id  6
  (7,  'now', 'now', TRUE, 'sick_lamarr',  'Sick Lamarr'),                -- id  7
  (8,  'now', 'now', TRUE, 'admiring_sammet',  'Admiring Sammet'),        -- id  8
  (9,  'now', 'now', TRUE, 'compassionate_bose',  'Compassionate Bose'),  -- id  9
  (10, 'now', 'now', TRUE, 'fervent_wright',  'Fervent Wright'),          -- id 10
  (11, 'now', 'now', TRUE, 'focused_bell',  'Focused Bell'),              -- id 11
  (12, 'now', 'now', TRUE, 'elated_meninsky',  'Elated Meninsky'),        -- id 12
  (13, 'now', 'now', TRUE, 'romantic_carson',  'Romantic Carson'),        -- id 13
  (14, 'now', 'now', TRUE, 'admiring_bartik',  'Admiring Bartik'),        -- id 14
  (15, 'now', 'now', TRUE, 'evil_austin',  'Evil Austin'),                -- id 15
  (16, 'now', 'now', TRUE, 'mad_mcnulty',  'Mad Mcnulty'),                -- id 16
  (17, 'now', 'now', TRUE, 'insane_poincare',  'Insane Poincare'),        -- id 17
  (18, 'now', 'now', TRUE, 'jovial_blackwell',  'Jovial Blackwell'),      -- id 18
  (19, 'now', 'now', TRUE, 'goofy_khorana',  'Goofy Khorana'),            -- id 19
  (20, 'now', 'now', TRUE, 'kickass_fermat',  'Kickass Fermat'),          -- id 20
  (21, 'now', 'now', TRUE, 'drunk_saha',  'Drunk Saha'),                  -- id 21
  (22, 'now', 'now', TRUE, 'angry_ritchie',  'Angry Ritchie'),            -- id 22
  (23, 'now', 'now', TRUE, 'desperate_easley',  'Desperate Easley'),      -- id 23
  (24, 'now', 'now', TRUE, 'admin', 'Admin #1' );                         -- id 24

-- set password to "login"
UPDATE "member" SET "password" = '$1$PcI6b1Bg$2SHjAZH2nMLFp0fxHis.Q0';
UPDATE "member" SET admin = TRUE WHERE login = 'admin';

INSERT INTO "policy" (
    "id",
    "index",
    "name",
    "min_admission_time", "max_admission_time",
    "discussion_time",
    "verification_time",
    "voting_time",
    "issue_quorum_num", "issue_quorum_den",
    "initiative_quorum_num", "initiative_quorum_den",
    "direct_majority_num", "direct_majority_den", "direct_majority_strict",
    "indirect_majority_num", "indirect_majority_den", "indirect_majority_strict",
    "no_reverse_beat_path", "no_multistage_majority"
  ) VALUES (
    1,
    1,
    'amendment of the statutes (solar system)',
    '0', '8 days', '15 days', '8 days', '15 days',
    10, 100,
    10, 100,
    1, 2, TRUE,
    2, 3, FALSE,
    TRUE, FALSE
  ), (
    2,
    2,
    'amendment of the statutes (earth moon federation)',
    '0', '8 days', '15 days', '8 days', '15 days',
    10, 100,
    10, 100,
    1, 2, TRUE,
    2, 3, FALSE,
    TRUE, FALSE
  ), (
    3,
    3,
    'amendment of the statutes (united mars colonies)',
    '0', '8 days', '15 days', '8 days', '15 days',
    10, 100,
    10, 100,
    1, 2, TRUE,
    2, 3, FALSE,
    TRUE, FALSE
  ), (
    4,
    4,
    'proposition',
    '0', '8 days', '15 days', '8 days', '15 days',
    10, 100,
    10, 100,
    1, 2, TRUE,
    1, 2, TRUE,
    TRUE, FALSE
  ), (
    5,
    5,
    'non-binding survey',
    '0', '2 days', '3 days', '2 days', '3 days',
    5, 100,
    5, 100,
    1, 2, TRUE,
    1, 2, TRUE,
    TRUE, FALSE
  ), (
    6,
    6,
    'non-binding survey (super fast)',
    '0', '1 hour', '30 minutes', '15 minutes', '30 minutes',
    5, 100,
    5, 100,
    1, 2, TRUE,
    1, 2, TRUE,
    TRUE, FALSE
  );

INSERT INTO "unit" ("id", "parent_id", "name") VALUES
  ( 1, NULL, 'Solar System'),           -- id 1
  ( 2, 1   , 'Earth Moon Federation'),  -- id 2
  ( 3, 2   , 'Earth'),                  -- id 3
  ( 4, 2   , 'Moon'),                   -- id 4
  ( 5, 1   , 'Mars');                   -- id 5

INSERT INTO "area" ("id", "unit_id", "name") VALUES
  ( 1, 1, 'Statutes of the United Solar System'),       -- id  1
  ( 2, 2, 'Statutes of the Earth Moon Federation'),     -- id  2
  ( 3, 5, 'Statutes of the United Mars Colonies'),      -- id  3
  ( 4, 1, 'Intra solar space travel'),                  -- id  4
  ( 5, 1, 'Intra solar system trade and taxation'),     -- id  5
  ( 6, 1, 'Comet defense and black holes management'),  -- id  6
  ( 7, 1, 'Alien affairs'),                             -- id  7
  ( 8, 2, 'Foreign affairs'),                           -- id  8
  ( 9, 3, 'Moon affairs'),                              -- id  9
  (10, 4, 'Earth affairs'),                             -- id 10
  (11, 4, 'Moon tourism'),                              -- id 11
  (12, 5, 'Foreign affairs'),                           -- id 12
  (13, 2, 'Department of space vehicles'),              -- id 13
  (14, 3, 'Environment'),                               -- id 14
  (15, 4, 'Energy and oxygen'),                         -- id 15
  (16, 5, 'Energy and oxygen'),                         -- id 16
  (17, 5, 'Mineral resources');                         -- id 17

INSERT INTO "allowed_policy" ("area_id", "policy_id", "default_policy") VALUES
  ( 1, 1, TRUE),
  ( 1, 5, FALSE),
  ( 1, 6, FALSE),
  ( 2, 2, TRUE),
  ( 2, 5, FALSE),
  ( 2, 6, FALSE),
  ( 3, 3, TRUE),
  ( 3, 5, FALSE),
  ( 3, 6, FALSE),
  ( 4, 4, TRUE),
  ( 4, 5, FALSE),
  ( 4, 6, FALSE),
  ( 5, 4, TRUE),
  ( 5, 5, FALSE),
  ( 5, 6, FALSE),
  ( 6, 4, TRUE),
  ( 6, 5, FALSE),
  ( 6, 6, FALSE),
  ( 7, 4, TRUE),
  ( 7, 5, FALSE),
  ( 7, 6, FALSE),
  ( 8, 4, TRUE),
  ( 8, 5, FALSE),
  ( 8, 6, FALSE),
  ( 9, 4, TRUE),
  ( 9, 5, FALSE),
  ( 9, 6, FALSE),
  (10, 4, TRUE),
  (10, 5, FALSE),
  (10, 6, FALSE),
  (11, 4, TRUE),
  (11, 5, FALSE),
  (11, 6, FALSE),
  (12, 4, TRUE),
  (12, 5, FALSE),
  (12, 6, FALSE),
  (13, 4, TRUE),
  (13, 5, FALSE),
  (13, 6, FALSE),
  (14, 4, TRUE),
  (14, 5, FALSE),
  (14, 6, FALSE),
  (15, 4, TRUE),
  (15, 5, FALSE),
  (15, 6, FALSE),
  (16, 4, TRUE),
  (16, 5, FALSE),
  (16, 6, FALSE),
  (17, 4, TRUE),
  (17, 5, FALSE),
  (17, 6, FALSE);

-- All Member are voting privileged for Unit 1 (Solar System)
INSERT INTO "privilege" ("unit_id", "member_id", "voting_right")
  SELECT 1 AS "unit_id", "id" AS "member_id", TRUE AS "voting_right"
  FROM "member"
  WHERE "member"."admin" != TRUE;

-- Members 1 - 15 are voting privileged for Unit 2 (Earth Moon Federation)
INSERT INTO "privilege" ("unit_id", "member_id", "voting_right")
  SELECT 2 AS "unit_id", "id" AS "member_id", TRUE AS "voting_right"
  FROM "member"
  WHERE "member"."id" > 0 AND "member"."id" < 16;

-- Members 1 - 10 are voting privileged for Unit 3 (Earth)
INSERT INTO "privilege" ("unit_id", "member_id", "voting_right")
  SELECT 3 AS "unit_id", "id" AS "member_id", TRUE AS "voting_right"
  FROM "member"
  WHERE "member"."id" > 0 AND "member"."id" < 11;

-- Members 10 - 15 are voting privileged for Unit 4 (Moon)
INSERT INTO "privilege" ("unit_id", "member_id", "voting_right")
  SELECT 4 AS "unit_id", "id" AS "member_id", TRUE AS "voting_right"
  FROM "member"
  WHERE "member"."id" > 10 AND "member"."id" < 16;

-- Members 16 - 23 are voting privileged for Unit 5 (Mars)
INSERT INTO "privilege" ("unit_id", "member_id", "voting_right")
  SELECT 5 AS "unit_id", "id" AS "member_id", TRUE AS "voting_right"
  FROM "member"
  WHERE "member"."id" > 15 AND "member"."id" < 24;

-- Solar System unit delegations
INSERT INTO "delegation"
    ("id", "truster_id", "scope", "unit_id", "trustee_id") VALUES
    ( 1, 2,  'unit', 1, 1),          -- Hugle delegates to Poitras
    ( 2, 3,  'unit', 1, 1),          -- Almeida delegates to Poitras
    ( 3, 4,  'unit', 1, 1),          -- Swirles delgates to Poitras
    ( 4, 5,  'unit', 1, 3),          -- Heisenberg delegates to Almeida
    ( 5, 6,  'unit', 1, 2),          -- Babbage delegates to Hugle
    ( 6, 8,  'unit', 1, 7),          -- Sammet delegates to Lamarr
    ( 7, 11, 'unit', 1, 1),          -- Bell delegates to Poitras
    ( 8, 12, 'unit', 1, 11),         -- Meminsky delegates to Bell
    ( 9, 17, 'unit', 1, 16),         -- Poincare delegates to McNulty
    (10, 18, 'unit', 1, 16);         -- Blackwell delegates to McNulty

-- Earth unit delegations
INSERT INTO "delegation"
    ("id", "truster_id", "scope", "unit_id", "trustee_id") VALUES
    (11, 1, 'unit', 2, 2),          -- Poitras delegates to Hugle
    (12, 3, 'unit', 2, 2),          -- Almeida delegates to Hugle
    (13, 5, 'unit', 2, 1);          -- Heisenberg delegates to Poitras

-- Solar System Alien Affairs area delegations
INSERT INTO "delegation"
    ("id", "truster_id", "scope", "area_id", "trustee_id") VALUES
    (14,  3,  'area', 7, 13),       -- Almeida delegates to Carson
    (15,  9,  'area', 7, 13),       -- Bose delegates to Carson
    (16,  19, 'area', 7, 5);        -- Khorana delegates to Heisenberg

-- Solar System Comet Defence area delegations
INSERT INTO "delegation"
    ("id", "truster_id", "scope", "area_id", "trustee_id") VALUES
    (17,  3,  'area', 6, 2);        -- Almeida delegates to Hugle


