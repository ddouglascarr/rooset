const spawn = require("child_process").spawn;
const fetch = require("node-fetch");
const uuid = require("uuid4");
const fs = require("fs");
const {
  getConfigFromEnv,
  findSchema,
  assertMessageCompliesWithSchema
} = require("../../rooset-application-toolkit/ratk-declarations-utils");

const EVENTSTORE_HOST = "localhost";
const EVENTSTORE_PORT = "2113";
const HEADERS = { "Content-Type": "application/json" };

module.exports = {
  startEventStore,
  initProjection,
  persistEvent
};

function startEventStore() {
  return new Promise((resolve, reject) => {
    const child = spawn("eventstored", ["--mem-db", "--run-projections=all"]);
    child.on("error", err => reject(err));
    child.stdout.on("data", chunk => {
      if (chunk.includes("IS MASTER")) return resolve(child);
    });
  });
}

function initProjection(queryType, fileContent) {
  return fetch(
    `http://admin:changeit@${EVENTSTORE_HOST}:${EVENTSTORE_PORT}/projections/continuous` +
      "?name=${queryType}" +
      "&type=js" +
      "&emit=true" +
      "&trackemittedstreams=true",
    { method: "POST", body: fileContent, headers: HEADERS }
  );
}

function persistEvent(event) {
  const type = event.type;
  const streamId = event.payload.id;

  const localHeaders = Object.assign({}, HEADERS, {
    "ES-EventType": type,
    "ES-EventId": uuid()
  });

  return fetch(
    `http://${EVENTSTORE_HOST}:${EVENTSTORE_PORT}/streams/${streamId}`,
    { body: JSON.stringify(event), headers: localHeaders, method: POST }
  ).then(resp => {
    if (!resp.ok)
      throw new Error(`saving event ${type} failed ${resp.statusText}`);
  });
}
