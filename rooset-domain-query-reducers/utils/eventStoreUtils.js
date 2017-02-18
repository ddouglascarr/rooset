const spawn = require("child_process").spawn;
const fetch = require("node-fetch");
const uuid = require("uuid4");
const fs = require("fs");
const btoa = require("btoa");
const {
  getConfigFromEnv,
  findSchema,
  assertMessageCompliesWithSchema
} = require("../../rooset-application-toolkit/ratk-declarations-utils");

const EVENTSTORE_HOST = "localhost";
const EVENTSTORE_PORT = "2113";
const HEADERS = {
  "Content-Type": "application/json",
  "Authorization": "Basic " + btoa("admin:changeit"),
};


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
      if (chunk.includes("Sub System 'Projections' initialized")) return resolve(child);
    });
  });
}

function initProjection(queryType, fileContent) {
  console.log('initProjection run');
    const url = `http://${EVENTSTORE_HOST}:${EVENTSTORE_PORT}/projections/continuous` +
      `?name=${queryType}` +
      "&type=js" +
      "&emit=true" +
      "&trackemittedstreams=true";
    console.log(url);
    return fetch(url, { method: "POST", body: fileContent, headers: HEADERS }
  ).then((resp) => {
    if (!resp.ok) throw new Error(resp.statusText);
    return Promise.resolve();
  });
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
    { body: JSON.stringify(event), headers: localHeaders, method: "POST" }
  ).then(resp => {
    if (!resp.ok)
      throw new Error(`saving event ${type} failed ${resp.statusText}`);
    return Promise.resolve(resp);
  }).then((resp) => {
    console.log(`persistEvent response ${resp}`);
    return Promise.resolve();
  });
}
