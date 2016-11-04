
#include "commands/UpdateMemberPasswordCommand.h"

using namespace std;
using namespace rapidjson;

namespace rooset {

  const unique_ptr<SchemaDocument> UpdateMemberPasswordCommand::schema = JsonUtils::parseSchema(
      "{\"$schema\":\"http://json-schema.org/draft-04/schema#\",\"id\":\"https://rooset.org/schemas/commands/UPDATE_MEMBER_PASSWORD_COMMAND.schema.json\",\"type\":\"object\",\"definitions\":{\"uuid\":{\"type\":\"string\",\"pattern\":\"[0-9a-f]{8}-[0-9a-f]{4}-[1-5][0-9a-f]{3}-[89ab][0-9a-f]{3}-[0-9a-f]{12}\"},\"string\":{\"type\":\"string\"}},\"properties\":{\"type\":{\"type\":\"string\",\"enum\":[\"UPDATE_MEMBER_PASSWORD_COMMAND\"]},\"payload\":{\"type\":\"object\",\"properties\":{\"id\":{\"$ref\":\"#/definitions/uuid\"},\"requesterId\":{\"$ref\":\"#/definitions/uuid\"},\"oldPassword\":{\"$ref\":\"#/definitions/string\"},\"newPassword\":{\"$ref\":\"#/definitions/string\"}},\"required\":[\"id\",\"requesterId\",\"oldPassword\",\"newPassword\"]}},\"required\":[\"payload\",\"type\"]}");

}

  