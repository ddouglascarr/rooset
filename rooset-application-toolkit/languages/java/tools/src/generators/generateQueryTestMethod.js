const { camelCase } = require('lodash');
const {
  applyUriTemplate,
  findDeclarationByType,
} = require('ratk-declaration-utils');


module.exports = (scenario, config) => {

  const testMethodName = camelCase(scenario.label);
  const queryType = scenario.when.action.type.split('_REQUEST')[0];
  const queryDecl = findDeclarationByType(config.queryDeclDir, queryType);
  const uri = applyUriTemplate(queryDecl.httpUri, scenario.when.action.payload);

  return `
  @Test
  public void ${testMethodName}() throws Exception
  {
    userRepository.save(testUser);

    // persist given events to testing event store
${persistGivenEvents(scenario.given.precondition)}

    // perform request
    ResponseEntity<String> response = this.restTemplate.getForEntity(
        "${uri}", String.class);
    JSONObject responseBody = new JSONObject(response.getBody());

    // assess response
    assertTrue("2xx response expected, got " + response.getStatusCode().toString(), response.getStatusCode().is2xxSuccessful());
    String expectedResponseBody = ${JSON.stringify(JSON.stringify(
        scenario.then.outcome.payload))};
    JSONAssert.assertEquals(expectedResponseBody, responseBody, true);
  }
`;
};


function persistGivenEvents(given) {
  return given
      .map((event) => `
        testingEventStoreService.saveEvent(new JSONObject(
            ${JSON.stringify(JSON.stringify(event))}));`)
      .join('');
}
