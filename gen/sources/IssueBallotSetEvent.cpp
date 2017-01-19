
#include "events/IssueBallotSetEvent.h"

using namespace std;
using namespace rapidjson;

namespace rooset {

  const unique_ptr<SchemaDocument> IssueBallotSetEvent::schema = JsonUtils::parseSchema(
      "{\"$schema\":\"http://json-schema.org/draft-04/schema#\",\"id\":\"https://rooset.org/schemas/events/ISSUE_BALLOT_SET_EVENT.schema.json\",\"type\":\"object\",\"definitions\":{\"uuid\":{\"type\":\"string\",\"pattern\":\"[0-9a-f]{8}-[0-9a-f]{4}-[1-5][0-9a-f]{3}-[89ab][0-9a-f]{3}-[0-9a-f]{12}\"},\"schulzeBallot\":{\"type\":\"object\",\"required\":[\"approve\",\"abstain\",\"disapprove\"],\"properties\":{\"approve\":{\"type\":\"array\",\"items\":{\"$ref\":\"#definitions/uuidArray\"}},\"abstain\":{\"$ref\":\"#definitions/uuidArray\"},\"disapprove\":{\"type\":\"array\",\"items\":{\"$ref\":\"#definitions/uuidArray\"}}}}},\"properties\":{\"type\":{\"type\":\"string\",\"enum\":[\"ISSUE_BALLOT_SET_EVENT\"]},\"payload\":{\"type\":\"object\",\"properties\":{\"id\":{\"$ref\":\"#/definitions/uuid\"},\"requesterId\":{\"$ref\":\"#/definitions/uuid\"},\"ballot\":{\"$ref\":\"#/definitions/schulzeBallot\"}},\"required\":[\"id\",\"requesterId\",\"ballot\"]}},\"required\":[\"payload\",\"type\"],\"commandConstructor\":\"SET_ISSUE_BALLOT_COMMAND\"}");

}

  