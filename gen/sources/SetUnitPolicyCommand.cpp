
#include "commands/SetUnitPolicyCommand.h"

using namespace std;
using namespace rapidjson;

namespace rooset {

  const unique_ptr<SchemaDocument> SetUnitPolicyCommand::schema = JsonUtils::parseSchema(
      "{\"$schema\":\"http://json-schema.org/draft-04/schema#\",\"id\":\"https://rooset.org/schemas/commands/SET_UNIT_POLICY_COMMAND.schema.json\",\"type\":\"object\",\"definitions\":{\"uuid\":{\"type\":\"string\",\"pattern\":\"[0-9a-f]{8}-[0-9a-f]{4}-[1-5][0-9a-f]{3}-[89ab][0-9a-f]{3}-[0-9a-f]{12}\"},\"string\":{\"type\":\"string\"},\"bool\":{\"type\":\"boolean\"},\"duration\":{\"type\":\"integer\",\"description\":\"Length of duration in seconds\"},\"int\":{\"type\":\"integer\"},\"defeatStrength\":{\"type\":\"string\",\"enum\":[\"TUPLE\",\"SIMPLE\"]}},\"properties\":{\"type\":{\"type\":\"string\",\"enum\":[\"SET_UNIT_POLICY_COMMAND\"]},\"payload\":{\"type\":\"object\",\"properties\":{\"id\":{\"$ref\":\"#/definitions/uuid\"},\"requesterId\":{\"$ref\":\"#/definitions/uuid\"},\"policyId\":{\"$ref\":\"#/definitions/uuid\"},\"name\":{\"$ref\":\"#/definitions/string\"},\"description\":{\"$ref\":\"#/definitions/string\"},\"polling\":{\"$ref\":\"#/definitions/bool\"},\"maxAdmissionTime\":{\"$ref\":\"#/definitions/duration\"},\"minAdmissionTime\":{\"$ref\":\"#/definitions/duration\"},\"discussionTime\":{\"$ref\":\"#/definitions/duration\"},\"verificationTime\":{\"$ref\":\"#/definitions/duration\"},\"votingTime\":{\"$ref\":\"#/definitions/duration\"},\"issueQuorumNum\":{\"$ref\":\"#/definitions/int\"},\"issueQuorumDen\":{\"$ref\":\"#/definitions/int\"},\"defeatStrength\":{\"$ref\":\"#/definitions/defeatStrength\"},\"directMajorityNum\":{\"$ref\":\"#/definitions/int\"},\"directMajorityDen\":{\"$ref\":\"#/definitions/int\"},\"directMajorityStrict\":{\"$ref\":\"#/definitions/bool\"},\"directMajorityPositive\":{\"$ref\":\"#/definitions/int\"},\"directMajorityNonNegative\":{\"$ref\":\"#/definitions/int\"},\"noReverseBeatPath\":{\"$ref\":\"#/definitions/bool\"},\"noMultistageMajority\":{\"$ref\":\"#/definitions/bool\"}},\"required\":[\"id\",\"requesterId\",\"policyId\",\"name\",\"description\",\"polling\",\"maxAdmissionTime\",\"minAdmissionTime\",\"discussionTime\",\"verificationTime\",\"votingTime\",\"issueQuorumNum\",\"issueQuorumDen\",\"defeatStrength\",\"directMajorityNum\",\"directMajorityDen\",\"directMajorityStrict\",\"directMajorityPositive\",\"directMajorityNonNegative\",\"noReverseBeatPath\",\"noMultistageMajority\"]}},\"required\":[\"payload\",\"type\"]}");

}

  