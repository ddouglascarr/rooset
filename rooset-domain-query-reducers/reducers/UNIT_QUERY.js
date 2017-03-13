fromCategory("aggregate")
.foreachStream()
.when({

  $init: function() {
    return {
      id: null,
      name: "",
      description: "",
      areas: [],
      policies: [],
      concerns: [],
    };
  },

  UNIT_CREATED_EVENT: function(s, e) {
    s.id = e.body.payload.id;
    s.name = e.body.payload.name;
    s.description = e.body.payload.description;
  },

  POLICY_CREATED_EVENT: function(s, e) {
    const p = e.body.payload;
    const policy = {
      policyId: p.policyId,
      name: p.name,
      description: p.description,
      votingAlgorithm: p.votingAlgorithm,
      minAdmissionTime: p.minAdmissionTime,
      maxAdmissionTime: p.maxAdmissionTime,
      discussionTime: p.discussionTime,
      verificationTime: p.verificationTime,
      votingTime: p.votingTime,
      issueQuorumNum: p.issueQuorumNum,
      issueQuorumDen: p.issueQuorumDen,
      initiativeQuorumNum: p.initiativeQuorumNum,
      initiativeQuorumDen: p.initiativeQuorumDen,
    };
    s.policies.push(policy);
  },

  CONCERN_CREATED_EVENT: function(s, e) {
    const p = e.body.payload;
    const concern = {
      concernId: p.concernId,
      name: p.name,
      description: p.description,
      initiativeContentType: p.initiativeContentType,
      policies: [],
      config: p.config,
    };
    s.concerns.push(concern);
  },

  CONCERN_POLICY_ADDED_EVENT: function(s, e) {
    const p = e.body.payload;
    let concern = s.concerns.find(c => c.concernId === p.concernId);
    concern.policies.push(p.policyId);
  },

  AREA_CREATED_EVENT: function(s, e) {
    const p = e.body.payload;
    const area = {
      areaId: p.areaId,
      name: p.name,
      description: p.description,
      externalReference: p.externalReference,
      concerns: [],
    };
    s.areas.push(area);
  },

  AREA_CONCERN_ADDED_EVENT: function(s, e) {
    const p = e.body.payload;
    const area = s.areas.find(a => a.areaId === p.areaId);
    area.concerns.push(p.concernId);
  },

});
