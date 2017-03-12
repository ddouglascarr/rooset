fromAll()
.foreachStream()
.when({

  $init: function() {
    return {
      id: null,
      name: "",
      description: "",
      areas: [],
      policies: [],
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

  AREA_CREATED_EVENT: function(s, e) {
    const p = e.body.payload;
    const area = {
      areaId: p.areaId,
      name: p.name,
      description: p.description,
      externalReference: p.externalReference,
    };
    s.areas.push(area);
  },

});
