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
const config = getConfigFromEnv({
  baseSchema: RATK_GEN_BASE_SCHEMA_FILE,
  testSrc: RATK_GEN_TEST_DECL_DIR,
  eventDeclSrc: RATK_GET_EVENT_DECL_DIR
});

const baseSchema = require(config.baseSchema);

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

function initProjection(queryType) {
  const file = fs.readFileSync(`../reducers/${queryType}.js`, "utf8");
  if (!file) throw new Error(`could not read ${queryType} reducer`);
  return fetch(
    `http://admin:changeit@${EVENTSTORE_HOST}:${EVENTSTORE_PORT}/projections/continuous` +
      "?name=${queryType}" +
      "&type=js" +
      "&emit=true" +
      "&trackemittedstreams=true",
    { method: POST, body: file, headers: HEADERS }
  );
}

function persistEvent(event) {
  const type = event.type;
  if (!type) throw new Error(`No type for event: ${event}`);
  const schema = findSchema(baseSchema, "events", eventDeclSrc, type);
  assertMessageCompliesWithSchema(schema, event);
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
