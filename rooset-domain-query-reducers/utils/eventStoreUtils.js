const spawn = require("child_process").spawn;
const exec = require("child_process").exec;

module.exports = {
  startEventStore
};

function startEventStore() {
  return new Promise((resolve, reject) => {
    const child = spawn("eventstored", ["--mem-db"]);
    child.on("error", err => reject(err));
    child.stdout.on("data", chunk => {
      if (chunk.includes("IS MASTER")) return resolve(child);
    });
  });
}
