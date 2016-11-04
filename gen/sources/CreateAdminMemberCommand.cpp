
#include "commands/CreateAdminMemberCommand.h"

using namespace std;
using namespace rapidjson;

namespace rooset {

  const unique_ptr<SchemaDocument> CreateAdminMemberCommand::schema = JsonUtils::parseSchema(
      "{\"$schema\":\"http://json-schema.org/draft-04/schema#\",\"id\":\"https://rooset.org/schemas/commands/CREATE_ADMIN_MEMBER_COMMAND.schema.json\",\"type\":\"object\",\"definitions\":{\"uuid\":{\"type\":\"string\",\"pattern\":\"[0-9a-f]{8}-[0-9a-f]{4}-[1-5][0-9a-f]{3}-[89ab][0-9a-f]{3}-[0-9a-f]{12}\"},\"string\":{\"type\":\"string\"},\"email\":{\"type\":\"string\",\"format\":\"email\"}},\"properties\":{\"type\":{\"type\":\"string\",\"enum\":[\"CREATE_ADMIN_MEMBER_COMMAND\"]},\"payload\":{\"type\":\"object\",\"properties\":{\"id\":{\"$ref\":\"#/definitions/uuid\"},\"login\":{\"$ref\":\"#/definitions/string\"},\"password\":{\"$ref\":\"#/definitions/string\"},\"name\":{\"$ref\":\"#/definitions/string\"},\"notifyEmail\":{\"$ref\":\"#/definitions/email\"}},\"required\":[\"id\",\"login\",\"password\",\"name\",\"notifyEmail\"]}},\"required\":[\"payload\",\"type\"]}");

}

  