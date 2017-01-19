
#include "commands/CreateNewInitiativeCommand.h"

using namespace std;
using namespace rapidjson;

namespace rooset {

  const unique_ptr<SchemaDocument> CreateNewInitiativeCommand::schema = JsonUtils::parseSchema(
      "{\"$schema\":\"http://json-schema.org/draft-04/schema#\",\"id\":\"https://rooset.org/schemas/commands/CREATE_NEW_INITIATIVE_COMMAND.schema.json\",\"type\":\"object\",\"definitions\":{\"uuid\":{\"type\":\"string\",\"pattern\":\"[0-9a-f]{8}-[0-9a-f]{4}-[1-5][0-9a-f]{3}-[89ab][0-9a-f]{3}-[0-9a-f]{12}\"},\"string\":{\"type\":\"string\"},\"bool\":{\"type\":\"boolean\"},\"date\":{\"type\":\"integer\",\"minimum\":0,\"maximum\":4294967295}},\"properties\":{\"type\":{\"type\":\"string\",\"enum\":[\"CREATE_NEW_INITIATIVE_COMMAND\"]},\"payload\":{\"type\":\"object\",\"properties\":{\"id\":{\"$ref\":\"#/definitions/uuid\"},\"requesterId\":{\"$ref\":\"#/definitions/uuid\"},\"initiativeId\":{\"$ref\":\"#/definitions/uuid\"},\"unitId\":{\"$ref\":\"#/definitions/uuid\"},\"areaId\":{\"$ref\":\"#/definitions/uuid\"},\"policyId\":{\"$ref\":\"#/definitions/uuid\"},\"name\":{\"$ref\":\"#/definitions/string\"},\"polling\":{\"$ref\":\"#/definitions/bool\"},\"externalReference\":{\"$ref\":\"#/definitions/string\"},\"content\":{\"$ref\":\"#/definitions/string\"},\"textSearchData\":{\"$ref\":\"#/definitions/string\"},\"created\":{\"$ref\":\"#/definitions/date\"}},\"required\":[\"id\",\"requesterId\",\"initiativeId\",\"unitId\",\"areaId\",\"policyId\",\"name\",\"polling\",\"externalReference\",\"content\",\"textSearchData\",\"created\"]}},\"required\":[\"payload\",\"type\"]}");

}

  