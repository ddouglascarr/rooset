const {
  getTypenameFromRef,
  getMsgTypeFromSchema,
  generateClassnameFromMsgType,
} = require('../utils');


module.exports = function(schema, messageCategory) {
  const messageType = getMsgTypeFromSchema(schema);
  const classTypename = generateClassnameFromMsgType(messageType);

  return `
#include "${messageCategory}/${classTypename}.h"

using namespace std;
using namespace rapidjson;

namespace rooset {

  const unique_ptr<SchemaDocument> ${classTypename}::schema = JsonUtils::parseSchema(
      ${JSON.stringify(JSON.stringify(schema))});

}

  `;
};
