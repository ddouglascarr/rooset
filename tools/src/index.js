const fs = require('fs');
const { upperFirst, camelCase, each, pull, pickBy } = require('lodash');
const schemaTools = require('../../schema');
const generateEventClass = require('./generators/generateEventClass');
const generateCommandClass = require('./generators/generateCommandClass');
const generateTestFile = require('./generators/generateTestFile');
const generateEnum = require('./generators/generateEnum');
const generateEnumParser = require('./generators/generateEnumParser');
const generateEnumUtilsHeader = require('./generators/generateEnumUtilsHeader');
const generateExceptionClass = require('./generators/generateExceptionClass');
const generateMessageCpp = require('./generators/generateMessageCpp');
const {
  generateFilenameFromMsgType,
  getMsgTypeFromSchema,
} = require('./utils');
const GEN_DIR = `${process.cwd()}/${process.env.ROOSET_GEN_DIR}`;
const filesToWrite = {};

// events
const eventSchemas = schemaTools.generateSchemas('events');
const eventClasses = schemaTools.generateSchemas('events')
    .map((schema) => generateEventClass(schema));
eventSchemas.forEach((eventSchema) => {
  const content = generateEventClass(eventSchema);
  const msgType = getMsgTypeFromSchema(eventSchema);
  const fileName = generateFilenameFromMsgType(msgType);
  stageFile('events', `${fileName}.h`, content);
  const cppContent = generateMessageCpp(eventSchema, 'events');
  stageFile('sources',`${fileName}.cpp`, cppContent);
});

// commands
const commandSchemas = schemaTools.generateSchemas('commands');
commandSchemas.forEach((commandSchema) => {
  const content = generateCommandClass(commandSchema);
  const msgType = getMsgTypeFromSchema(commandSchema);
  const fileName = generateFilenameFromMsgType(msgType);
  stageFile('commands', `${fileName}.h`, content);
  const cppContent = generateMessageCpp(commandSchema, 'commands');
  stageFile('sources', `${fileName}.cpp`, cppContent);
});

// enums
const definitions = require('../../schema/src/base.schema.json').definitions;
const enumDefinitions = pickBy(definitions, (d) => d.type === 'string' && d.enum);
console.log(enumDefinitions);
const enumClassnames = [];
each(enumDefinitions, (enumDefinition, key) => {
  const enumClassname = upperFirst(key);
  enumClassnames.push(enumClassname);
  stageFile('enums', `${enumClassname}.h`, generateEnum(
      enumClassname, enumDefinition.enum));
  stageFile('sources', `EnumUtils${enumClassname}Parsers.cpp`,
      generateEnumParser(enumClassname, enumDefinition.enum));
});
stageFile('enums', 'EnumUtils.h', generateEnumUtilsHeader(enumClassnames));

// exceptions
const exceptionSchemas = schemaTools.generateSchemas('exceptions');
exceptionSchemas.forEach((exceptionSchema) => {
  const exceptionType = getMsgTypeFromSchema(exceptionSchema);
  const content = generateExceptionClass(exceptionType);
  const fileName = generateFilenameFromMsgType(exceptionType);
  stageFile('exceptions', `${fileName}.h`, content);
  const cppContent = generateMessageCpp(exceptionSchema, 'exceptions');
  stageFile('sources', `${fileName}.cpp`, cppContent);
});

// tests
const testDocs = schemaTools.getTests();
testDocs.forEach((testDoc) => {
  const content = generateTestFile(testDoc);
  const fileName = `${camelCase(testDoc.label)}.cpp`;
  stageFile('tests', fileName, content);
});

// perform update
each(filesToWrite, (fileNameMap, path) => schemaTools.updateDir(path, fileNameMap));

function stageFile(_path, filename, content) {
  const path = `${GEN_DIR}/${_path}`;
  if (!filesToWrite[path]) filesToWrite[path] = {};
  if (filesToWrite[path][filename]) {
    throw new Error(`attempting to write ${filename} in ${path} more than once`);
  }
  filesToWrite[path][filename] = content;
}
