
#include "commands/CreateUnitCommand.h"

using namespace std;
using namespace rapidjson;

namespace rooset {



const SchemaDocument CreateUnitCommand::schema = JsonUtils::parseSchema(
    "{\"$schema\":\"http://json-schema.org/draft-04/schema#\",\"id\":\"https://rooset.org/schemas/commands/CREATE_UNIT_COMMAND.schema.json\",\"type\":\"object\",\"definitions\":{\"uuid\":{\"type\":\"string\",\"pattern\":\"[0-9a-f]{8}-[0-9a-f]{4}-[1-5][0-9a-f]{3}-[89ab][0-9a-f]{3}-[0-9a-f]{12}\"},\"string\":{\"type\":\"string\"}},\"properties\":{\"type\":{\"type\":\"string\",\"enum\":[\"CREATE_UNIT_COMMAND\"]},\"payload\":{\"type\":\"object\",\"properties\":{\"id\":{\"$ref\":\"#/definitions/uuid\",\"description\":\"Unit id\"},\"requesterId\":{\"$ref\":\"#/definitions/uuid\",\"description\":\"Creating Member. Will be made admin\"},\"name\":{\"$ref\":\"#/definitions/string\",\"description\":\"Units short name\"},\"description\":{\"$ref\":\"#/definitions/string\",\"description\":\"Unit description\"},\"urlParameterName\":{\"$ref\":\"#/definitions/string\",\"description\":\"The name that appears in the url (eg my-unit)\"}},\"required\":[\"id\",\"requesterId\",\"name\",\"description\",\"urlParameterName\"]}},\"required\":[\"payload\",\"type\"]}");



}

  