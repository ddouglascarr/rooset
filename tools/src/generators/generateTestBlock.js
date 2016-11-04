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
    const jsonStr = JSON.stringify(JSON.stringify(msgJson));

    return `
  auto ${instanceName}_doc = JsonUtils::parse(${jsonStr});
  try {
  JsonUtils::validate(*${classname}::schema, *${instanceName}_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("${instanceName} schema invalid");
  }
  ${classname} ${instanceName}(*${instanceName}_doc);`;
  };

  const instantiateMockAggregate = () => {
    const firstEvent = head(scenario.given);
    const otherEvents = tail(scenario.given);
    const instantiation = `
  ${instantiateMessage('firstEvent', firstEvent)}
  ${aggregateClassname} mockAggregate(firstEvent);`;

    return otherEvents.reduce((result, evt, idx) => {
      const instanceName = `evt${idx}`;
      return `
  ${result}
  ${instantiateMessage(instanceName, evt)}
  mockAggregate.handleEvent(${instanceName});
  `;
    }, instantiation);
  };

  const stripMsgFromExpectedErr = () => {
    if (isExceptionExpected && doIgnoreMsg) {
      return `
        (*expectedDoc)["payload"].RemoveMember("message");
        (*resultDoc)["payload"].RemoveMember("message");
      `;
    }
    return '';
  };

  const instantiateMockRepo = () => {
    if (hasGivenEvents) {
      return `
  ${instantiateMockAggregate()}
  auto repo = make_unique<AggregateRepositoryMockImpl<
      ${aggregateClassname}>>(mockAggregate);`;
    }
    return `
  auto repo = make_unique<AggregateRepositoryEmptyMockImpl<
      ${aggregateClassname}>>();`;
  };

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
  auto resultDoc = result->serialize();
  ${generateAssertion()}`;
  };

  const generateExpectedExceptionBlock = () => {
    return `
  try {
    commandHandler.evaluate(cmd);
    throw string("An exception was expected");
  } catch(CommandEvaluationException e) {
    auto resultDoc = e.serialize();
    ${generateAssertion()}
  }`;
  };


  return `
TEST(${testCategoryLabel}, ${generateTestLabel(scenario.label)})
{
  ${instantiateMockRepo()}
  ${commandHandlerClassname} commandHandler(move(repo));
  ${instantiateMessage('expected', scenario.then)}
  ${instantiateMessage('cmd', scenario.when)}
  ${isExceptionExpected ? generateExpectedExceptionBlock() : generateExpectedEventBlock()}
}
`;

}
