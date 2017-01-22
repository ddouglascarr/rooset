
#include "commands/GrantPrivilegeCommand.h"

using namespace std;
using namespace rapidjson;

namespace rooset {



const SchemaDocument GrantPrivilegeCommand::schema = JsonUtils::parseSchema(
    "{\"$schema\":\"http://json-schema.org/draft-04/schema#\",\"id\":\"https://rooset.org/schemas/commands/GRANT_PRIVILEGE_COMMAND.schema.json\",\"type\":\"object\",\"definitions\":{\"uuid\":{\"type\":\"string\",\"pattern\":\"[0-9a-f]{8}-[0-9a-f]{4}-[1-5][0-9a-f]{3}-[89ab][0-9a-f]{3}-[0-9a-f]{12}\"},\"bool\":{\"type\":\"boolean\"},\"uint\":{\"type\":\"integer\",\"minimum\":0,\"maximum\":65535}},\"properties\":{\"type\":{\"type\":\"string\",\"enum\":[\"GRANT_PRIVILEGE_COMMAND\"]},\"payload\":{\"type\":\"object\",\"properties\":{\"id\":{\"$ref\":\"#/definitions/uuid\"},\"requesterId\":{\"$ref\":\"#/definitions/uuid\"},\"memberId\":{\"$ref\":\"#/definitions/uuid\"},\"pollingRight\":{\"$ref\":\"#/definitions/bool\"},\"votingRight\":{\"$ref\":\"#/definitions/bool\"},\"initiativeRight\":{\"$ref\":\"#/definitions/bool\"},\"managementRight\":{\"$ref\":\"#/definitions/bool\"},\"weight\":{\"$ref\":\"#/definitions/uint\"}},\"required\":[\"id\",\"requesterId\",\"memberId\",\"pollingRight\",\"votingRight\",\"initiativeRight\",\"managementRight\",\"weight\"]}},\"required\":[\"payload\",\"type\"]}");



}

  