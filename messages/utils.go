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

// BuildAggregateRootID builds the container root id from a message ID
func BuildAggregateRootID(message proto.Message) (string, error) {
	descriptorMessage, ok := message.(descriptor.Message)
	if !ok {
		return "", errors.New("not a descripable message")
	}
	id, err := GetAggregateRootID(descriptorMessage)
	if err != nil {
		return "", err
	}
	return id, nil
}

// GetMessageType returns the message type of a rooset message as a string
func GetMessageType(message proto.Message) string {
	return proto.MessageName(message)
}

// WrapMessage takes a message and creates a MessageContainer for it
func WrapMessage(message proto.Message) (MessageContainer, error) {
	typename := GetMessageType(message)
	container := MessageContainer{MessageType: typename}
	id, err := BuildAggregateRootID(message)
	if err != nil {
		return container, err
	}
	container.AggregateRootID = id
	bMessage, err := proto.Marshal(message)
	if err != nil {
		return container, err
	}
	container.Payload = bMessage

	return container, nil
}

// GetAggregateRootField gets the field property, as a string, for a rooset message
func GetAggregateRootField(message descriptor.Message) (string, error) {
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

type tmpContainer struct {
	AggregateRootID string
	MessageType     string
	Payload         interface{}
}

// UnmarshalJSONMessageContainer parses a json rooset message container
func UnmarshalJSONMessageContainer(data []byte) (*MessageContainer, error) {
	container := &MessageContainer{}
	tmpContainer := &tmpContainer{}
	if err := json.Unmarshal(data, &tmpContainer); err != nil {
		return container, err
	}
	jPayload, err := json.Marshal(tmpContainer.Payload)
	if err != nil {
		return container, err
	}

	bMessage, err := unmarshalJSONMessageContainerPayload(tmpContainer.MessageType, jPayload)
	if err != nil {
		return container, err
	}
	container.AggregateRootID = tmpContainer.AggregateRootID
	container.MessageType = tmpContainer.MessageType
	container.Payload = bMessage

	return container, nil
}

func unmarshalJSONMessageContainerPayload(messageType string, jPayload []byte) ([]byte, error) {
	reader := bytes.NewReader(jPayload)
	var bMessage []byte
	mt := proto.MessageType(messageType)
	mi := reflect.New(mt.Elem()).Interface()
	message, ok := mi.(proto.Message)
	if !ok {
		return nil, fmt.Errorf("No message of type %s", messageType)
	}
	if err := jsonpb.Unmarshal(reader, message); err != nil {
		return bMessage, err
	}

	bMessage, err := proto.Marshal(message)
	if err != nil {
		return bMessage, err
	}

	return bMessage, nil
}

// MarshalJSONMessageContainer marshals a message container to json
func MarshalJSONMessageContainer(container MessageContainer) ([]byte, error) {
	var bMessage bytes.Buffer
	// _ = bufio.NewWriter(&bMessage)
	mt := proto.MessageType(container.MessageType)
	mi := reflect.New(mt.Elem()).Interface()
	message, ok := mi.(proto.Message)
	if !ok {
		return bMessage.Bytes(), fmt.Errorf("No message of type %s", container.MessageType)
	}

	if err := proto.Unmarshal(container.Payload, message); err != nil {
		return bMessage.Bytes(), err
	}

	ma := jsonpb.Marshaler{}
	if err := ma.Marshal(&bMessage, message); err != nil {
		return bMessage.Bytes(), err
	}

	var out struct {
		AggregateRootID string
		MessageType     string
		Payload         json.RawMessage
	}
	out.AggregateRootID = container.AggregateRootID
	out.MessageType = container.MessageType
	out.Payload = bMessage.Bytes()

	cbuf, err := json.Marshal(out)
	if err != nil {
		return nil, err
	}

	return cbuf, nil
}
