// +build integration

package storage

import (
	"database/sql"
	"testing"

	"github.com/ddouglascarr/rooset/assert"
	"github.com/ddouglascarr/rooset/messages"
	proto "github.com/golang/protobuf/proto"
	_ "github.com/lib/pq"
)

func TestPersistAndFetchEventContainers(t *testing.T) {
	evt := &messages.UnitCreatedEvent{
		UnitID:           "a1",
		RequesterID:      "b1",
		Name:             "Test Unit",
		Description:      "The Test Unit",
		URLParameterName: "test-unit",
	}
	container, err := messages.WrapMessage(evt)
	assert.ErrorNotNil(t, err)

	db, err := sql.Open(
		"postgres",
		"user=postgres dbname=rooset_test_0 host=localhost sslmode=disable port=5433",
	)
	assert.ErrorNotNil(t, err)
	defer db.Close()

	tx, err := db.Begin()
	assert.ErrorNotNil(t, err)
	defer tx.Rollback()

	err = persistEventContainers(tx, []messages.MessageContainer{container})
	assert.ErrorNotNil(t, err)

	results, _, err := fetchEventContainers(tx, 0)
	assert.ErrorNotNil(t, err)
	result := results[len(results)-1]

	assert.Equals(t, "sets AggregateRootID on container", result.AggregateRootID, evt.UnitID)
	assert.Equals(t, "sets MessageType on container", result.MessageType, "messages.UnitCreatedEvent")
	resultEvt := &messages.UnitCreatedEvent{}

	err = proto.Unmarshal(result.Message, resultEvt)
	assert.ErrorNotNil(t, err)
	assert.MessageEquals(t, "wrapped message is correct", resultEvt, evt)
}

func TestFetchProjection(t *testing.T) {
	proj := &messages.UnitProjection{
		UnitID:           "a1",
		Name:             "Test Unit",
		Description:      "The Test Unit",
		URLParameterName: "test-unit",
	}
	container, err := messages.WrapMessage(proj)
	assert.ErrorNotNil(t, err)

	db, err := sql.Open(
		"postgres",
		"user=postgres dbname=rooset_test_0 host=localhost sslmode=disable port=5433",
	)
	assert.ErrorNotNil(t, err)
	defer db.Close()

	tx, err := db.Begin()
	assert.ErrorNotNil(t, err)
	defer tx.Rollback()

	err = persistEventContainers(tx, []messages.MessageContainer{container})
	assert.ErrorNotNil(t, err)

	resultContainer, err := FetchProjectionContainer(
		tx,
		"messages.UnitProjection",
		"a1",
	)
	assert.ErrorNotNil(t, err)

	assert.Equals(t, "fetches AggregateRootID correctly", resultContainer.AggregateRootID, "a1")
	assert.Equals(t, "fetches MessageType correctly", resultContainer.MessageType, "messages.UnitProjection")
	resultProj := &messages.UnitProjection{}

	err = proto.Unmarshal(resultContainer.Message, resultProj)
	assert.ErrorNotNil(t, err)
	assert.MessageEquals(t, "Wrapped message is correct", resultProj, proj)
}
