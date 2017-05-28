
#include "events/NewInitiativeCreatedEvent.h"

using namespace std;
using namespace rapidjson;

namespace rooset {



const SchemaDocument NewInitiativeCreatedEvent::schema = JsonUtils::parseSchema(
    "{\"$schema\":\"http://json-schema.org/draft-04/schema#\",\"id\":\"https://rooset.org/schemas/events/NEW_INITIATIVE_CREATED_EVENT.schema.json\",\"type\":\"object\",\"definitions\":{\"uuid\":{\"type\":\"string\",\"pattern\":\"[0-9a-f]{8}-[0-9a-f]{4}-[1-5][0-9a-f]{3}-[89ab][0-9a-f]{3}-[0-9a-f]{12}\"},\"string\":{\"type\":\"string\"},\"date\":{\"type\":\"integer\",\"minimum\":0,\"maximum\":4294967295}},\"properties\":{\"type\":{\"type\":\"string\",\"enum\":[\"NEW_INITIATIVE_CREATED_EVENT\"]},\"payload\":{\"type\":\"object\",\"properties\":{\"id\":{\"$ref\":\"#/definitions/uuid\",\"description\":\"\"},\"requesterId\":{\"$ref\":\"#/definitions/uuid\",\"description\":\"\"},\"initiativeId\":{\"$ref\":\"#/definitions/uuid\",\"description\":\"\"},\"unitId\":{\"$ref\":\"#/definitions/uuid\",\"description\":\"\"},\"areaId\":{\"$ref\":\"#/definitions/uuid\",\"description\":\"\"},\"policyId\":{\"$ref\":\"#/definitions/uuid\",\"description\":\"\"},\"concernId\":{\"$ref\":\"#/definitions/uuid\",\"description\":\"\"},\"name\":{\"$ref\":\"#/definitions/string\",\"description\":\"\"},\"content\":{\"$ref\":\"#/definitions/string\",\"description\":\"\"},\"created\":{\"$ref\":\"#/definitions/date\",\"description\":\"\"}},\"required\":[\"id\",\"requesterId\",\"initiativeId\",\"unitId\",\"areaId\",\"policyId\",\"concernId\",\"name\",\"content\",\"created\"]}},\"required\":[\"payload\",\"type\"],\"commandConstructor\":\"CREATE_NEW_INITIATIVE_COMMAND\"}");



}

  