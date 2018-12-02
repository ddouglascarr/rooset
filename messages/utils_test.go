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

	assert.ErrorIsNil(t, err)
	assert.Equals(t, "UnitID", id, "UnitID")
}

func TestGetAggregateRootID(t *testing.T) {
	evt := messages.UnitCreatedEvent{
		UnitID: "a1",
	}
	id, err := messages.GetAggregateRootID(&evt)

	assert.ErrorIsNil(t, err)
	assert.Equals(t, "UnitID", id, "a1")
}

func TestUnmarshalBMessage(t *testing.T) {
	evt := &messages.UnitCreatedEvent{
		UnitID:           "123",
		Name:             "Bar Unit",
		RequesterID:      "asdf",
		Description:      "The Bar Unit",
		URLParameterName: "bar",
	}
	bMsg, err := proto.Marshal(evt)
	assert.ErrorIsNil(t, err)

	resultMsg, err := messages.UnmarshalBMessage("messages.UnitCreatedEvent", bMsg)
	assert.ErrorIsNil(t, err)
	assert.MessageEquals(
		t,
		"message deserializes to abstract Message",
		[]messages.Message{resultMsg},
		evt,
	)

	resultEvt, ok := resultMsg.(*messages.UnitCreatedEvent)
	assert.Equals(t, "result message casts", true, ok)
	assert.MessageEquals(t, "case message equal", []messages.Message{resultEvt}, evt)
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

type descriptionHaver interface {
	proto.Message
	GetDescription() string
}

func getDescription(message proto.Message) string {
	evt, ok := message.(descriptionHaver)
	if !ok {
		return ""
	}
	return evt.GetDescription()
}

func TestCastInMethod(t *testing.T) {
	unitCreatedEvt := &messages.UnitCreatedEvent{
		UnitID:           "123",
		Name:             "Bar Unit",
		RequesterID:      "asdf",
		Description:      "The Bar Unit",
		URLParameterName: "bar",
	}

	privilegeRevokedEvt := &messages.PrivilegeRevokedEvent{
		UnitID:   "456",
		MemberID: "jim",
	}

	returnedDescription := getDescription(unitCreatedEvt)
	emptyDescription := getDescription(privilegeRevokedEvt)

	assert.Equals(t, "returns description", returnedDescription, "The Bar Unit")
	assert.Equals(t, "returns empty string", emptyDescription, "")
}

func getSomething(message proto.Message) string {
	switch evt := message.(type) {
	case *messages.UnitCreatedEvent:
		return evt.UnitID
	case *messages.PrivilegeRevokedEvent:
		return evt.MemberID
	default:
		return ""
	}
}

func TestCaseForStruct(t *testing.T) {
	unitCreatedEvt := &messages.UnitCreatedEvent{
		UnitID:           "123",
		Name:             "Bar Unit",
		RequesterID:      "asdf",
		Description:      "The Bar Unit",
		URLParameterName: "bar",
	}

	privilegeRevokedEvt := &messages.PrivilegeRevokedEvent{
		UnitID:   "456",
		MemberID: "jim",
	}
	privilegeGrantedEvent := &messages.PrivilegeGrantedEvent{
		UnitID:   "789",
		MemberID: "foo",
	}

	unitCreatedResult := getSomething(unitCreatedEvt)
	privilegeRevokedResult := getSomething(privilegeRevokedEvt)
	emptyResult := getSomething(privilegeGrantedEvent)

	assert.Equals(t, "UnitCreatedEvent", unitCreatedResult, "123")
	assert.Equals(t, "PrivilegeRevokedEvent", privilegeRevokedResult, "jim")
	assert.Equals(t, "empty", emptyResult, "")
}
