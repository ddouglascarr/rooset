const {
  applyUriTemplate,
  findDeclarationByType,
} = require('ratk-declaration-utils');
const fs = require('fs');

module.exports = (testDecl, queryDeclDir, reducerDir) => {

  const testBlocks = testDecl.scenarios.map(generateTestBlock);

  return `
const fetch = require("node-fetch");
const {
  startEventStore,
  initAllProjections,
  persistEvent,
 } = require('../utils/eventStoreUtils');

describe("${testDecl.label}", () => {

  let eventStoreProcess = null;
  beforeEach(() => {
    return startEventStore()
        .then((process) => { eventStoreProcess = process });
  });

  afterEach(() => {
    eventStoreProcess.kill();
  });


${testBlocks.join("\n")}

});


`;


  function generateTestBlock(scenario) {
    const setEvents = scenario.given.precondition.map((event) =>
  `       .then(() => { return persistEvent(${JSON.stringify(event)}); })`);

    const queryType = scenario.when.action.type.split('_REQUEST')[0];
    const queryDecl = findDeclarationByType(queryDeclDir, queryType);

    return `
    it("${scenario.label}", () => {
      // init projection
      return Promise.resolve()
      .then(() => initAllProjections())

      // persist events
      .then(() => {
          return Promise.resolve()
  ${setEvents.join("\n")}
          .then(() => new Promise((r) => { setTimeout(r, 300); }))
          .then((resp) => Promise.resolve({foo: 'bar'}))
      })

      // perform query
      .then(() => {
        const url = "http://localhost:2113/projection/${queryType}${applyUriTemplate(queryDecl.projectionUri, scenario.when.action.payload )}";
        return fetch(url);
      })

      .then((resp) => {
        if (!resp.ok) throw new Error("response failed: " + resp);
        return resp.json();
      })
      .then(function(body) {
        expect(body).toEqual(${JSON.stringify(scenario.then.outcome.payload)});
        return Promise.resolve();
      });
    });

  `;


  }

};
