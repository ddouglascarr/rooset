module.exports = function(enumClassname, enums) {

  const generateParsers = () => enums.map((s) =>
      `if (e == "${s}") return ${enumClassname}::${s};`);
  const generateSerializers = () => enums.map((s) =>
      `if (e == ${enumClassname}::${s}) return "${s}";`)

  return `
#include "enums/EnumUtils.h"
#include "enums/${enumClassname}.h"
#include <stdexcept>

namespace rooset {
  ${enumClassname} EnumUtils::parse${enumClassname}(const string& e)
  {
    ${generateParsers().join('\n')}
    throw std::invalid_argument("unknown enum");
  }

  string EnumUtils::serialize${enumClassname}(${enumClassname} e)
  {
    ${generateSerializers().join('\n')}
    throw std::invalid_argument("unknown enum");
  }

}  `;
}
