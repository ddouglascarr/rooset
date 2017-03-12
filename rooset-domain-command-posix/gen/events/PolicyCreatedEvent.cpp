
#include "events/PolicyCreatedEvent.h"

using namespace std;
using namespace rapidjson;

namespace rooset {



const SchemaDocument PolicyCreatedEvent::schema = JsonUtils::parseSchema(
    "{\"$schema\":\"http://json-schema.org/draft-04/schema#\",\"id\":\"https://rooset.org/schemas/events/POLICY_CREATED_EVENT.schema.json\",\"type\":\"object\",\"definitions\":{\"uuid\":{\"type\":\"string\",\"pattern\":\"[0-9a-f]{8}-[0-9a-f]{4}-[1-5][0-9a-f]{3}-[89ab][0-9a-f]{3}-[0-9a-f]{12}\"},\"string\":{\"type\":\"string\"},\"votingAlgorithm\":{\"type\":\"string\",\"enum\":[\"SCHULZE\"]},\"duration\":{\"type\":\"integer\",\"description\":\"Length of duration in seconds\"},\"uint\":{\"type\":\"integer\",\"minimum\":0,\"maximum\":65535}},\"properties\":{\"type\":{\"type\":\"string\",\"enum\":[\"POLICY_CREATED_EVENT\"]},\"payload\":{\"type\":\"object\",\"properties\":{\"id\":{\"$ref\":\"#/definitions/uuid\",\"description\":\"Unit Id\"},\"requesterId\":{\"$ref\":\"#/definitions/uuid\",\"description\":\"\"},\"policyId\":{\"$ref\":\"#/definitions/uuid\",\"description\":\"\"},\"name\":{\"$ref\":\"#/definitions/string\",\"description\":\"\"},\"description\":{\"$ref\":\"#/definitions/string\",\"description\":\"\"},\"votingAlgorithm\":{\"$ref\":\"#/definitions/votingAlgorithm\",\"description\":\"The althorithm used in the VOTING phase\"},\"maxAdmissionTime\":{\"$ref\":\"#/definitions/duration\",\"description\":\"The maximum time a new issue stays in ADMISSION before it is resolved\"},\"minAdmissionTime\":{\"$ref\":\"#/definitions/duration\",\"description\":\"The minimum time before an issue is passed the ADMISSION phase\"},\"discussionTime\":{\"$ref\":\"#/definitions/duration\",\"description\":\"duration of the DISCUSSION phase\"},\"verificationTime\":{\"$ref\":\"#/definitions/duration\",\"description\":\"duration of the VERIFICATION phase\"},\"votingTime\":{\"$ref\":\"#/definitions/duration\",\"description\":\"duration of the VOTING phase\"},\"issueQuorumNum\":{\"$ref\":\"#/definitions/uint\",\"description\":\"The numerator used to determine whether an issue quorum has been reached in the ADMISSION phase\"},\"issueQuorumDen\":{\"$ref\":\"#/definitions/uint\",\"description\":\"The denominator used to determine whether an issue quorum has been reached in the ADMISSION phase\"},\"initiativeQuorumNum\":{\"$ref\":\"#/definitions/uint\",\"description\":\"The numerator used to determine whether an issue quorum has been reached in the VERIFICATION phase\"},\"initiativeQuorumDen\":{\"$ref\":\"#/definitions/uint\",\"description\":\"The denominator used to determine whether an initiative quorum has been reached in the VERIFICATION phase\"}},\"required\":[\"id\",\"requesterId\",\"policyId\",\"name\",\"description\",\"votingAlgorithm\",\"maxAdmissionTime\",\"minAdmissionTime\",\"discussionTime\",\"verificationTime\",\"votingTime\",\"issueQuorumNum\",\"issueQuorumDen\",\"initiativeQuorumNum\",\"initiativeQuorumDen\"]}},\"required\":[\"payload\",\"type\"],\"commandConstructor\":\"CREATE_POLICY_COMMAND\"}");



}

  