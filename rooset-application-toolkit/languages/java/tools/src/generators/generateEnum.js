module.exports = function(enumName, enums) {

  return `
package org.rooset.httpapi.enums;

public enum ${enumName}
{
  ${enums.join(',\n')}
}
  `;

}
