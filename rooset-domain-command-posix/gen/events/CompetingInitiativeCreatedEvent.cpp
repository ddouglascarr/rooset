
#include "events/CompetingInitiativeCreatedEvent.h"

using namespace std;
using namespace rapidjson;

namespace rooset {



const SchemaDocument CompetingInitiativeCreatedEvent::schema = JsonUtils::parseSchema(
    "{\"$schema\":\"http://json-schema.org/draft-04/schema#\",\"id\":\"https://rooset.org/schemas/events/COMPETING_INITIATIVE_CREATED_EVENT.schema.json\",\"type\":\"object\",\"definitions\":{\"uuid\":{\"type\":\"string\",\"pattern\":\"[0-9a-f]{8}-[0-9a-f]{4}-[1-5][0-9a-f]{3}-[89ab][0-9a-f]{3}-[0-9a-f]{12}\"},\"string\":{\"type\":\"string\"},\"date\":{\"type\":\"integer\",\"minimum\":0,\"maximum\":4294967295}},\"properties\":{\"type\":{\"type\":\"string\",\"enum\":[\"COMPETING_INITIATIVE_CREATED_EVENT\"]},\"payload\":{\"type\":\"object\",\"properties\":{\"id\":{\"$ref\":\"#/definitions/uuid\",\"description\":\"\"},\"requesterId\":{\"$ref\":\"#/definitions/uuid\",\"description\":\"\"},\"initiativeId\":{\"$ref\":\"#/definitions/uuid\",\"description\":\"\"},\"name\":{\"$ref\":\"#/definitions/string\",\"description\":\"\"},\"externalReference\":{\"$ref\":\"#/definitions/string\",\"description\":\"\"},\"content\":{\"$ref\":\"#/definitions/string\",\"description\":\"\"},\"textSearchData\":{\"$ref\":\"#/definitions/string\",\"description\":\"\"},\"created\":{\"$ref\":\"#/definitions/date\",\"description\":\"\"}},\"required\":[\"id\",\"requesterId\",\"initiativeId\",\"name\",\"externalReference\",\"content\",\"textSearchData\",\"created\"]}},\"required\":[\"payload\",\"type\"],\"commandConstructor\":\"CREATE_COMPETING_INITIATIVE_COMMAND\"}");



}

  