
#include "events/ConcernDeactivatedEvent.h"

using namespace std;
using namespace rapidjson;

namespace rooset {



const SchemaDocument ConcernDeactivatedEvent::schema = JsonUtils::parseSchema(
    "{\"$schema\":\"http://json-schema.org/draft-04/schema#\",\"id\":\"https://rooset.org/schemas/events/CONCERN_DEACTIVATED_EVENT.schema.json\",\"type\":\"object\",\"definitions\":{\"uuid\":{\"type\":\"string\",\"pattern\":\"[0-9a-f]{8}-[0-9a-f]{4}-[1-5][0-9a-f]{3}-[89ab][0-9a-f]{3}-[0-9a-f]{12}\"}},\"properties\":{\"type\":{\"type\":\"string\",\"enum\":[\"CONCERN_DEACTIVATED_EVENT\"]},\"payload\":{\"type\":\"object\",\"properties\":{\"id\":{\"$ref\":\"#/definitions/uuid\",\"description\":\"Unit Id\"},\"requesterId\":{\"$ref\":\"#/definitions/uuid\",\"description\":\"\"},\"concernId\":{\"$ref\":\"#/definitions/uuid\",\"description\":\"\"}},\"required\":[\"id\",\"requesterId\",\"concernId\"]}},\"required\":[\"payload\",\"type\"],\"commandConstructor\":\"DEACTIVATE_CONCERN_COMMAND\"}");



}

  