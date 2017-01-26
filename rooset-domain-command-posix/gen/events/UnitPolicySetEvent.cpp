
#include "events/UnitPolicySetEvent.h"

using namespace std;
using namespace rapidjson;

namespace rooset {



const SchemaDocument UnitPolicySetEvent::schema = JsonUtils::parseSchema(
    "{\"$schema\":\"http://json-schema.org/draft-04/schema#\",\"id\":\"https://rooset.org/schemas/events/UNIT_POLICY_SET_EVENT.schema.json\",\"type\":\"object\",\"definitions\":{\"uuid\":{\"type\":\"string\",\"pattern\":\"[0-9a-f]{8}-[0-9a-f]{4}-[1-5][0-9a-f]{3}-[89ab][0-9a-f]{3}-[0-9a-f]{12}\"},\"string\":{\"type\":\"string\"},\"bool\":{\"type\":\"boolean\"},\"duration\":{\"type\":\"integer\",\"description\":\"Length of duration in seconds\"},\"uint\":{\"type\":\"integer\",\"minimum\":0,\"maximum\":65535},\"defeatStrength\":{\"type\":\"string\",\"enum\":[\"TUPLE\",\"SIMPLE\"]}},\"properties\":{\"type\":{\"type\":\"string\",\"enum\":[\"UNIT_POLICY_SET_EVENT\"]},\"payload\":{\"type\":\"object\",\"properties\":{\"id\":{\"$ref\":\"#/definitions/uuid\"},\"requesterId\":{\"$ref\":\"#/definitions/uuid\"},\"policyId\":{\"$ref\":\"#/definitions/uuid\"},\"name\":{\"$ref\":\"#/definitions/string\"},\"description\":{\"$ref\":\"#/definitions/string\"},\"polling\":{\"$ref\":\"#/definitions/bool\"},\"maxAdmissionTime\":{\"$ref\":\"#/definitions/duration\"},\"minAdmissionTime\":{\"$ref\":\"#/definitions/duration\"},\"discussionTime\":{\"$ref\":\"#/definitions/duration\"},\"verificationTime\":{\"$ref\":\"#/definitions/duration\"},\"votingTime\":{\"$ref\":\"#/definitions/duration\"},\"issueQuorumNum\":{\"$ref\":\"#/definitions/uint\"},\"issueQuorumDen\":{\"$ref\":\"#/definitions/uint\"},\"defeatStrength\":{\"$ref\":\"#/definitions/defeatStrength\"},\"directMajorityNum\":{\"$ref\":\"#/definitions/uint\"},\"directMajorityDen\":{\"$ref\":\"#/definitions/uint\"},\"directMajorityStrict\":{\"$ref\":\"#/definitions/bool\"},\"directMajorityPositive\":{\"$ref\":\"#/definitions/uint\"},\"directMajorityNonNegative\":{\"$ref\":\"#/definitions/uint\"},\"noReverseBeatPath\":{\"$ref\":\"#/definitions/bool\"},\"noMultistageMajority\":{\"$ref\":\"#/definitions/bool\"}},\"required\":[\"id\",\"requesterId\",\"policyId\",\"name\",\"description\",\"polling\",\"maxAdmissionTime\",\"minAdmissionTime\",\"discussionTime\",\"verificationTime\",\"votingTime\",\"issueQuorumNum\",\"issueQuorumDen\",\"defeatStrength\",\"directMajorityNum\",\"directMajorityDen\",\"directMajorityStrict\",\"directMajorityPositive\",\"directMajorityNonNegative\",\"noReverseBeatPath\",\"noMultistageMajority\"]}},\"required\":[\"payload\",\"type\"],\"commandConstructor\":\"SET_UNIT_POLICY_COMMAND\"}");



}

  