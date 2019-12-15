CREATE FUNCTION "time_warp"(warp_interval INTERVAL) RETURNS VOID
  LANGUAGE 'plpgsql' VOLATILE AS $$
    BEGIN
      UPDATE "issue" SET
        "snapshot"     = "snapshot"     - warp_interval,
        "created"      = "created"      - warp_interval,
        "accepted"     = "accepted"     - warp_interval,
        "half_frozen"  = "half_frozen"  - warp_interval,
        "fully_frozen" = "fully_frozen" - warp_interval;
      PERFORM "check_everything"();
      RETURN;
    END;
  $$;
