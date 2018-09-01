package messages

import (
	"fmt"

	"github.com/golang/protobuf/descriptor"
	"github.com/golang/protobuf/jsonpb"
	proto "github.com/golang/protobuf/proto"

	"bytes"
	"encoding/json"
	"errors"
	"reflect"
)

// Message all messages implement this interface
type Message interface {
	descriptor.Message // superclass of proto.Message
}

// GetMessageType returns the message type of a rooset message as a string
func GetMessageType(message Message) string {
	return proto.MessageName(message)
}

// GetAggregateRootField gets the field property, as a string, for a rooset message
func GetAggregateRootField(message Message) (string, error) {
	_, md := descriptor.ForMessage(message)
	idExtension, err := proto.GetExtension(md.Options, E_AggregateRootField)
	if err != nil {
		return "", err
	}
	idField, ok := idExtension.(*string)
	if !ok {
		return "", errors.New("AggregateRootID is not a string")
	}

	return *idField, nil
}

// GetAggregateRootID gets the value of the AggregateRootField of a rooset message
func GetAggregateRootID(message descriptor.Message) (string, error) {
	fieldName, err := GetAggregateRootField(message)
	if err != nil {
		return "", err
	}
	messageValue := reflect.ValueOf(message)
	fieldValue := reflect.Indirect(messageValue).FieldByName(fieldName)

	return fieldValue.String(), nil
}

// jSONMessageContainer wraps message for JSON serialization
type jSONMessageContainer struct {
	MessageType string
	Message     json.RawMessage
}

// UnmarshalJSONMessage deserialized JSONin the form of { MessageType: "messages.UnitCreatedEvent", Message: { ... } }
func UnmarshalJSONMessage(jSONMsgContainer []byte) (Message, error) {
	container := &jSONMessageContainer{}
	err := json.Unmarshal(jSONMsgContainer, &container)
	if err != nil {
		return nil, err
	}
	reader := bytes.NewReader(container.Message)

	mt := proto.MessageType(container.MessageType)
	mi := reflect.New(mt.Elem()).Interface()
	msg, ok := mi.(Message)
	if !ok {
		return nil, fmt.Errorf("No message of type %s", container.MessageType)
	}

	if err := jsonpb.Unmarshal(reader, msg); err != nil {
		return nil, err
	}

	return msg, nil
}

// MarshalJSONMessage serializes a Message to JSON
func MarshalJSONMessage(msg Message) ([]byte, error) {
	var bMsg bytes.Buffer
	ma := jsonpb.Marshaler{}
	err := ma.Marshal(&bMsg, msg)
	if err != nil {
		return nil, err
	}

	container := jSONMessageContainer{
		MessageType: GetMessageType(msg),
		Message:     bMsg.Bytes(),
	}

	jSONContainer, err := json.Marshal(container)
	if err != nil {
		return nil, err
	}

	return jSONContainer, nil
}

// UnmarshalBMessage deserialize a binary message
func UnmarshalBMessage(messageType string, bMsg []byte) (Message, error) {
	mt := proto.MessageType(messageType)
	mi := reflect.New(mt.Elem()).Interface()
	msg, ok := mi.(Message)
	if !ok {
		return nil, fmt.Errorf("unrecognized MessageType: %s", messageType)
	}

	err := proto.Unmarshal(bMsg, msg)
	if err != nil {
		return msg, err
	}

	return msg, nil
}

// MarshalBMessage serializes a message to protbuf binary
func MarshalBMessage(msg Message) ([]byte, error) {
	return proto.Marshal(msg)
}
