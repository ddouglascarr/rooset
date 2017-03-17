
const fetch = require("node-fetch");
const {
  startEventStore,
  initAllProjections,
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
      return Promise.resolve()
      .then(() => initAllProjections())

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
          .then(() => new Promise((r) => { setTimeout(r, 300); }))
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
      return Promise.resolve()
      .then(() => initAllProjections())

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
          .then(() => new Promise((r) => { setTimeout(r, 300); }))
          .then((resp) => Promise.resolve({foo: 'bar'}))
      })

      // perform query
      .then(() => {
        const url = "http://localhost:2113/projection/UNIT_QUERY/state?partition=aggregate-464b1ebb-32c1-460c-8e9e-000000000000";
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


