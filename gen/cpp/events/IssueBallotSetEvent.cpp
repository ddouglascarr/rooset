
#include "events/IssueBallotSetEvent.h"

using namespace std;
using namespace rapidjson;

namespace rooset {



const SchemaDocument IssueBallotSetEvent::schema = JsonUtils::parseSchema(
    "{\"$schema\":\"http://json-schema.org/draft-04/schema#\",\"id\":\"https://rooset.org/schemas/events/ISSUE_BALLOT_SET_EVENT.schema.json\",\"type\":\"object\",\"definitions\":{\"uuid\":{\"type\":\"string\",\"pattern\":\"[0-9a-f]{8}-[0-9a-f]{4}-[1-5][0-9a-f]{3}-[89ab][0-9a-f]{3}-[0-9a-f]{12}\"},\"schulzeBallot\":{\"type\":\"object\",\"required\":[\"approve\",\"abstain\",\"disapprove\"],\"properties\":{\"approve\":{\"type\":\"array\",\"items\":{\"type\":\"array\",\"items\":{\"$ref\":\"#/definitions/uuid\"}}},\"abstain\":{\"type\":\"array\",\"items\":{\"$ref\":\"#/definitions/uuid\"}},\"disapprove\":{\"type\":\"array\",\"items\":{\"type\":\"array\",\"items\":{\"$ref\":\"#/definitions/uuid\"}}}}}},\"properties\":{\"type\":{\"type\":\"string\",\"enum\":[\"ISSUE_BALLOT_SET_EVENT\"]},\"payload\":{\"type\":\"object\",\"properties\":{\"id\":{\"$ref\":\"#/definitions/uuid\",\"description\":\"\"},\"requesterId\":{\"$ref\":\"#/definitions/uuid\",\"description\":\"\"},\"ballot\":{\"$ref\":\"#/definitions/schulzeBallot\",\"description\":\"\"}},\"required\":[\"id\",\"requesterId\",\"ballot\"]}},\"required\":[\"payload\",\"type\"],\"commandConstructor\":\"SET_ISSUE_BALLOT_COMMAND\"}");



}

  