
const fetch = require("node-fetch");
const {
  startEventStore,
  initProjection,
  persistEvent,
 } = require('../utils/eventStoreUtils');

describe("Unit Query Tests", () => {

  let eventStoreProcess = null;
  beforeEach(() => {
    return startEventStore()
        .then((process) => { eventStoreProcess = process });
  });

  afterEach(() => {
    eventStoreProcess.kill();
  });



    it("Query active unit", () => {
      // init projection
      const reducerFileContent = "fromAll()\n.foreachStream()\n.when({\n\n  $init: function() {\n    return {\n      id: null,\n      name: \"\",\n      description: \"\",\n      areas: [],\n      policy: null,\n    };\n  },\n\n  UNIT_CREATED_EVENT: function(s, e) {\n    s.id = e.body.payload.id;\n    s.name = e.body.payload.name;\n    s.description = e.body.payload.description;\n  },\n\n  UNIT_POLICY_SET_EVENT: function(s, e) {\n    const p = e.body.payload;\n    const policy = {\n      policyId: p.policyId,\n      name: p.name,\n      description: p.description,\n      discussionTime: p.discussionTime,\n      verificationTime: p.verificationTime,\n      votingTime: p.votingTime,\n      issueQuorumNum: p.issueQuorumNum,\n      issueQuorumDen: p.issueQuorumDen,\n    };\n    s.policy = policy;\n  },\n\n  AREA_CREATED_EVENT: function(s, e) {\n    const p = e.body.payload;\n    const area = {\n      areaId: p.areaId,\n      name: p.name,\n      description: p.description,\n      externalReference: p.externalReference,\n    };\n    s.areas.push(area);\n  },\n\n});\n";
      return Promise.resolve()
      .then(() =>  initProjection("UNIT_QUERY", reducerFileContent))

      // persist events
      .then(() => {
          return Promise.resolve()
         .then(() => { return persistEvent({"type":"UNIT_CREATED_EVENT","payload":{"id":"464b1ebb-32c1-460c-8e9e-000000000000","requesterId":"464b1ebb-32c1-460c-8e9e-444444444444","name":"Test Unit","description":"The Test Unit"}}); })
       .then(() => { return persistEvent({"type":"UNIT_POLICY_SET_EVENT","payload":{"policyId":"464b1ebb-32c1-460c-8e9e-333333333333","name":"Test Policy","description":"The Test Policy","discussionTime":604800000,"verificationTime":604800000,"votingTime":604800000,"issueQuorumNum":1,"issueQuorumDen":10,"id":"464b1ebb-32c1-460c-8e9e-000000000000","requesterId":"464b1ebb-32c1-460c-8e9e-444444444444","polling":false,"maxAdmissionTime":604800000,"minAdmissionTime":0,"defeatStrength":"SIMPLE","directMajorityNum":1,"directMajorityDen":2,"directMajorityStrict":true,"directMajorityPositive":1,"directMajorityNonNegative":1,"noReverseBeatPath":false,"noMultistageMajority":false}}); })
       .then(() => { return persistEvent({"type":"AREA_CREATED_EVENT","payload":{"id":"464b1ebb-32c1-460c-8e9e-000000000000","areaId":"464b1ebb-32c1-460c-8e9e-111111111111","requesterId":"464b1ebb-32c1-460c-8e9e-444444444444","name":"area0","description":"The 0 Area","externalReference":"http://area0.org"}}); })
       .then(() => { return persistEvent({"type":"AREA_CREATED_EVENT","payload":{"id":"464b1ebb-32c1-460c-8e9e-000000000000","areaId":"464b1ebb-32c1-460c-8e9e-222222222222","requesterId":"464b1ebb-32c1-460c-8e9e-444444444444","name":"area1","description":"The 1 Area","externalReference":"http://area1.org"}}); })
          .then((resp) => Promise.resolve({foo: 'bar'}))
      })

      // perform query
      .then(() => {
        const url = "http://localhost:2113/projection/UNIT_QUERY/state?partition=464b1ebb-32c1-460c-8e9e-000000000000";
        return fetch(url);
      })

      .then((resp) => {
        if (!resp.ok) throw new Error("response failed: " + resp);
        return resp.json();
      })
      .then(function(body) {
        expect(body).toEqual({"id":"464b1ebb-32c1-460c-8e9e-000000000000","name":"Test Unit","description":"The Test Unit","policy":{"policyId":"464b1ebb-32c1-460c-8e9e-333333333333","name":"Test Policy","description":"The Test Policy","discussionTime":604800000,"verificationTime":604800000,"votingTime":604800000,"issueQuorumNum":1,"issueQuorumDen":10},"areas":[{"areaId":"464b1ebb-32c1-460c-8e9e-111111111111","name":"area0","description":"The 0 Area","externalReference":"http://area0.org"},{"areaId":"464b1ebb-32c1-460c-8e9e-222222222222","name":"area1","description":"The 1 Area","externalReference":"http://area1.org"}]});
        return Promise.resolve();
      });
    });

  

});

