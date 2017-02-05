const { camelCase, map } = require('lodash');


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

    // set up generators
${mockUniqueIdGenerator(when)}

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
        testingEventStoreService.saveEvent(new JSONObject(
            ${JSON.stringify(JSON.stringify(event))}));`)
      .join('');
}


function assessResponse(then) {
  // TODO: implment HTTP_RESPONSE assessment
  if (then.type === 'HTTP_RESPONSE') throw new Error("HTTP_RESPONSE assessment not implmemented");

  if (then.error) {
    return `
    assertTrue(response.getStatusCode().is4xxClientError());
    String expectedEvent = ${JSON.stringify(JSON.stringify(then))};
    JSONAssert.assertEquals(expectedEvent, responseBody, true);
    `;
  }

  return `
    assertTrue(response.getStatusCode().is2xxSuccessful());
    UUID aggregateId = UUID.fromString(responseBody.getString("id"));
    JSONObject event = testingEventStoreService.getLastEventForAggregate(aggregateId);
    String expectedEvent = ${JSON.stringify(JSON.stringify(then))};
    JSONAssert.assertEquals(expectedEvent, event, true);
  `;
}


function mockUniqueIdGenerator(when) {
  if (!when.generate || Object.keys(when.generate).length === 0) return '';
  // TODO: Support for more than one generated id
  if (Object.keys(when.generate).length !== 1) throw new Error(
      `support for generating more than 1 unique id per test not implmented`);
  const thenReturnArgs = map(when.generate, (v) => `UUID.fromString("${v}")`)
      .join(',\n');
  return `
    when(idService.generateUniqueId()).thenReturn(
        ${thenReturnArgs});`;
}
