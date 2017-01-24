
#include "exceptions/SystemException.h"

using namespace std;
using namespace rapidjson;

namespace rooset {



const SchemaDocument SystemException::schema = JsonUtils::parseSchema(
    "{\"$schema\":\"http://json-schema.org/draft-04/schema#\",\"id\":\"https://rooset.org/schemas/exceptions/SYSTEM_EXCEPTION.schema.json\",\"type\":\"object\",\"definitions\":{\"exceptionCode\":{\"type\":\"string\",\"enum\":[\"CONFLICT_EXCEPTION\",\"GENERAL_PROJECT_EXCEPTION\",\"ITEM_NOT_FOUND_EXCEPTION\",\"UNAUTHORIZED_EXCEPTION\",\"UNPRIVILEGED_EXCEPTION\",\"INVALID_INPUT_EXCEPTION\",\"ISSUE_STATE_EXCEPTION\"]},\"string\":{\"type\":\"string\"}},\"properties\":{\"type\":{\"type\":\"string\",\"enum\":[\"SYSTEM_EXCEPTION\"]},\"error\":{\"type\":\"boolean\",\"enum\":[true]},\"payload\":{\"type\":\"object\",\"properties\":{\"code\":{\"$ref\":\"#/definitions/exceptionCode\"},\"message\":{\"$ref\":\"#/definitions/string\"}},\"required\":[\"code\",\"message\"]}},\"required\":[\"payload\",\"error\",\"type\"]}");



}

  