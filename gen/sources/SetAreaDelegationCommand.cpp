
#include "commands/SetAreaDelegationCommand.h"

using namespace std;
using namespace rapidjson;

namespace rooset {



const SchemaDocument SetAreaDelegationCommand::schema = JsonUtils::parseSchema(
    "{\"$schema\":\"http://json-schema.org/draft-04/schema#\",\"id\":\"https://rooset.org/schemas/commands/SET_AREA_DELEGATION_COMMAND.schema.json\",\"type\":\"object\",\"definitions\":{\"uuid\":{\"type\":\"string\",\"pattern\":\"[0-9a-f]{8}-[0-9a-f]{4}-[1-5][0-9a-f]{3}-[89ab][0-9a-f]{3}-[0-9a-f]{12}\"}},\"properties\":{\"type\":{\"type\":\"string\",\"enum\":[\"SET_AREA_DELEGATION_COMMAND\"]},\"payload\":{\"type\":\"object\",\"properties\":{\"id\":{\"$ref\":\"#/definitions/uuid\"},\"areaId\":{\"$ref\":\"#/definitions/uuid\"},\"requesterId\":{\"$ref\":\"#/definitions/uuid\"},\"trusteeId\":{\"$ref\":\"#/definitions/uuid\"}},\"required\":[\"id\",\"areaId\",\"requesterId\",\"trusteeId\"]}},\"required\":[\"payload\",\"type\"]}");



}

  