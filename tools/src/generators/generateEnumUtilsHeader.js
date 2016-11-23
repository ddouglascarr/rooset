module.exports = function(enumClassnames) {

  const generateInclude = (enumClassname) =>
      `#include "enums/${enumClassname}.h"`;
  const generateParser = (enumClassname) =>
    `static ${enumClassname} parse${enumClassname}(const string& code);`;
  const generateSerializer = (enumClassname) =>
    `static string serialize${enumClassname}(${enumClassname} e);`;

  return `
#pragma once
#include <string>
${enumClassnames.map(generateInclude).join('\n')}

using namespace std;

namespace rooset {
  class EnumUtils
  {
  public:
    ${enumClassnames.map(generateParser).join('\n')}
    ${enumClassnames.map(generateSerializer).join('\n')}
  };
}

  `;
}
