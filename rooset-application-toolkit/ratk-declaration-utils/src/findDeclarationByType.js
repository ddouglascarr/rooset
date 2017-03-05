const { find } = require('lodash');
const getDeclarations = require('./getDeclarations');


module.exports = (srcPath, type) => {
  const decls = getDeclarations(srcPath);
  const decl = find(decls, (d) => d.type === type);
  if (!decl) throw new Error(
      `cannot find ${type} in ${srcPath}`);
  return decl;
};
