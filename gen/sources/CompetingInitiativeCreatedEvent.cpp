
#include "events/CompetingInitiativeCreatedEvent.h"

using namespace std;
using namespace rapidjson;

namespace rooset {

  const unique_ptr<SchemaDocument> CompetingInitiativeCreatedEvent::schema = JsonUtils::parseSchema(
      "{\"$schema\":\"http://json-schema.org/draft-04/schema#\",\"id\":\"https://rooset.org/schemas/events/COMPETING_INITIATIVE_CREATED_EVENT.schema.json\",\"type\":\"object\",\"definitions\":{\"uuid\":{\"type\":\"string\",\"pattern\":\"[0-9a-f]{8}-[0-9a-f]{4}-[1-5][0-9a-f]{3}-[89ab][0-9a-f]{3}-[0-9a-f]{12}\"},\"string\":{\"type\":\"string\"},\"date\":{\"type\":\"integer\",\"minimum\":0,\"maximum\":4294967295}},\"properties\":{\"type\":{\"type\":\"string\",\"enum\":[\"COMPETING_INITIATIVE_CREATED_EVENT\"]},\"payload\":{\"type\":\"object\",\"properties\":{\"id\":{\"$ref\":\"#/definitions/uuid\"},\"requesterId\":{\"$ref\":\"#/definitions/uuid\"},\"initiativeId\":{\"$ref\":\"#/definitions/uuid\"},\"name\":{\"$ref\":\"#/definitions/string\"},\"externalReference\":{\"$ref\":\"#/definitions/string\"},\"content\":{\"$ref\":\"#/definitions/string\"},\"textSearchData\":{\"$ref\":\"#/definitions/string\"},\"created\":{\"$ref\":\"#/definitions/date\"}},\"required\":[\"id\",\"requesterId\",\"initiativeId\",\"name\",\"externalReference\",\"content\",\"textSearchData\",\"created\"]}},\"required\":[\"payload\",\"type\"],\"commandConstructor\":\"CREATE_COMPETING_INITIATIVE_COMMAND\"}");

}

  