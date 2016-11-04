
#include "events/MemberPasswordUpdatedEvent.h"

using namespace std;
using namespace rapidjson;

namespace rooset {

  const unique_ptr<SchemaDocument> MemberPasswordUpdatedEvent::schema = JsonUtils::parseSchema(
      "{\"$schema\":\"http://json-schema.org/draft-04/schema#\",\"id\":\"https://rooset.org/schemas/events/MEMBER_PASSWORD_UPDATED_EVENT.schema.json\",\"type\":\"object\",\"definitions\":{\"uuid\":{\"type\":\"string\",\"pattern\":\"[0-9a-f]{8}-[0-9a-f]{4}-[1-5][0-9a-f]{3}-[89ab][0-9a-f]{3}-[0-9a-f]{12}\"},\"string\":{\"type\":\"string\"}},\"properties\":{\"type\":{\"type\":\"string\",\"enum\":[\"MEMBER_PASSWORD_UPDATED_EVENT\"]},\"payload\":{\"type\":\"object\",\"properties\":{\"id\":{\"$ref\":\"#/definitions/uuid\"},\"requesterId\":{\"$ref\":\"#/definitions/uuid\"},\"newPassword\":{\"$ref\":\"#/definitions/string\"}},\"required\":[\"id\",\"requesterId\",\"newPassword\"]}},\"required\":[\"payload\",\"type\"]}");

}

  