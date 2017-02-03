
#include "events/PrivilegeGrantedEvent.h"

using namespace std;
using namespace rapidjson;

namespace rooset {



const SchemaDocument PrivilegeGrantedEvent::schema = JsonUtils::parseSchema(
    "{\"$schema\":\"http://json-schema.org/draft-04/schema#\",\"id\":\"https://rooset.org/schemas/events/PRIVILEGE_GRANTED_EVENT.schema.json\",\"type\":\"object\",\"definitions\":{\"uuid\":{\"type\":\"string\",\"pattern\":\"[0-9a-f]{8}-[0-9a-f]{4}-[1-5][0-9a-f]{3}-[89ab][0-9a-f]{3}-[0-9a-f]{12}\"},\"bool\":{\"type\":\"boolean\"},\"uint\":{\"type\":\"integer\",\"minimum\":0,\"maximum\":65535}},\"properties\":{\"type\":{\"type\":\"string\",\"enum\":[\"PRIVILEGE_GRANTED_EVENT\"]},\"payload\":{\"type\":\"object\",\"properties\":{\"id\":{\"$ref\":\"#/definitions/uuid\",\"description\":\"\"},\"requesterId\":{\"$ref\":\"#/definitions/uuid\",\"description\":\"\"},\"memberId\":{\"$ref\":\"#/definitions/uuid\",\"description\":\"\"},\"pollingRight\":{\"$ref\":\"#/definitions/bool\",\"description\":\"\"},\"votingRight\":{\"$ref\":\"#/definitions/bool\",\"description\":\"\"},\"initiativeRight\":{\"$ref\":\"#/definitions/bool\",\"description\":\"\"},\"managementRight\":{\"$ref\":\"#/definitions/bool\",\"description\":\"\"},\"weight\":{\"$ref\":\"#/definitions/uint\",\"description\":\"\"}},\"required\":[\"id\",\"requesterId\",\"memberId\",\"pollingRight\",\"votingRight\",\"initiativeRight\",\"managementRight\",\"weight\"]}},\"required\":[\"payload\",\"type\"],\"commandConstructor\":\"GRANT_PRIVILEGE_COMMAND\"}");



}

  