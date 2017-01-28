
#include "commands/UnblockDelegationForIssueCommand.h"

using namespace std;
using namespace rapidjson;

namespace rooset {



const SchemaDocument UnblockDelegationForIssueCommand::schema = JsonUtils::parseSchema(
    "{\"$schema\":\"http://json-schema.org/draft-04/schema#\",\"id\":\"https://rooset.org/schemas/commands/UNBLOCK_DELEGATION_FOR_ISSUE_COMMAND.schema.json\",\"type\":\"object\",\"definitions\":{\"uuid\":{\"type\":\"string\",\"pattern\":\"[0-9a-f]{8}-[0-9a-f]{4}-[1-5][0-9a-f]{3}-[89ab][0-9a-f]{3}-[0-9a-f]{12}\"}},\"properties\":{\"type\":{\"type\":\"string\",\"enum\":[\"UNBLOCK_DELEGATION_FOR_ISSUE_COMMAND\"]},\"payload\":{\"type\":\"object\",\"properties\":{\"id\":{\"$ref\":\"#/definitions/uuid\"},\"requesterId\":{\"$ref\":\"#/definitions/uuid\"}},\"required\":[\"id\",\"requesterId\"]}},\"required\":[\"payload\",\"type\"]}");



}

  