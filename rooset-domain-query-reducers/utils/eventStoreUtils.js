const spawn = require('child_process').spawn;
const exec = require('child_process').exec;


module.exports = {

  startEventStore: () => {
    return new Promise((resolve, reject) => {
      const child = spawn('eventstored', ['--mem-db']);
      child.on('error', (err) => reject(err));
      child.stdout.on('data', (chunk) => {
        console.log(`chunk: ${chunk}`);
      });
      resolve(child);
    });
  },

};
