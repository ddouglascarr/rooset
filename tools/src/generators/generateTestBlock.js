const {
  generateClassnameFromMsgType,
  generateTestLabel,
} = require('../utils');
const { head, tail } = require('lodash');

module.exports = function(
    testCategoryLabel, aggregateClassname, commandHandlerClassname, scenario) {
  const hasGivenEvents = scenario.given.length > 0;
  const isExceptionExpected = scenario.then.error === true;
  const doIgnoreMsg = isExceptionExpected ? !scenario.then.payload.message : false;

  const instantiateMessage = (instanceName, msgJson) => {
    const classname = generateClassnameFromMsgType(msgJson.type);
    const jsonStr = JSON.stringify(msgJson, null, 2);

    return `
  auto ${instanceName}_doc = JsonUtils::parse(u8R"json(${jsonStr})json");
  try {
  JsonUtils::validate(*${classname}::schema, *${instanceName}_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("${instanceName} schema invalid");
  }
  ${classname} ${instanceName}(*${instanceName}_doc);`;
  };

  const stripMsgFromExpectedErr = () => {
    if (isExceptionExpected && doIgnoreMsg) {
      return `
  // ignore the message from the test
  (*expectedDoc)["payload"].RemoveMember("message");
  (*resultDoc)["payload"].RemoveMember("message");
      `;
    }
    return '';
  };

  const pushGivenEvents = () => {
    const statements = scenario.given.map((event) => `
  givenEvents.push_back(JsonUtils::parse(u8R"json(${JSON.stringify(event, null, 2)})json"));`);
    return statements.join('');
  };

  const buildCommandHandler = () => `
  vector<unique_ptr<Document>> givenEvents;
  ${pushGivenEvents()}
  CommandHandlerTestImpl commandHandler(givenEvents); `;

  const generateAssertion = () => {
    return `
  // if docs don't match, assess the json output to make useful error report
  auto expectedDoc = expected.serialize();
  ${stripMsgFromExpectedErr()}
  bool isPass = *resultDoc == *expectedDoc;
  if (isPass) {
    EXPECT_EQ(*resultDoc, *expectedDoc);
  }  else {
    EXPECT_EQ(*JsonUtils::serialize(*resultDoc),
        *JsonUtils::serialize(*expectedDoc));
  };`;
  };

  const generateExpectedEventBlock = () => {
    return `
  auto result = commandHandler.evaluate(cmd);
  if (result == nullptr) throw invalid_argument("command handler returned nullptr");
  auto resultDoc = result->serialize();
  ${generateAssertion()}`;
  };

  const generateExpectedExceptionBlock = () => {
    return `
  try {
    commandHandler.evaluate(cmd);
    throw invalid_argument("An exception was expected");
  } catch(CommandEvaluationException e) {
    auto resultDoc = e.serialize();
    ${generateAssertion()}
  }`;
  };


  return `
TEST(${testCategoryLabel}, ${generateTestLabel(scenario.label)})
{
  ${buildCommandHandler()}
  ${instantiateMessage('expected', scenario.then)}
  ${instantiateMessage('cmd', scenario.when)}
  ${isExceptionExpected ? generateExpectedExceptionBlock() : generateExpectedEventBlock()}
}`;

}
