module.exports = function(javaBasePackage, enumName, enums) {

  return `
package ${javaBasePackage}.enums;

public enum ${enumName}
{
  ${enums.join(',\n')}
}
  `;

}
