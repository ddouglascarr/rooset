
BEGIN;

-- NOTE: This setting must be set using SQL to bootstrap the system. See INSTALL.mkd
INSERT INTO "system_setting" ("member_ttl") VALUES ('31 days');


-- NOTE: thre must be some of these, and there's no mechanism to set the up with lua
INSERT INTO "contingent" ("polling", "time_frame", "text_entry_limit", "initiative_limit") VALUES
  (FALSE, '60 minutes', 6, 10),
  (FALSE, '1 day', 60, 15),
  (FALSE, '1 week', 120, 20),
  (TRUE, '60 minutes', 6, 1),
  (TRUE, '1 day', 60, 10),
  (TRUE, '1 week', 120, 20);

COMMIT;
