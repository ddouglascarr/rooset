
#include "events/InitiativeSupportRevokedEvent.h"

using namespace std;
using namespace rapidjson;

namespace rooset {



const SchemaDocument InitiativeSupportRevokedEvent::schema = JsonUtils::parseSchema(
    "{\"$schema\":\"http://json-schema.org/draft-04/schema#\",\"id\":\"https://rooset.org/schemas/events/INITIATIVE_SUPPORT_REVOKED_EVENT.schema.json\",\"type\":\"object\",\"definitions\":{\"uuid\":{\"type\":\"string\",\"pattern\":\"[0-9a-f]{8}-[0-9a-f]{4}-[1-5][0-9a-f]{3}-[89ab][0-9a-f]{3}-[0-9a-f]{12}\"}},\"properties\":{\"type\":{\"type\":\"string\",\"enum\":[\"INITIATIVE_SUPPORT_REVOKED_EVENT\"]},\"payload\":{\"type\":\"object\",\"properties\":{\"id\":{\"$ref\":\"#/definitions/uuid\"},\"requesterId\":{\"$ref\":\"#/definitions/uuid\"},\"initiativeId\":{\"$ref\":\"#/definitions/uuid\"}},\"required\":[\"id\",\"requesterId\",\"initiativeId\"]}},\"required\":[\"payload\",\"type\"],\"commandConstructor\":\"REVOKE_INITIATIVE_SUPPORT_COMMAND\"}");



}

  