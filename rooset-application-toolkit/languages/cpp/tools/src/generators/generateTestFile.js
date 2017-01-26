const { snakeCase } = require('lodash');
const generateTestBlock = require('./generateTestBlock');


module.exports = function(testDoc) {
  const aggregateClassname = testDoc.aggregate;
  const commandHandlerClassname = testDoc.commandHandler;
  const testCategoryLabel = snakeCase(testDoc.label);
  const testBlocks = testDoc.scenarios.map((scenario) => generateTestBlock(
      testCategoryLabel, aggregateClassname, commandHandlerClassname, scenario));

  return `
#include "gtest/gtest.h"

#include <string>
#include <memory>
#include "ratk/JsonUtils.h"
#include "ratk/IdToolsImpl.h"
#include "ratk/EventRepositoryMockImpl.h"
#include "aggregates/CommandHandler.h"
#include "exceptions/CommandEvaluationException.h"

using namespace std;
using namespace rooset;
using ::testing::NiceMock;

namespace rooset_${testCategoryLabel}_tests {

${testBlocks.join('\n\n')}

}`;
}
