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
    // set user details
    testUser.setId(UUID.fromString("${when.requesterId}"));
    userRepository.save(testUser);

    JSONObject requestBody = new JSONObject(
        ${JSON.stringify(JSON.stringify(when.body || {}))});
    HttpEntity<String> request = new HttpEntity<>(requestBody.toString(), headers);
    ResponseEntity<String> resp = ${performRequest(scenario)}
    
    assertEquals(true, false);
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
