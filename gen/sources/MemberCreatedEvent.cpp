
#include "events/MemberCreatedEvent.h"

using namespace std;
using namespace rapidjson;

namespace rooset {



const SchemaDocument MemberCreatedEvent::schema = JsonUtils::parseSchema(
    "{\"$schema\":\"http://json-schema.org/draft-04/schema#\",\"id\":\"https://rooset.org/schemas/events/MEMBER_CREATED_EVENT.schema.json\",\"type\":\"object\",\"definitions\":{\"uuid\":{\"type\":\"string\",\"pattern\":\"[0-9a-f]{8}-[0-9a-f]{4}-[1-5][0-9a-f]{3}-[89ab][0-9a-f]{3}-[0-9a-f]{12}\"},\"string\":{\"type\":\"string\"},\"email\":{\"type\":\"string\",\"format\":\"email\"},\"date\":{\"type\":\"integer\",\"minimum\":0,\"maximum\":4294967295}},\"properties\":{\"type\":{\"type\":\"string\",\"enum\":[\"MEMBER_CREATED_EVENT\"]},\"payload\":{\"type\":\"object\",\"properties\":{\"id\":{\"$ref\":\"#/definitions/uuid\"},\"login\":{\"$ref\":\"#/definitions/string\"},\"password\":{\"$ref\":\"#/definitions/string\"},\"name\":{\"$ref\":\"#/definitions/string\"},\"notifyEmail\":{\"$ref\":\"#/definitions/email\"},\"activated\":{\"$ref\":\"#/definitions/date\"}},\"required\":[\"id\",\"login\",\"password\",\"name\",\"notifyEmail\",\"activated\"]}},\"required\":[\"payload\",\"type\"]}");



}

  