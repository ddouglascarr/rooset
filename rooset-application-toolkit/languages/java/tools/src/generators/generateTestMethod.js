const { camelCase } = require('lodash');


module.exports = (scenario) => {
  const testMethodName = camelCase(scenario.label);
  const when = scenario.when;
  if (when.type !== 'HTTP_REQUEST') throw new Error(
      `Http Tests only support type HTTP_REQUEST, ${when.type} given`);
  
  return `
  @Test
  public void ${testMethodName}() throws Exception
  {
    // set requesting user id and persist to testing user database
    testUser.setId(UUID.fromString("${when.requesterId}"));
    userRepository.save(testUser);

    // set up request
    JSONObject requestBody = new JSONObject(
        ${JSON.stringify(JSON.stringify(when.body || {}))});
    HttpEntity<String> request = new HttpEntity<>(requestBody.toString(), headers);

    // persist given events to testing event store
${persistGivenEvents(scenario.given)}

    // perform request
    ResponseEntity<String> response = ${performRequest(scenario)}
    JSONObject responseBody = new JSONObject(response.getBody());

    // assess response
${assessResponse(scenario.then)}
  }
`;

};


function performRequest(scenario) {
  console.log(JSON.stringify(scenario, null, 2));
  if (scenario.when.method !== 'POST') throw new Error(
      `when.method is ${scenario.when.method} but only POST is supported`);
  return `this.restTemplate.postForEntity(
        "${scenario.when.uri}", request, String.class);`;
}


function persistGivenEvents(given) {
  return given
      .map((event) => `
        testingEventStoreService.saveEvent(new JsonObject(
            ${JSON.stringify(JSON.stringify(event))}));`)
      .join('');
}


function assessResponse(then) {
  // TODO: implment HTTP_RESPONSE assessment
  if (then.type === 'HTTP_RESPONSE') throw new Error("HTTP_RESPONSE assessment not implmemented");
  // TODO: implement error HTTP_RESPONSE assessment
  if (then.error) throw new Error("HTTP_RESPONSE error assessment not implemented");

  return `
      assertTrue(response.getStatusCode().is2xxSuccessful());
      UUID aggregateId = UUID.fromString(responseBody.getString("id"));
      JSONObject event = testingEventStoreService.getLastEventForAggregate(aggregateId);
      String expectedEvent = ${JSON.stringify(JSON.stringify(then))};
      JSONAssert.assertEquals(expectedEvent, event, true);
  `;
}
