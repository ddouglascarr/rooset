module.exports = function(enumName, enums) {

  return `
#pragma once
namespace rooset {
  enum class ${enumName}
  {
    ${enums.join(',\n')}
  };
}

  `;
}
