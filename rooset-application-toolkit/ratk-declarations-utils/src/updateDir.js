const fs = require('fs');
const { each, pull } = require('lodash');

module.exports = function(path, fileNameMap) {
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
      console.log(`.... ${fileName} updated`);
    }
  });
  filesToDelete.forEach((fileName) => fs.unlinkSync(`${path}/${fileName}`));
  if (filesToDelete.length > 0) console.log(`.... ${filesToDelete} deleted`);
}
