const { camelCase } = require('lodash');


module.exports = (scenario) => {
  const testMethodName = camelCase(scenario.label);

  return `
  @Test
  public void ${testMethodName}() throws Exception
  {
    assertEquals(true, false);
  }
`;

};
