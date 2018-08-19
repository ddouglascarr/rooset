package messages_test

import (
	"reflect"
	"testing"

	"github.com/ddouglascarr/rooset/assert"
	"github.com/ddouglascarr/rooset/messages"
	proto "github.com/golang/protobuf/proto"
)

func TestGetAggregateRootField(t *testing.T) {
	evt := messages.UnitCreatedEvent{}
	id, err := messages.GetAggregateRootField(&evt)

	assert.ErrorNotNil(t, err)
	assert.Equals(t, "UnitID", id, "UnitID")
}

func TestGetAggregateRootID(t *testing.T) {
	evt := messages.UnitCreatedEvent{
		UnitID: "a1",
	}
	id, err := messages.GetAggregateRootID(&evt)

	assert.ErrorNotNil(t, err)
	assert.Equals(t, "UnitID", id, "a1")
}

func TestBuildAggregateRootID(t *testing.T) {
	evt := messages.UnitCreatedEvent{
		UnitID: "a1",
	}
	id, err := messages.BuildAggregateRootID(&evt)

	assert.ErrorNotNil(t, err)
	assert.Equals(t, "AggregateRootID", id, "a1")
}

func TestWrapMessage(t *testing.T) {
	evt := messages.UnitCreatedEvent{
		UnitID: "a1",
	}
	container, err := messages.WrapMessage(&evt)
	assert.ErrorNotNil(t, err)

	assert.Equals(t, "AggregateRootID set", container.AggregateRootID, "a1")
	assert.Equals(t, "MessageType set", container.MessageType, "messages.UnitCreatedEvent")
	// havent checked if it stores the Message :shrug;
}

func TestUnmarshalJSONMessageContainer(t *testing.T) {
	b := []byte(`{"AggregateRootID": "123", "MessageType": "messages.UnitCreatedEvent", "Message": { "UnitID":"123", "Name": "Bar Unit", "RequesterID": "asdf", "Description": "The Bar Unit", "URLParameterName": "bar"}}`)
	expectedMessage := &messages.UnitCreatedEvent{
		UnitID:           "123",
		Name:             "Bar Unit",
		RequesterID:      "asdf",
		Description:      "The Bar Unit",
		URLParameterName: "bar",
	}

	container, err := messages.UnmarshalJSONMessageContainer(b)
	assert.ErrorNotNil(t, err)

	assert.Equals(t, "Has AggregateRootID", container.AggregateRootID, "123")
	assert.Equals(t, "Has MessageType", container.MessageType, "messages.UnitCreatedEvent")

	message := messages.UnitCreatedEvent{}
	proto.Unmarshal(container.Message, &message)

	assert.MessageEquals(t, "Has Message", &message, expectedMessage)
}

func TestMessageType(t *testing.T) {
	evt := &messages.UnitCreatedEvent{
		UnitID:           "123",
		Name:             "Bar Unit",
		RequesterID:      "asdf",
		Description:      "The Bar Unit",
		URLParameterName: "bar",
	}
	name := proto.MessageName(evt)
	assert.Equals(t, "MessageName created", name, "messages.UnitCreatedEvent")

	mt := proto.MessageType(name)
	assert.Equals(t, "MessageType", mt, reflect.TypeOf(evt))
	assert.Equals(t, "MessageName from type", mt.String(), "*messages.UnitCreatedEvent")

}

// func TestMarshalJSONMessageContainer(t *testing.T) {
// 	expectedJSON := []byte(`{"AggregateRootID": "123", "MessageType": "messages.UnitCreatedEvent", "Message": { "UnitID":"123", "Name": "Bar Unit", "RequesterID": "asdf", "Description": "The Bar Unit", "URLParameterName": "bar"}}`)
//
// 	message := &messages.UnitCreatedEvent{
// 		UnitID:           "123",
// 		Name:             "Bar Unit",
// 		RequesterID:      "asdf",
// 		Description:      "The Bar Unit",
// 		URLParameterName: "bar",
// 	}
// 	container, err := messages.WrapMessage(message)
// 	assert.ErrorNotNil(t, err)
// 	returnedJSON, err := messages.MarshalJSONMessageContainer(container)
// 	assert.ErrorNotNil(t, err)
// 	assert.Equals(t, "JSON generated correctly", string(expectedJSON), string(returnedJSON))
// }
