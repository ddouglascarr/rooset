
const fetch = require("node-fetch");
const {
  startEventStore,
  initAllProjections,
  persistEvent,
 } = require('../utils/eventStoreUtils');

describe("Issue Query Tests", () => {

  let eventStoreProcess = null;
  beforeEach(() => {
    return startEventStore()
        .then((process) => { eventStoreProcess = process });
  });

  afterEach(() => {
    eventStoreProcess.kill();
  });



    it("Query issue", () => {
      // init projection
      const reducerFileContent = "fromAll()\n.foreachStream()\n.when({\n\n  $init: function() {\n    return {\n      id: null,\n      unitId: null,\n      areaId: null,\n      policyId: null,\n      concernId: null,\n      created: 0,\n      issueState: \"ADMISSION\",\n      resolved: false,\n      initiatives: [],\n    };\n  },\n\n  NEW_INITIATIVE_CREATED_EVENT: function(s, e) {\n    const p = e.body.payload;\n    s.id = p.id;\n    s.unitId = p.unitId;\n    s.areaId = p.areaId;\n    s.policyId = p.policyId;\n    s.concernId = p.concernId;\n    s.created = p.created;\n    s.initiatives.push({\n      key: p.initiativeId,\n      name: p.name,\n    });\n  },\n\n  COMPETING_INITIATIVE_CREATED_EVENT: function(s, e) {\n    const p = e.body.payload;\n    s.initiatives.push({\n      key: p.initiativeId,\n      name: p.name,\n    });\n  },\n\n  ISSUE_ADMISSION_QUORUM_PASSED_EVENT: function(s, e) {\n    s.issueState = \"DISCUSSION\";\n  },\n\n  ISSUE_ADMISSION_QUORUM_FAILED_EVENT: function(s, e) {\n    s.issueState = \"CANCELED_NO_INITIATIVE_ADMITTED\";\n    s.resolved = true;\n    s.initiatives = [];\n  },\n\n})\n";
      return Promise.resolve()
      .then(() => initAllProjections())

      // persist events
      .then(() => {
          return Promise.resolve()
         .then(() => { return persistEvent({"type":"NEW_INITIATIVE_CREATED_EVENT","payload":{"id":"464b1ebb-32c1-460c-8e9e-aaaaaaaaaaaa","requesterId":"464b1ebb-32c1-460c-8e9e-111111111111","initiativeId":"464b1ebb-32c1-460c-8e9e-444444444444","unitId":"464b1ebb-32c1-460c-8e9e-000000000000","areaId":"464b1ebb-32c1-460c-8e9e-bbbbbbbbbbbb","policyId":"464b1ebb-32c1-460c-8e9e-cccccccccccc","concernId":"464b1ebb-32c1-460c-8e9e-dddddddddddd","name":"Paint the bike shed","content":"Paint the bike shed red","created":1487394829}}); })
       .then(() => { return persistEvent({"type":"COMPETING_INITIATIVE_CREATED_EVENT","payload":{"id":"464b1ebb-32c1-460c-8e9e-aaaaaaaaaaaa","requesterId":"464b1ebb-32c1-460c-8e9e-222222222222","initiativeId":"464b1ebb-32c1-460c-8e9e-555555555555","name":"Paint it green","content":"Paint the bike shed green","created":1487394950}}); })
          .then((resp) => Promise.resolve({foo: 'bar'}))
      })

      // perform query
      .then(() => {
        const url = "http://localhost:2113/projection/ISSUE_QUERY/state?partition=aggregate-464b1ebb-32c1-460c-8e9e-aaaaaaaaaaaa";
        return fetch(url);
      })

      .then((resp) => {
        if (!resp.ok) throw new Error("response failed: " + resp);
        return resp.json();
      })
      .then(function(body) {
        expect(body).toEqual({"id":"464b1ebb-32c1-460c-8e9e-aaaaaaaaaaaa","unitId":"464b1ebb-32c1-460c-8e9e-000000000000","areaId":"464b1ebb-32c1-460c-8e9e-bbbbbbbbbbbb","policyId":"464b1ebb-32c1-460c-8e9e-cccccccccccc","concernId":"464b1ebb-32c1-460c-8e9e-dddddddddddd","created":1487394829,"issueState":"ADMISSION","resolved":false,"initiatives":[{"key":"464b1ebb-32c1-460c-8e9e-444444444444","name":"Paint the bike shed"},{"key":"464b1ebb-32c1-460c-8e9e-555555555555","name":"Paint it green"}]});
        return Promise.resolve();
      });
    });

  

    it("The issue state advances to DISCUSSION", () => {
      // init projection
      const reducerFileContent = "fromAll()\n.foreachStream()\n.when({\n\n  $init: function() {\n    return {\n      id: null,\n      unitId: null,\n      areaId: null,\n      policyId: null,\n      concernId: null,\n      created: 0,\n      issueState: \"ADMISSION\",\n      resolved: false,\n      initiatives: [],\n    };\n  },\n\n  NEW_INITIATIVE_CREATED_EVENT: function(s, e) {\n    const p = e.body.payload;\n    s.id = p.id;\n    s.unitId = p.unitId;\n    s.areaId = p.areaId;\n    s.policyId = p.policyId;\n    s.concernId = p.concernId;\n    s.created = p.created;\n    s.initiatives.push({\n      key: p.initiativeId,\n      name: p.name,\n    });\n  },\n\n  COMPETING_INITIATIVE_CREATED_EVENT: function(s, e) {\n    const p = e.body.payload;\n    s.initiatives.push({\n      key: p.initiativeId,\n      name: p.name,\n    });\n  },\n\n  ISSUE_ADMISSION_QUORUM_PASSED_EVENT: function(s, e) {\n    s.issueState = \"DISCUSSION\";\n  },\n\n  ISSUE_ADMISSION_QUORUM_FAILED_EVENT: function(s, e) {\n    s.issueState = \"CANCELED_NO_INITIATIVE_ADMITTED\";\n    s.resolved = true;\n    s.initiatives = [];\n  },\n\n})\n";
      return Promise.resolve()
      .then(() => initAllProjections())

      // persist events
      .then(() => {
          return Promise.resolve()
         .then(() => { return persistEvent({"type":"NEW_INITIATIVE_CREATED_EVENT","payload":{"id":"464b1ebb-32c1-460c-8e9e-aaaaaaaaaaaa","requesterId":"464b1ebb-32c1-460c-8e9e-111111111111","initiativeId":"464b1ebb-32c1-460c-8e9e-444444444444","unitId":"464b1ebb-32c1-460c-8e9e-000000000000","areaId":"464b1ebb-32c1-460c-8e9e-bbbbbbbbbbbb","policyId":"464b1ebb-32c1-460c-8e9e-cccccccccccc","concernId":"464b1ebb-32c1-460c-8e9e-dddddddddddd","name":"Paint the bike shed","content":"Paint the bike shed red","created":1487394829}}); })
       .then(() => { return persistEvent({"type":"COMPETING_INITIATIVE_CREATED_EVENT","payload":{"id":"464b1ebb-32c1-460c-8e9e-aaaaaaaaaaaa","requesterId":"464b1ebb-32c1-460c-8e9e-222222222222","initiativeId":"464b1ebb-32c1-460c-8e9e-555555555555","name":"Paint it green","content":"Paint the bike shed green","created":1487394950}}); })
       .then(() => { return persistEvent({"type":"ISSUE_ADMISSION_QUORUM_PASSED_EVENT","payload":{"id":"464b1ebb-32c1-460c-8e9e-aaaaaaaaaaaa"}}); })
          .then((resp) => Promise.resolve({foo: 'bar'}))
      })

      // perform query
      .then(() => {
        const url = "http://localhost:2113/projection/ISSUE_QUERY/state?partition=aggregate-464b1ebb-32c1-460c-8e9e-aaaaaaaaaaaa";
        return fetch(url);
      })

      .then((resp) => {
        if (!resp.ok) throw new Error("response failed: " + resp);
        return resp.json();
      })
      .then(function(body) {
        expect(body).toEqual({"id":"464b1ebb-32c1-460c-8e9e-aaaaaaaaaaaa","unitId":"464b1ebb-32c1-460c-8e9e-000000000000","areaId":"464b1ebb-32c1-460c-8e9e-bbbbbbbbbbbb","policyId":"464b1ebb-32c1-460c-8e9e-cccccccccccc","concernId":"464b1ebb-32c1-460c-8e9e-dddddddddddd","created":1487394829,"issueState":"DISCUSSION","resolved":false,"initiatives":[{"key":"464b1ebb-32c1-460c-8e9e-444444444444","name":"Paint the bike shed"},{"key":"464b1ebb-32c1-460c-8e9e-555555555555","name":"Paint it green"}]});
        return Promise.resolve();
      });
    });

  

    it("The issue state falls to CANCELED_NO_INITIATIVE_ADMITTED if quorum not passed", () => {
      // init projection
      const reducerFileContent = "fromAll()\n.foreachStream()\n.when({\n\n  $init: function() {\n    return {\n      id: null,\n      unitId: null,\n      areaId: null,\n      policyId: null,\n      concernId: null,\n      created: 0,\n      issueState: \"ADMISSION\",\n      resolved: false,\n      initiatives: [],\n    };\n  },\n\n  NEW_INITIATIVE_CREATED_EVENT: function(s, e) {\n    const p = e.body.payload;\n    s.id = p.id;\n    s.unitId = p.unitId;\n    s.areaId = p.areaId;\n    s.policyId = p.policyId;\n    s.concernId = p.concernId;\n    s.created = p.created;\n    s.initiatives.push({\n      key: p.initiativeId,\n      name: p.name,\n    });\n  },\n\n  COMPETING_INITIATIVE_CREATED_EVENT: function(s, e) {\n    const p = e.body.payload;\n    s.initiatives.push({\n      key: p.initiativeId,\n      name: p.name,\n    });\n  },\n\n  ISSUE_ADMISSION_QUORUM_PASSED_EVENT: function(s, e) {\n    s.issueState = \"DISCUSSION\";\n  },\n\n  ISSUE_ADMISSION_QUORUM_FAILED_EVENT: function(s, e) {\n    s.issueState = \"CANCELED_NO_INITIATIVE_ADMITTED\";\n    s.resolved = true;\n    s.initiatives = [];\n  },\n\n})\n";
      return Promise.resolve()
      .then(() => initAllProjections())

      // persist events
      .then(() => {
          return Promise.resolve()
         .then(() => { return persistEvent({"type":"NEW_INITIATIVE_CREATED_EVENT","payload":{"id":"464b1ebb-32c1-460c-8e9e-aaaaaaaaaaaa","requesterId":"464b1ebb-32c1-460c-8e9e-111111111111","initiativeId":"464b1ebb-32c1-460c-8e9e-444444444444","unitId":"464b1ebb-32c1-460c-8e9e-000000000000","areaId":"464b1ebb-32c1-460c-8e9e-bbbbbbbbbbbb","policyId":"464b1ebb-32c1-460c-8e9e-cccccccccccc","concernId":"464b1ebb-32c1-460c-8e9e-dddddddddddd","name":"Paint the bike shed","content":"Paint the bike shed red","created":1487394829}}); })
       .then(() => { return persistEvent({"type":"COMPETING_INITIATIVE_CREATED_EVENT","payload":{"id":"464b1ebb-32c1-460c-8e9e-aaaaaaaaaaaa","requesterId":"464b1ebb-32c1-460c-8e9e-222222222222","initiativeId":"464b1ebb-32c1-460c-8e9e-555555555555","name":"Paint it green","content":"Paint the bike shed green","created":1487394950}}); })
       .then(() => { return persistEvent({"type":"ISSUE_ADMISSION_QUORUM_FAILED_EVENT","payload":{"id":"464b1ebb-32c1-460c-8e9e-aaaaaaaaaaaa"}}); })
          .then((resp) => Promise.resolve({foo: 'bar'}))
      })

      // perform query
      .then(() => {
        const url = "http://localhost:2113/projection/ISSUE_QUERY/state?partition=aggregate-464b1ebb-32c1-460c-8e9e-aaaaaaaaaaaa";
        return fetch(url);
      })

      .then((resp) => {
        if (!resp.ok) throw new Error("response failed: " + resp);
        return resp.json();
      })
      .then(function(body) {
        expect(body).toEqual({"id":"464b1ebb-32c1-460c-8e9e-aaaaaaaaaaaa","unitId":"464b1ebb-32c1-460c-8e9e-000000000000","areaId":"464b1ebb-32c1-460c-8e9e-bbbbbbbbbbbb","policyId":"464b1ebb-32c1-460c-8e9e-cccccccccccc","concernId":"464b1ebb-32c1-460c-8e9e-dddddddddddd","created":1487394829,"issueState":"CANCELED_NO_INITIATIVE_ADMITTED","resolved":true,"initiatives":[]});
        return Promise.resolve();
      });
    });

  

});


