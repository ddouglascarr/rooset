
#include "events/AreaConcernAddedEvent.h"

using namespace std;
using namespace rapidjson;

namespace rooset {



const SchemaDocument AreaConcernAddedEvent::schema = JsonUtils::parseSchema(
    "{\"$schema\":\"http://json-schema.org/draft-04/schema#\",\"id\":\"https://rooset.org/schemas/events/AREA_CONCERN_ADDED_EVENT.schema.json\",\"type\":\"object\",\"definitions\":{\"uuid\":{\"type\":\"string\",\"pattern\":\"[0-9a-f]{8}-[0-9a-f]{4}-[1-5][0-9a-f]{3}-[89ab][0-9a-f]{3}-[0-9a-f]{12}\"}},\"properties\":{\"type\":{\"type\":\"string\",\"enum\":[\"AREA_CONCERN_ADDED_EVENT\"]},\"payload\":{\"type\":\"object\",\"properties\":{\"id\":{\"$ref\":\"#/definitions/uuid\",\"description\":\"Unit Id\"},\"areaId\":{\"$ref\":\"#/definitions/uuid\",\"description\":\"\"},\"requesterId\":{\"$ref\":\"#/definitions/uuid\",\"description\":\"\"},\"concernId\":{\"$ref\":\"#/definitions/uuid\",\"description\":\"The concern from the parent unit to allow for this area\"}},\"required\":[\"id\",\"areaId\",\"requesterId\",\"concernId\"]}},\"required\":[\"payload\",\"type\"],\"commandConstructor\":\"ADD_AREA_CONCERN_COMMAND\"}");



}

  