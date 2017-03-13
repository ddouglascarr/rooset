
const fetch = require("node-fetch");
const {
  startEventStore,
  initProjection,
  persistEvent,
 } = require('../utils/eventStoreUtils');

describe("Unit Member Query Tests", () => {

  let eventStoreProcess = null;
  beforeEach(() => {
    return startEventStore()
        .then((process) => { eventStoreProcess = process });
  });

  afterEach(() => {
    eventStoreProcess.kill();
  });



    it("Query unit members", () => {
      // init projection
      const reducerFileContent = "fromAll()\n.foreachStream()\n.when({\n\n  $init: function() {\n    return {\n      id: null,\n      members: [],\n      totalWeight: 0,\n    };\n  },\n\n  UNIT_CREATED_EVENT: function(s, e) {\n    s.id = e.body.payload.id;\n  },\n\n  PRIVILEGE_GRANTED_EVENT: function(s, e) {\n    const p = e.body.payload;\n    s.members.push({\n      id: p.id,\n      memberId: p.memberId,\n      pollingRight: p.pollingRight,\n      votingRight: p.votingRight,\n      initiativeRight: p.initiativeRight,\n      managementRight: p.managementRight,\n      weight: p.weight,\n    });\n    s.totalWeight += p.weight;\n  },\n\n})\n";
      return Promise.resolve()
      .then(() =>  initProjection("UNIT_MEMBER_QUERY", reducerFileContent))

      // persist events
      .then(() => {
          return Promise.resolve()
         .then(() => { return persistEvent({"type":"UNIT_CREATED_EVENT","payload":{"id":"464b1ebb-32c1-460c-8e9e-000000000000","requesterId":"464b1ebb-32c1-460c-8e9e-333333333333","name":"Test Unit","description":"The Test Unit","urlParameterName":"test-unit"}}); })
       .then(() => { return persistEvent({"type":"PRIVILEGE_GRANTED_EVENT","payload":{"id":"464b1ebb-32c1-460c-8e9e-000000000000","requesterId":"464b1ebb-32c1-460c-8e9e-333333333333","memberId":"464b1ebb-32c1-460c-8e9e-111111111111","pollingRight":true,"votingRight":true,"initiativeRight":true,"managementRight":false,"weight":2}}); })
       .then(() => { return persistEvent({"type":"PRIVILEGE_GRANTED_EVENT","payload":{"id":"464b1ebb-32c1-460c-8e9e-000000000000","requesterId":"464b1ebb-32c1-460c-8e9e-333333333333","memberId":"464b1ebb-32c1-460c-8e9e-222222222222","pollingRight":true,"votingRight":true,"initiativeRight":false,"managementRight":false,"weight":1}}); })
          .then((resp) => Promise.resolve({foo: 'bar'}))
      })

      // perform query
      .then(() => {
        const url = "http://localhost:2113/projection/UNIT_MEMBER_QUERY/state?partition=aggregate-464b1ebb-32c1-460c-8e9e-000000000000";
        return fetch(url);
      })

      .then((resp) => {
        if (!resp.ok) throw new Error("response failed: " + resp);
        return resp.json();
      })
      .then(function(body) {
        expect(body).toEqual({"id":"464b1ebb-32c1-460c-8e9e-000000000000","members":[{"id":"464b1ebb-32c1-460c-8e9e-000000000000","memberId":"464b1ebb-32c1-460c-8e9e-111111111111","pollingRight":true,"votingRight":true,"initiativeRight":true,"managementRight":false,"weight":2},{"id":"464b1ebb-32c1-460c-8e9e-000000000000","memberId":"464b1ebb-32c1-460c-8e9e-222222222222","pollingRight":true,"votingRight":true,"initiativeRight":false,"managementRight":false,"weight":1}],"totalWeight":3});
        return Promise.resolve();
      });
    });

  

});


