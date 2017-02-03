
#include "events/IssueVotingPhaseCompletedEvent.h"

using namespace std;
using namespace rapidjson;

namespace rooset {



const SchemaDocument IssueVotingPhaseCompletedEvent::schema = JsonUtils::parseSchema(
    "{\"$schema\":\"http://json-schema.org/draft-04/schema#\",\"id\":\"https://rooset.org/schemas/events/ISSUE_VOTING_PHASE_COMPLETED_EVENT.schema.json\",\"type\":\"object\",\"definitions\":{\"uuid\":{\"type\":\"string\",\"pattern\":\"[0-9a-f]{8}-[0-9a-f]{4}-[1-5][0-9a-f]{3}-[89ab][0-9a-f]{3}-[0-9a-f]{12}\"},\"uuidNullable\":{\"oneOf\":[{\"$ref\":\"#/definitions/uuid\"},{\"type\":\"null\"}]}},\"properties\":{\"type\":{\"type\":\"string\",\"enum\":[\"ISSUE_VOTING_PHASE_COMPLETED_EVENT\"]},\"payload\":{\"type\":\"object\",\"properties\":{\"id\":{\"$ref\":\"#/definitions/uuid\",\"description\":\"\"},\"winnerId\":{\"$ref\":\"#/definitions/uuidNullable\",\"description\":\"\"}},\"required\":[\"id\",\"winnerId\"]}},\"required\":[\"payload\",\"type\"]}");



}

  