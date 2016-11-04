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
#include "framework/AggregateRepositoryMockImpl.h"
#include "framework/AggregateRepositoryEmptyMockImpl.h"
#include "framework/JsonUtils.h"
#include "framework/IdToolsImpl.h"
#include "exceptions/CommandEvaluationException.h"
#include "aggregates/${aggregateClassname}.h"
#include "aggregates/${commandHandlerClassname}.h"

using namespace std;
using namespace rooset;

namespace rooset_${testCategoryLabel}_tests {

${testBlocks.join('\n\n')}

}`;
}
