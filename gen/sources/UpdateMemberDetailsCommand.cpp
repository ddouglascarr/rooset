
#include "commands/UpdateMemberDetailsCommand.h"

using namespace std;
using namespace rapidjson;

namespace rooset {

  const unique_ptr<SchemaDocument> UpdateMemberDetailsCommand::schema = JsonUtils::parseSchema(
      "{\"$schema\":\"http://json-schema.org/draft-04/schema#\",\"id\":\"https://rooset.org/schemas/commands/UPDATE_MEMBER_DETAILS_COMMAND.schema.json\",\"type\":\"object\",\"definitions\":{\"uuid\":{\"type\":\"string\",\"pattern\":\"[0-9a-f]{8}-[0-9a-f]{4}-[1-5][0-9a-f]{3}-[89ab][0-9a-f]{3}-[0-9a-f]{12}\"},\"email\":{\"type\":\"string\",\"format\":\"email\"}},\"properties\":{\"type\":{\"type\":\"string\",\"enum\":[\"UPDATE_MEMBER_DETAILS_COMMAND\"]},\"payload\":{\"type\":\"object\",\"properties\":{\"id\":{\"$ref\":\"#/definitions/uuid\"},\"notifyEmail\":{\"$ref\":\"#/definitions/email\"}},\"required\":[\"id\",\"notifyEmail\"]}},\"required\":[\"payload\",\"type\"]}");

}

  