
#include "commands/SetUnitPolicyCommand.h"

using namespace std;
using namespace rapidjson;

namespace rooset {



const SchemaDocument SetUnitPolicyCommand::schema = JsonUtils::parseSchema(
    "{\"$schema\":\"http://json-schema.org/draft-04/schema#\",\"id\":\"https://rooset.org/schemas/commands/SET_UNIT_POLICY_COMMAND.schema.json\",\"type\":\"object\",\"definitions\":{\"uuid\":{\"type\":\"string\",\"pattern\":\"[0-9a-f]{8}-[0-9a-f]{4}-[1-5][0-9a-f]{3}-[89ab][0-9a-f]{3}-[0-9a-f]{12}\"},\"string\":{\"type\":\"string\"},\"bool\":{\"type\":\"boolean\"},\"duration\":{\"type\":\"integer\",\"description\":\"Length of duration in seconds\"},\"uint\":{\"type\":\"integer\",\"minimum\":0,\"maximum\":65535},\"defeatStrength\":{\"type\":\"string\",\"enum\":[\"TUPLE\",\"SIMPLE\"]}},\"properties\":{\"type\":{\"type\":\"string\",\"enum\":[\"SET_UNIT_POLICY_COMMAND\"]},\"payload\":{\"type\":\"object\",\"properties\":{\"id\":{\"$ref\":\"#/definitions/uuid\",\"description\":\"\"},\"requesterId\":{\"$ref\":\"#/definitions/uuid\",\"description\":\"\"},\"policyId\":{\"$ref\":\"#/definitions/uuid\",\"description\":\"\"},\"name\":{\"$ref\":\"#/definitions/string\",\"description\":\"\"},\"description\":{\"$ref\":\"#/definitions/string\",\"description\":\"\"},\"polling\":{\"$ref\":\"#/definitions/bool\",\"description\":\"\"},\"maxAdmissionTime\":{\"$ref\":\"#/definitions/duration\",\"description\":\"\"},\"minAdmissionTime\":{\"$ref\":\"#/definitions/duration\",\"description\":\"\"},\"discussionTime\":{\"$ref\":\"#/definitions/duration\",\"description\":\"\"},\"verificationTime\":{\"$ref\":\"#/definitions/duration\",\"description\":\"\"},\"votingTime\":{\"$ref\":\"#/definitions/duration\",\"description\":\"\"},\"issueQuorumNum\":{\"$ref\":\"#/definitions/uint\",\"description\":\"\"},\"issueQuorumDen\":{\"$ref\":\"#/definitions/uint\",\"description\":\"\"},\"defeatStrength\":{\"$ref\":\"#/definitions/defeatStrength\",\"description\":\"\"},\"directMajorityNum\":{\"$ref\":\"#/definitions/uint\",\"description\":\"\"},\"directMajorityDen\":{\"$ref\":\"#/definitions/uint\",\"description\":\"\"},\"directMajorityStrict\":{\"$ref\":\"#/definitions/bool\",\"description\":\"\"},\"directMajorityPositive\":{\"$ref\":\"#/definitions/uint\",\"description\":\"\"},\"directMajorityNonNegative\":{\"$ref\":\"#/definitions/uint\",\"description\":\"\"},\"noReverseBeatPath\":{\"$ref\":\"#/definitions/bool\",\"description\":\"\"},\"noMultistageMajority\":{\"$ref\":\"#/definitions/bool\",\"description\":\"\"}},\"required\":[\"id\",\"requesterId\",\"policyId\",\"name\",\"description\",\"polling\",\"maxAdmissionTime\",\"minAdmissionTime\",\"discussionTime\",\"verificationTime\",\"votingTime\",\"issueQuorumNum\",\"issueQuorumDen\",\"defeatStrength\",\"directMajorityNum\",\"directMajorityDen\",\"directMajorityStrict\",\"directMajorityPositive\",\"directMajorityNonNegative\",\"noReverseBeatPath\",\"noMultistageMajority\"]}},\"required\":[\"payload\",\"type\"]}");



}

  