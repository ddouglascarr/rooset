const fs = require('fs');
const { upperFirst, camelCase, each, pull } = require('lodash');
const schemaTools = require('../../schema');
const generateEventClass = require('./generators/generateEventClass');
const generateCommandClass = require('./generators/generateCommandClass');
const generateTestFile = require('./generators/generateTestFile');
const generateEnum = require('./generators/generateEnum');
const generateEnumParser = require('./generators/generateEnumParser');
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

// exceptionCode enum
const exceptionCodes = require('../../schema/src/base.schema.json')
    .definitions.exceptionCode.enum;
stageFile('enums', `ExceptionCode.h`, generateEnum(
    'ExceptionCode', exceptionCodes));
stageFile('sources', `JsonUtilsExceptionCodeParser.cpp`,
    generateEnumParser('ExceptionCode', exceptionCodes));

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
each(filesToWrite, (fileNameMap, path) => updateDir(path, fileNameMap));

function stageFile(_path, filename, content) {
  const path = `${GEN_DIR}/${_path}`;
  if (!filesToWrite[path]) filesToWrite[path] = {};
  if (filesToWrite[path][filename]) {
    throw new Error(`attempting to write ${filename} in ${path} more than once`);
  }
  filesToWrite[path][filename] = content;
}

function updateDir(path, fileNameMap) {
  console.log(`-- updating ${path}`);
  let filesToDelete = fs.readdirSync(path);
  each(fileNameMap, (content, fileName) => {
    pull(filesToDelete, fileName);
    const filePath = `${path}/${fileName}`;
    let isContentDifferent = true;
    if (fs.existsSync(filePath)) {
      const oldContent = fs.readFileSync(filePath, 'utf8');
      isContentDifferent = (oldContent !== content);
    }
    if (isContentDifferent) {
      fs.writeFileSync(filePath, content, 'utf8');
      console.log(`  -- ${fileName} updated`);
    }
  });
  filesToDelete.forEach((fileName) => fs.unlinkSync(`${path}/${fileName}`));
  if (filesToDelete.length > 0) console.log(`${filesToDelete} deleted`);
}
