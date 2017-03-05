const { camelCase, map, merge, chain } = require('lodash');
const {
  findHttpCommandRequestByUri,
  findCommandSchemaByType,
} = require('ratk-declaration-utils');
const { getTypenameFromRef } = require('../utils');


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

${mockGenerators(scenario)}

${declareRequest(when)}

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
  const supportedMethods = [ 'POST', 'DELETE' ];
  switch (scenario.when.method) {
    case ('PUT'):
      return `this.restTemplate.exchange("${scenario.when.uri}", HttpMethod.PUT, request, String.class);`;
    case ('POST'):
      return `this.restTemplate.postForEntity("${scenario.when.uri}", request, String.class);`;
    case ('DELETE'):
      return `this.restTemplate.exchange("${scenario.when.uri}", HttpMethod.DELETE, request, String.class);`;
    default:
      throw new Error(
          `when.method is ${scenario.when.method} but only PUT, POST, DELETE are supported`);
  }
}


function declareRequest(when) {
  if (when.body) {
    return `
    JSONObject requestBody = new JSONObject(
        ${JSON.stringify(JSON.stringify(when.body || {}))});
    HttpEntity<String> request = new HttpEntity<>(requestBody.toString(), headers);`;
  } else {
    return `
    HttpEntity<String> request = new HttpEntity<>(headers);`;

  }
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
    assertTrue("4xx response expected", response.getStatusCode().is4xxClientError());
    String expectedEvent = ${JSON.stringify(JSON.stringify(then))};
    ${stripMessageFromExceptionResponseBody(then)}
    JSONAssert.assertEquals(expectedEvent, responseBody, true);
    `;
  }

  return `
    assertTrue("2xx response expected", response.getStatusCode().is2xxSuccessful());
    UUID aggregateId = UUID.fromString(responseBody.getString("id"));
    JSONObject event = testingEventStoreService.getLastEventForAggregate(aggregateId);
    String expectedEvent = ${JSON.stringify(JSON.stringify(then))};
    JSONAssert.assertEquals(expectedEvent, event, true);
  `;
}


function mockGenerators(scenario) {
  const when = scenario.when;
  const generate = when.generate;
  if (!generate) return '    // nothing to generate';
  const generateKeys = Object.keys(generate);

  const httpDecl = findHttpCommandRequestByUri(when.uri, when.method);
  if (!httpDecl) throw new Error(`cant find httpDecl ${when.method} ${when.uri}`);
  const commandSchema = findCommandSchemaByType(httpDecl.commandTarget);
  if (!commandSchema) throw new Error(`cant find commandSchema ${httpDecl.commandTarget}`);
  const generateCalls = chain(commandSchema.properties.payload.properties)
      .map((ref, v) => generateKeys.indexOf(v) !== -1 ? [v, getTypenameFromRef(ref)] : null)
      .filter((type) => type !== null)
      .groupBy((a) => a[1])
      .value();

  const generations =  map(generateCalls, (calls, type) => {
        let call = null;
        if (type === 'uuid') {
          calls = map(calls, (call) => `UUID.fromString("${generate[call[0]]}")`).join(', ');
          return `when(idService.generateUniqueId()).thenReturn(${calls});`;
        }
        if (type === 'date') {
          calls = map(calls, (call) => generate[call[0]]).map((n) => `${n}L`).join(', ');
          return `when(dateService.getNow()).thenReturn(${calls});`;
        }
        throw new Error(`no generateor for type ${type} of key ${calls[o]}`);
      })
      .join('\n    ');

  return `
    // generate mock values
    ${generations}`;
}


function stripMessageFromExceptionResponseBody(then) {
  // if the expected exception message === '', do not require a match
  const doStripMessage = (then.payload.message === '');
  if (!doStripMessage) return '';
  return `
    // strip message from exception payload
    JSONObject newPayload = responseBody.getJSONObject("payload");
    newPayload.put("message", "");
    responseBody.put("payload", newPayload);
  `;
}
