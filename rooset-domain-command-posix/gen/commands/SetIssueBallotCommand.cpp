
#include "commands/SetIssueBallotCommand.h"

using namespace std;
using namespace rapidjson;

namespace rooset {



const SchemaDocument SetIssueBallotCommand::schema = JsonUtils::parseSchema(
    "{\"$schema\":\"http://json-schema.org/draft-04/schema#\",\"id\":\"https://rooset.org/schemas/commands/SET_ISSUE_BALLOT_COMMAND.schema.json\",\"type\":\"object\",\"definitions\":{\"uuid\":{\"type\":\"string\",\"pattern\":\"[0-9a-f]{8}-[0-9a-f]{4}-[1-5][0-9a-f]{3}-[89ab][0-9a-f]{3}-[0-9a-f]{12}\"},\"schulzeBallot\":{\"type\":\"object\",\"required\":[\"approve\",\"abstain\",\"disapprove\"],\"properties\":{\"approve\":{\"type\":\"array\",\"items\":{\"$ref\":\"#definitions/uuidArray\"}},\"abstain\":{\"$ref\":\"#definitions/uuidArray\"},\"disapprove\":{\"type\":\"array\",\"items\":{\"$ref\":\"#definitions/uuidArray\"}}}}},\"properties\":{\"type\":{\"type\":\"string\",\"enum\":[\"SET_ISSUE_BALLOT_COMMAND\"]},\"payload\":{\"type\":\"object\",\"properties\":{\"id\":{\"$ref\":\"#/definitions/uuid\",\"description\":\"\"},\"requesterId\":{\"$ref\":\"#/definitions/uuid\",\"description\":\"\"},\"ballot\":{\"$ref\":\"#/definitions/schulzeBallot\",\"description\":\"\"}},\"required\":[\"id\",\"requesterId\",\"ballot\"]}},\"required\":[\"payload\",\"type\"]}");



}

  