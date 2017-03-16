
const fetch = require("node-fetch");
const {
  startEventStore,
  initAllProjections,
  persistEvent,
 } = require('../utils/eventStoreUtils');

describe("Unit Delegations Tests", () => {

  let eventStoreProcess = null;
  beforeEach(() => {
    return startEventStore()
        .then((process) => { eventStoreProcess = process });
  });

  afterEach(() => {
    eventStoreProcess.kill();
  });



    it("Unit and area delegations registered", () => {
      // init projection
      return Promise.resolve()
      .then(() => initAllProjections())

      // persist events
      .then(() => {
          return Promise.resolve()
         .then(() => { return persistEvent({"type":"UNIT_DELEGATION_SET_EVENT","payload":{"id":"464b1ebb-32c1-460c-8e9e-000000000000","trusterId":"464b1ebb-32c1-460c-8e9f-000000000000","trusteeId":"464b1ebb-32c1-460c-8e9f-111111111111"}}); })
       .then(() => { return persistEvent({"type":"AREA_DELEGATION_SET_EVENT","payload":{"id":"464b1ebb-32c1-460c-8e9e-000000000000","areaId":"464b1ebb-32c1-460c-8e9e-222222222222","trusterId":"464b1ebb-32c1-460c-8e9f-000000000000","trusteeId":"464b1ebb-32c1-460c-8e9f-222222222222"}}); })
       .then(() => { return persistEvent({"type":"AREA_DELEGATION_SET_EVENT","payload":{"id":"464b1ebb-32c1-460c-8e9e-000000000000","areaId":"464b1ebb-32c1-460c-8e9e-222222222222","trusterId":"464b1ebb-32c1-460c-8e9f-000000000000","trusteeId":"464b1ebb-32c1-460c-8e9f-333333333333"}}); })
          .then(() => new Promise((r) => { setTimeout(r, 100); }))
          .then((resp) => Promise.resolve({foo: 'bar'}))
      })

      // perform query
      .then(() => {
        const url = "http://localhost:2113/projection/UNIT_DELEGATIONS_QUERY/state?partition=unitDelegations-464b1ebb-32c1-460c-8e9e-000000000000_464b1ebb-32c1-460c-8e9f-000000000000";
        return fetch(url);
      })

      .then((resp) => {
        if (!resp.ok) throw new Error("response failed: " + resp);
        return resp.json();
      })
      .then(function(body) {
        expect(body).toEqual({"unit":{"truteeId":"464b1ebb-32c1-460c-8e9f-111111111111","blocked":false},"areas":{"*area1Id":{"trusteeId":"464b1ebb-32c1-460c-8e9f-222222222222","blocked":false},"*area2Id":{"trusteeId":"464b1ebb-32c1-460c-8e9f-333333333333","blocked":false}}});
        return Promise.resolve();
      });
    });

  

});


