
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



    it("Query Url Parameter name", () => {
      // init projection
      const reducerFileContent = "fromAll()\n.partitionBy(function(event) {\n  return event.body.payload.urlParameterName || \"na\";\n})\n.when({\n  $init: function() {\n    return { id: null };\n  },\n\n  UNIT_CREATED_EVENT: function(s, e) {\n    s.id = e.body.payload.id;\n  }\n})\n";
      return Promise.resolve()
      .then(() =>  initProjection("UNIT_URL_PARAMETER_NAME_QUERY", reducerFileContent))

      // persist events
      .then(() => {
          return Promise.resolve()
         .then(() => { return persistEvent({"type":"UNIT_CREATED_EVENT","payload":{"id":"464b1ebb-32c1-460c-8e9e-000000000000","requesterId":"464b1ebb-32c1-460c-8e9e-444444444444","name":"Test Unit","description":"The Test Unit","urlParameterName":"test-unit"}}); })
       .then(() => { return persistEvent({"type":"POLICY_CREATED_EVENT","payload":{"policyId":"464b1ebb-32c1-460c-8e9e-333333333333","name":"Test Policy","description":"The Test Policy","votingAlgorithm":"SCHULZE","maxAdmissionTime":604800000,"minAdmissionTime":0,"discussionTime":604800000,"verificationTime":604800000,"votingTime":604800000,"issueQuorumNum":1,"issueQuorumDen":10,"initiativeQuorumNum":1,"initiativeQuorumDen":10,"id":"464b1ebb-32c1-460c-8e9e-000000000000","requesterId":"464b1ebb-32c1-460c-8e9e-444444444444"}}); })
       .then(() => { return persistEvent({"type":"AREA_CREATED_EVENT","payload":{"id":"464b1ebb-32c1-460c-8e9e-000000000000","areaId":"464b1ebb-32c1-460c-8e9e-111111111111","requesterId":"464b1ebb-32c1-460c-8e9e-444444444444","name":"area0","description":"The 0 Area","externalReference":"http://area0.org"}}); })
       .then(() => { return persistEvent({"type":"AREA_CREATED_EVENT","payload":{"id":"464b1ebb-32c1-460c-8e9e-000000000000","areaId":"464b1ebb-32c1-460c-8e9e-222222222222","requesterId":"464b1ebb-32c1-460c-8e9e-444444444444","name":"area1","description":"The 1 Area","externalReference":"http://area1.org"}}); })
       .then(() => { return persistEvent({"type":"CONCERN_CREATED_EVENT","payload":{"id":"464b1ebb-32c1-460c-8e9e-000000000000","concernId":"464b1ebb-32c1-460c-8e9a-ffffffffffff","requesterId":"464b1ebb-32c1-460c-8e9e-444444444444","name":"Test Concern","description":"The Test Concern","initiativeContentType":"STATEMENT","config":{}}}); })
       .then(() => { return persistEvent({"type":"AREA_CONCERN_ADDED_EVENT","payload":{"id":"464b1ebb-32c1-460c-8e9e-000000000000","areaId":"464b1ebb-32c1-460c-8e9e-222222222222","concernId":"464b1ebb-32c1-460c-8e9a-ffffffffffff","requesterId":"464b1ebb-32c1-460c-8e9e-444444444444"}}); })
       .then(() => { return persistEvent({"type":"CONCERN_POLICY_ADDED_EVENT","payload":{"id":"464b1ebb-32c1-460c-8e9e-000000000000","requesterId":"464b1ebb-32c1-460c-8e9e-444444444444","policyId":"464b1ebb-32c1-460c-8e9e-333333333333","concernId":"464b1ebb-32c1-460c-8e9a-ffffffffffff"}}); })
          .then((resp) => Promise.resolve({foo: 'bar'}))
      })

      // perform query
      .then(() => {
        const url = "http://localhost:2113/projection/UNIT_URL_PARAMETER_NAME_QUERY/state?partition=test-unit";
        return fetch(url);
      })

      .then((resp) => {
        if (!resp.ok) throw new Error("response failed: " + resp);
        return resp.json();
      })
      .then(function(body) {
        expect(body).toEqual({"id":"464b1ebb-32c1-460c-8e9e-000000000000"});
        return Promise.resolve();
      });
    });

  

    it("Query active unit", () => {
      // init projection
      const reducerFileContent = "fromAll()\n.foreachStream()\n.when({\n\n  $init: function() {\n    return {\n      id: null,\n      name: \"\",\n      description: \"\",\n      areas: [],\n      policies: [],\n      concerns: [],\n    };\n  },\n\n  UNIT_CREATED_EVENT: function(s, e) {\n    s.id = e.body.payload.id;\n    s.name = e.body.payload.name;\n    s.description = e.body.payload.description;\n  },\n\n  POLICY_CREATED_EVENT: function(s, e) {\n    const p = e.body.payload;\n    const policy = {\n      policyId: p.policyId,\n      name: p.name,\n      description: p.description,\n      votingAlgorithm: p.votingAlgorithm,\n      minAdmissionTime: p.minAdmissionTime,\n      maxAdmissionTime: p.maxAdmissionTime,\n      discussionTime: p.discussionTime,\n      verificationTime: p.verificationTime,\n      votingTime: p.votingTime,\n      issueQuorumNum: p.issueQuorumNum,\n      issueQuorumDen: p.issueQuorumDen,\n      initiativeQuorumNum: p.initiativeQuorumNum,\n      initiativeQuorumDen: p.initiativeQuorumDen,\n    };\n    s.policies.push(policy);\n  },\n\n  CONCERN_CREATED_EVENT: function(s, e) {\n    const p = e.body.payload;\n    const concern = {\n      concernId: p.concernId,\n      name: p.name,\n      description: p.description,\n      initiativeContentType: p.initiativeContentType,\n      policies: [],\n      config: p.config,\n    };\n    s.concerns.push(concern);\n  },\n\n  CONCERN_POLICY_ADDED_EVENT: function(s, e) {\n    const p = e.body.payload;\n    let concern = s.concerns.find(c => c.concernId === p.concernId);\n    concern.policies.push(p.policyId);\n  },\n\n  AREA_CREATED_EVENT: function(s, e) {\n    const p = e.body.payload;\n    const area = {\n      areaId: p.areaId,\n      name: p.name,\n      description: p.description,\n      externalReference: p.externalReference,\n      concerns: [],\n    };\n    s.areas.push(area);\n  },\n\n  AREA_CONCERN_ADDED_EVENT: function(s, e) {\n    const p = e.body.payload;\n    const area = s.areas.find(a => a.areaId === p.areaId);\n    area.concerns.push(p.concernId);\n  },\n\n});\n";
      return Promise.resolve()
      .then(() =>  initProjection("UNIT_QUERY", reducerFileContent))

      // persist events
      .then(() => {
          return Promise.resolve()
         .then(() => { return persistEvent({"type":"UNIT_CREATED_EVENT","payload":{"id":"464b1ebb-32c1-460c-8e9e-000000000000","requesterId":"464b1ebb-32c1-460c-8e9e-444444444444","name":"Test Unit","description":"The Test Unit","urlParameterName":"test-unit"}}); })
       .then(() => { return persistEvent({"type":"POLICY_CREATED_EVENT","payload":{"policyId":"464b1ebb-32c1-460c-8e9e-333333333333","name":"Test Policy","description":"The Test Policy","votingAlgorithm":"SCHULZE","maxAdmissionTime":604800000,"minAdmissionTime":0,"discussionTime":604800000,"verificationTime":604800000,"votingTime":604800000,"issueQuorumNum":1,"issueQuorumDen":10,"initiativeQuorumNum":1,"initiativeQuorumDen":10,"id":"464b1ebb-32c1-460c-8e9e-000000000000","requesterId":"464b1ebb-32c1-460c-8e9e-444444444444"}}); })
       .then(() => { return persistEvent({"type":"AREA_CREATED_EVENT","payload":{"id":"464b1ebb-32c1-460c-8e9e-000000000000","areaId":"464b1ebb-32c1-460c-8e9e-111111111111","requesterId":"464b1ebb-32c1-460c-8e9e-444444444444","name":"area0","description":"The 0 Area","externalReference":"http://area0.org"}}); })
       .then(() => { return persistEvent({"type":"AREA_CREATED_EVENT","payload":{"id":"464b1ebb-32c1-460c-8e9e-000000000000","areaId":"464b1ebb-32c1-460c-8e9e-222222222222","requesterId":"464b1ebb-32c1-460c-8e9e-444444444444","name":"area1","description":"The 1 Area","externalReference":"http://area1.org"}}); })
       .then(() => { return persistEvent({"type":"CONCERN_CREATED_EVENT","payload":{"id":"464b1ebb-32c1-460c-8e9e-000000000000","concernId":"464b1ebb-32c1-460c-8e9a-ffffffffffff","requesterId":"464b1ebb-32c1-460c-8e9e-444444444444","name":"Test Concern","description":"The Test Concern","initiativeContentType":"STATEMENT","config":{}}}); })
       .then(() => { return persistEvent({"type":"AREA_CONCERN_ADDED_EVENT","payload":{"id":"464b1ebb-32c1-460c-8e9e-000000000000","areaId":"464b1ebb-32c1-460c-8e9e-222222222222","concernId":"464b1ebb-32c1-460c-8e9a-ffffffffffff","requesterId":"464b1ebb-32c1-460c-8e9e-444444444444"}}); })
       .then(() => { return persistEvent({"type":"CONCERN_POLICY_ADDED_EVENT","payload":{"id":"464b1ebb-32c1-460c-8e9e-000000000000","requesterId":"464b1ebb-32c1-460c-8e9e-444444444444","policyId":"464b1ebb-32c1-460c-8e9e-333333333333","concernId":"464b1ebb-32c1-460c-8e9a-ffffffffffff"}}); })
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
        expect(body).toEqual({"id":"464b1ebb-32c1-460c-8e9e-000000000000","name":"Test Unit","description":"The Test Unit","policies":[{"policyId":"464b1ebb-32c1-460c-8e9e-333333333333","name":"Test Policy","description":"The Test Policy","votingAlgorithm":"SCHULZE","maxAdmissionTime":604800000,"minAdmissionTime":0,"discussionTime":604800000,"verificationTime":604800000,"votingTime":604800000,"issueQuorumNum":1,"issueQuorumDen":10,"initiativeQuorumNum":1,"initiativeQuorumDen":10}],"concerns":[{"concernId":"464b1ebb-32c1-460c-8e9a-ffffffffffff","name":"Test Concern","description":"The Test Concern","initiativeContentType":"STATEMENT","policies":["464b1ebb-32c1-460c-8e9e-333333333333"],"config":{}}],"areas":[{"areaId":"464b1ebb-32c1-460c-8e9e-111111111111","name":"area0","description":"The 0 Area","externalReference":"http://area0.org","concerns":[]},{"areaId":"464b1ebb-32c1-460c-8e9e-222222222222","name":"area1","description":"The 1 Area","externalReference":"http://area1.org","concerns":["464b1ebb-32c1-460c-8e9a-ffffffffffff"]}]});
        return Promise.resolve();
      });
    });

  

});


