
#include "events/DelegationUnblockedForAreaEvent.h"

using namespace std;
using namespace rapidjson;

namespace rooset {



const SchemaDocument DelegationUnblockedForAreaEvent::schema = JsonUtils::parseSchema(
    "{\"$schema\":\"http://json-schema.org/draft-04/schema#\",\"id\":\"https://rooset.org/schemas/events/DELEGATION_UNBLOCKED_FOR_AREA_EVENT.schema.json\",\"type\":\"object\",\"definitions\":{\"uuid\":{\"type\":\"string\",\"pattern\":\"[0-9a-f]{8}-[0-9a-f]{4}-[1-5][0-9a-f]{3}-[89ab][0-9a-f]{3}-[0-9a-f]{12}\"}},\"properties\":{\"type\":{\"type\":\"string\",\"enum\":[\"DELEGATION_UNBLOCKED_FOR_AREA_EVENT\"]},\"payload\":{\"type\":\"object\",\"properties\":{\"id\":{\"$ref\":\"#/definitions/uuid\",\"description\":\"\"},\"areaId\":{\"$ref\":\"#/definitions/uuid\",\"description\":\"\"},\"trusterId\":{\"$ref\":\"#/definitions/uuid\",\"description\":\"\"}},\"required\":[\"id\",\"areaId\",\"trusterId\"]}},\"required\":[\"payload\",\"type\"]}");



}

  