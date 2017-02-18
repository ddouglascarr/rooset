const {
  applyUriTemplate,
  findDeclarationByType,
} = require('../../../../../ratk-declarations-utils');

module.exports = (testDecl, queryDeclDir, reducerDir) => {

  const testBlocks = testDecl.scenarios.map(generateTestBlock);

  return `
const fetch = require("node-fetch");
const fs = require('fs');
const {
  startEventStore,
  initProjection,
  persistEvent,
 } = require('../utils/eventStoreUtils');

describe("${testDecl.label}", () => {

  let eventStoreProcess = null;
  beforeEach(() => {
    return startEventStore()
        .then((process) => { eventStoreProcess = process });
  });

  afterEach(() => {
    // eventStoreProcess.kill();
  });


${testBlocks.join("\n")}

});


`;


  function generateTestBlock(scenario) {
    const setEvents = scenario.given.precondition.map((event) =>
  `   .then(() => persistEvent(${JSON.stringify(event)}))`);

    const queryType = scenario.when.action.type.split('_REQUEST')[0];
    const queryDecl = findDeclarationByType(queryDeclDir, queryType);

    return `
    it("${scenario.label}", () => {
      // init projection
      const file = fs.readFileSync(__dirname + "/../reducers/${queryType}.js", "utf8");
      if (!file) throw new Error("could not read ${queryType} reducer");
      return Promise.resolve()
      .then(() =>  initProjection("${queryType}", file))

      // persist events
  ${setEvents.join("\n")}

      // perform query
      .then(() => fetch(
        "http://localhost:2113/projection/${queryType}${applyUriTemplate(queryDecl.projectionUri, { id: scenario.then.outcome.payload.id })}"
      ))

      .then((resp) => {
        if (!resp.ok) throw new Error("response failed: " + resp);
        expect(resp.json()).toEqual(${JSON.stringify(scenario.then.outcome)});
        return resp.json();
      })
      .then((body) => {
        expect(body).toEqual(${JSON.stringify(scenario.then.outcome)});
      });
    });

  `;


  }

};


