
#include "events/AreaDelegationSetEvent.h"

using namespace std;
using namespace rapidjson;

namespace rooset {



const SchemaDocument AreaDelegationSetEvent::schema = JsonUtils::parseSchema(
    "{\"$schema\":\"http://json-schema.org/draft-04/schema#\",\"id\":\"https://rooset.org/schemas/events/AREA_DELEGATION_SET_EVENT.schema.json\",\"type\":\"object\",\"definitions\":{\"uuid\":{\"type\":\"string\",\"pattern\":\"[0-9a-f]{8}-[0-9a-f]{4}-[1-5][0-9a-f]{3}-[89ab][0-9a-f]{3}-[0-9a-f]{12}\"}},\"properties\":{\"type\":{\"type\":\"string\",\"enum\":[\"AREA_DELEGATION_SET_EVENT\"]},\"payload\":{\"type\":\"object\",\"properties\":{\"id\":{\"$ref\":\"#/definitions/uuid\",\"description\":\"\"},\"areaId\":{\"$ref\":\"#/definitions/uuid\",\"description\":\"\"},\"trusterId\":{\"$ref\":\"#/definitions/uuid\",\"description\":\"\"},\"trusteeId\":{\"$ref\":\"#/definitions/uuid\",\"description\":\"\"}},\"required\":[\"id\",\"areaId\",\"trusterId\",\"trusteeId\"]}},\"required\":[\"payload\",\"type\"]}");



}

  