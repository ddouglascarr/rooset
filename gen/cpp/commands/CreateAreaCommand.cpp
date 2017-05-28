
#include "commands/CreateAreaCommand.h"

using namespace std;
using namespace rapidjson;

namespace rooset {



const SchemaDocument CreateAreaCommand::schema = JsonUtils::parseSchema(
    "{\"$schema\":\"http://json-schema.org/draft-04/schema#\",\"id\":\"https://rooset.org/schemas/commands/CREATE_AREA_COMMAND.schema.json\",\"type\":\"object\",\"definitions\":{\"uuid\":{\"type\":\"string\",\"pattern\":\"[0-9a-f]{8}-[0-9a-f]{4}-[1-5][0-9a-f]{3}-[89ab][0-9a-f]{3}-[0-9a-f]{12}\"},\"string\":{\"type\":\"string\"}},\"properties\":{\"type\":{\"type\":\"string\",\"enum\":[\"CREATE_AREA_COMMAND\"]},\"payload\":{\"type\":\"object\",\"properties\":{\"id\":{\"$ref\":\"#/definitions/uuid\",\"description\":\"\"},\"areaId\":{\"$ref\":\"#/definitions/uuid\",\"description\":\"\"},\"requesterId\":{\"$ref\":\"#/definitions/uuid\",\"description\":\"\"},\"name\":{\"$ref\":\"#/definitions/string\",\"description\":\"\"},\"description\":{\"$ref\":\"#/definitions/string\",\"description\":\"\"},\"externalReference\":{\"$ref\":\"#/definitions/string\",\"description\":\"\"}},\"required\":[\"id\",\"areaId\",\"requesterId\",\"name\",\"description\",\"externalReference\"]}},\"required\":[\"payload\",\"type\"]}");



}

  