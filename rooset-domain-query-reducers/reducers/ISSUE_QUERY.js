fromAll()
.foreachStream()
.when({

  $init: function() {
    return {
      id: null,
      unitId: null,
      areaId: null,
      policyId: null,
      concernId: null,
      created: 0,
      issueState: "ADMISSION",
      resolved: false,
      initiatives: [],
    };
  },

  NEW_INITIATIVE_CREATED_EVENT: function(s, e) {
    const p = e.body.payload;
    s.id = p.id;
    s.unitId = p.unitId;
    s.areaId = p.areaId;
    s.policyId = p.policyId;
    s.concernId = p.concernId;
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

  ISSUE_ADMISSION_QUORUM_PASSED_EVENT: function(s, e) {
    s.issueState = "DISCUSSION";
  },

  ISSUE_ADMISSION_QUORUM_FAILED_EVENT: function(s, e) {
    s.issueState = "CANCELED_NO_INITIATIVE_ADMITTED";
    s.resolved = true;
    s.initiatives = [];
  },

})
