module.exports = function(enumName, enums) {

  return `
package org.rooset.httpapi.generated.enums;

public enum ${enumName}
{
  ${enums.join(',\n')}
}
  `;

}
