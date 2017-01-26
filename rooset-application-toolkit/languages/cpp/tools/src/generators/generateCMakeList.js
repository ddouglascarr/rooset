const { chain } = require('lodash');


module.exports = function(files, cmakeVariable) {

  const generateFilepath = (files) => chain(Object.keys(files))
      .filter((f) => f.slice(f.length - 4, f.length) === '.cpp')
      .map((filename) => '${CMAKE_CURRENT_SOURCE_DIR}' + `/${filename}`)
      .value()
      .join('\n');

  return `

set(${cmakeVariable}
  ${generateFilepath(files)}

  CACHE FILEPATH "Generated source files")

`;

};
