const fs = require('fs');
const generateSchemas = require('./generateSchemas');

const BUILD_DIR = ('build');
const MESSAGE_CATEGORIES = ['commands', 'events'];


const writeCategory = (messageCategory, schemas) => {
  schemas.forEach((schema) => {
    const type = schema.properties.type.enum[0];
    fs.writeFileSync(`./build/${messageCategory}/${type}.schema.json`, JSON.stringify(schema));
  });
};

const cleanBuild = () => {
  deleteFolderRecursive(BUILD_DIR);
  fs.mkdirSync('build');
  MESSAGE_CATEGORIES.forEach(
      (messageCategory) => fs.mkdirSync(`build/${messageCategory}`));
};

const build = () => {
  cleanBuild();
  MESSAGE_CATEGORIES.forEach((messageCategory) => {
    const schemas = generateSchemas(messageCategory);
    writeCategory(messageCategory, schemas);
  });
};

module.exports = build;

function deleteFolderRecursive(path) {
  if( fs.existsSync(path) ) {
    fs.readdirSync(path).forEach(function(file,index){
      var curPath = path + "/" + file;
      if(fs.lstatSync(curPath).isDirectory()) { // recurse
        deleteFolderRecursive(curPath);
      } else { // delete file
        fs.unlinkSync(curPath);
      }
    });
    fs.rmdirSync(path);
  }
};
