
const fetch = require("node-fetch");
const fs = require('fs');
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
      const file = fs.readFileSync(__dirname + "/../reducers/UNIT_MEMBER_QUERY.js", "utf8");
      if (!file) throw new Error("could not read UNIT_MEMBER_QUERY reducer");
      return Promise.resolve()
      .then(() =>  initProjection("UNIT_MEMBER_QUERY", file))

      // persist events
      .then(() => {
          return Promise.resolve()
         .then(() => { return persistEvent({"type":"UNIT_CREATED_EVENT","payload":{"id":"464b1ebb-32c1-460c-8e9e-000000000000","requesterId":"464b1ebb-32c1-460c-8e9e-333333333333","name":"Test Unit","description":"The Test Unit"}}); })
       .then(() => { return persistEvent({"type":"PRIVILEGE_GRANTED_EVENT","payload":{"id":"464b1ebb-32c1-460c-8e9e-000000000000","requesterId":"464b1ebb-32c1-460c-8e9e-333333333333","memberId":"464b1ebb-32c1-460c-8e9e-111111111111","pollingRight":true,"votingRight":true,"initiativeRight":true,"managementRight":false,"weight":2}}); })
       .then(() => { return persistEvent({"type":"PRIVILEGE_GRANTED_EVENT","payload":{"id":"464b1ebb-32c1-460c-8e9e-000000000000","requesterId":"464b1ebb-32c1-460c-8e9e-333333333333","memberId":"464b1ebb-32c1-460c-8e9e-222222222222","pollingRight":true,"votingRight":true,"initiativeRight":false,"managementRight":false,"weight":1}}); })
          .then((resp) => Promise.resolve({foo: 'bar'}))
      })

      // perform query
      .then(() => {
        const url = "http://localhost:2113/projection/UNIT_MEMBER_QUERY/state?partition=464b1ebb-32c1-460c-8e9e-000000000000";
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


