
#include "events/ConcernCreatedEvent.h"

using namespace std;
using namespace rapidjson;

namespace rooset {



const SchemaDocument ConcernCreatedEvent::schema = JsonUtils::parseSchema(
    "{\"$schema\":\"http://json-schema.org/draft-04/schema#\",\"id\":\"https://rooset.org/schemas/events/CONCERN_CREATED_EVENT.schema.json\",\"type\":\"object\",\"definitions\":{\"uuid\":{\"type\":\"string\",\"pattern\":\"[0-9a-f]{8}-[0-9a-f]{4}-[1-5][0-9a-f]{3}-[89ab][0-9a-f]{3}-[0-9a-f]{12}\"},\"string\":{\"type\":\"string\"},\"initiativeContentType\":{\"type\":\"string\",\"enum\":[\"STATEMENT\",\"GIT_PATCH\"]},\"jsonObject\":{\"type\":\"object\"}},\"properties\":{\"type\":{\"type\":\"string\",\"enum\":[\"CONCERN_CREATED_EVENT\"]},\"payload\":{\"type\":\"object\",\"properties\":{\"id\":{\"$ref\":\"#/definitions/uuid\",\"description\":\"Unit Id\"},\"concernId\":{\"$ref\":\"#/definitions/uuid\",\"description\":\"\"},\"requesterId\":{\"$ref\":\"#/definitions/uuid\",\"description\":\"\"},\"name\":{\"$ref\":\"#/definitions/string\",\"description\":\"\"},\"description\":{\"$ref\":\"#/definitions/string\",\"description\":\"\"},\"initiativeContentType\":{\"$ref\":\"#/definitions/initiativeContentType\",\"description\":\"The type of content initiatives of this concern\"},\"config\":{\"$ref\":\"#/definitions/jsonObject\",\"description\":\"Content configuration, different for each contentType\"}},\"required\":[\"id\",\"concernId\",\"requesterId\",\"name\",\"description\",\"initiativeContentType\",\"config\"]}},\"required\":[\"payload\",\"type\"],\"commandConstructor\":\"CREATE_CONCERN_COMMAND\"}");



}

  