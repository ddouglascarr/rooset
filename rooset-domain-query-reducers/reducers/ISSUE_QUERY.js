fromAll()
.foreachStream()
.when({

  $init: function() {
    return {
      id: null,
      unitId: null,
      areaId: null,
      policyId: null,
      created: 0,
      issueState: "ADMISSION",
      initiatives: [],
    };
  },

  NEW_INITIATIVE_CREATED_EVENT: function(s, e) {
    const p = e.body.payload;
    s.id = p.id;
    s.unitId = p.unitId;
    s.areaId = p.areaId;
    s.policyId = p.policyId;
    s.created = p.created;
    s.initiatives.push({
      key: p.initiativeId,
      name: p.name,
    });
  },

  COMPETING_INITIATIVE_CREATED_EVENT: function(s, e) {
    const p = e.body.payload;
    s.initiatives.push({
      key: p.initiativeId,
      name: p.name,
    });
  },

})
