const { camelCase, upperFirst, map } = require("lodash");
const typenames = require("./templates/typenameMap");

module.exports = {
  getClassNameFromType: type => upperFirst(camelCase(type)),
  buildPayloadDeclaration: decl =>
    map(decl, (v, k) => `  ${k}: ${typenames[v.type]}`),
};
