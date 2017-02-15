const uuid = require('uuid4');
const { getTestPrecondition } = require('../../ratk-declarations-utils');

module.exports = (testSpec, preconditionKey) => {
  const events = getTestPrecondition(testSpec, preconditionKey)
  const command = events.map((e) => {
    const streamId = e.payload.id;
    const eventType = e.type;
    return `/usr/bin/curl -i "http://localhost:2113/streams/${streamId}" -H "Content-Type:application/json" -H "ES-EventType: ${eventType}" -H "ES-EventId: ${uuid()}" -d '${JSON.stringify(e)}'`
  }).forEach((e) => console.log(e));
}
