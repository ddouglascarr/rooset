fromCategory("unitDelegations").foreachStream().when({
  $init: function() {
    return {
      unit: {
        trusteeId: null,
        blocked: false
      },
      areas: {}
    };
  },

  UNIT_DELEGATION_SET_EVENT: (s, e) => {
    const p = e.body.payload;
    s.unit = {
      trusteeId: p.trusteeId,
      blocked: false
    };
  },

  UNIT_DELEGATION_UNSET_EVENT: (s, e) => {
    const p = e.body.payload;
    s.unit = {
      trusteeId: null,
      blocked: false
    };
  },

  AREA_DELEGATION_SET_EVENT: (s, e) => {
    const p = e.body.payload;
    s.areas[p.areaId] = {
      trusteeId: p.trusteeId,
      blocked: false
    };
  }
});
