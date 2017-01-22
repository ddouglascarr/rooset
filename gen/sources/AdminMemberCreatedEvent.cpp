
#include "events/AdminMemberCreatedEvent.h"

using namespace std;
using namespace rapidjson;

namespace rooset {



const SchemaDocument AdminMemberCreatedEvent::schema = JsonUtils::parseSchema(
    "{\"$schema\":\"http://json-schema.org/draft-04/schema#\",\"id\":\"https://rooset.org/schemas/events/ADMIN_MEMBER_CREATED_EVENT.schema.json\",\"type\":\"object\",\"definitions\":{\"uuid\":{\"type\":\"string\",\"pattern\":\"[0-9a-f]{8}-[0-9a-f]{4}-[1-5][0-9a-f]{3}-[89ab][0-9a-f]{3}-[0-9a-f]{12}\"},\"string\":{\"type\":\"string\"},\"email\":{\"type\":\"string\",\"format\":\"email\"}},\"properties\":{\"type\":{\"type\":\"string\",\"enum\":[\"ADMIN_MEMBER_CREATED_EVENT\"]},\"payload\":{\"type\":\"object\",\"properties\":{\"id\":{\"$ref\":\"#/definitions/uuid\"},\"login\":{\"$ref\":\"#/definitions/string\"},\"password\":{\"$ref\":\"#/definitions/string\"},\"name\":{\"$ref\":\"#/definitions/string\"},\"notifyEmail\":{\"$ref\":\"#/definitions/email\"}},\"required\":[\"id\",\"login\",\"password\",\"name\",\"notifyEmail\"]}},\"required\":[\"payload\",\"type\"]}");



}

  