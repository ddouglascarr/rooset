const { pickBy, each, upperFirst } = require('lodash');
const {
  updateDir,
  getConfigFromEnv,
  generateSchemas,
} = require('../../../../ratk-declarations-utils');
const {
  generateFilenameFromMsgType,
  getMsgTypeFromSchema,
} = require('./utils');

const generateEnum = require('./generators/generateEnum');
const generateEnumParser = require('./generators/generateEnumParser');
const generateEnumUtilsHeader = require('./generators/generateEnumUtilsHeader');
const generateCMakeList = require('./generators/generateCMakeList');

const config = getConfigFromEnv({
  destPath: 'RATK_GEN_ENUM_CPP_DEST_DIR',
  baseSchemaLocation: 'RATK_GEN_BASE_SCHEMA_FILE',
});
const baseSchema = require(config.baseSchemaLocation);
const files = {};



// enums
const enumDefinitions = pickBy(baseSchema.definitions, (d) => d.type === 'string' && d.enum);
const enumClassnames = [];
each(enumDefinitions, (enumDefinition, key) => {
  const enumClassname = upperFirst(key);
  enumClassnames.push(enumClassname);
  files[`${enumClassname}.h`] =  generateEnum(
      enumClassname, enumDefinition.enum);
  files[`EnumUtils${enumClassname}Parsers.cpp`] =
      generateEnumParser(enumClassname, enumDefinition.enum);
});
files['EnumUtils.h'] = generateEnumUtilsHeader(enumClassnames);
files['CMakeLists.txt'] = generateCMakeList(files, 'RATK_GEN_ENUM_SOURCES');

updateDir(config.destPath, files);
