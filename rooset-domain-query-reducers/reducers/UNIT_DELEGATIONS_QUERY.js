fromCategory("unitDelegations")
  .foreachStream()
  .when({

    $init: function() {
      return {
        unit: {
          trusteeId: null,
        },
        areas: []
      };
    },

    UNIT_DELEGATION_SET_EVENT: (s, e) => {
      const p = e.body.payload;
      s.unit.trusteeId = p.trusteeId;
    },

    UNIT_DELEGATION_UNSET_EVENT: (s, e) => {
      const p = e.body.payload;
      s.unit.trusteeId = null;
    }

  });
