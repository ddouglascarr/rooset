// +build integration

package storage

import (
	"database/sql"
	"testing"

	"github.com/ddouglascarr/rooset/assert"
	"github.com/ddouglascarr/rooset/messages"
	_ "github.com/lib/pq"
)

func TestFetchMessagesSinceSeq(t *testing.T) {
	evt := &messages.UnitCreatedEvent{
		UnitID:           "a1",
		RequesterID:      "b1",
		Name:             "Test Unit",
		Description:      "The Test Unit",
		URLParameterName: "test-unit",
	}

	db, err := sql.Open(
		"postgres",
		"user=postgres dbname=rooset_test_0 host=localhost sslmode=disable port=5433",
	)
	assert.ErrorIsNil(t, err)
	defer db.Close()

	tx, err := db.Begin()
	assert.ErrorIsNil(t, err)
	defer tx.Rollback()

	err = PersistMessages(tx, []messages.Message{evt})
	assert.ErrorIsNil(t, err)

	results, _, err := FetchMessagesSinceSeq(tx, 0, 100)
	assert.ErrorIsNil(t, err)
	result := results[len(results)-1]

	assert.MessageEquals(t, "message is correct", result, evt)
}

func TestFetchProjection(t *testing.T) {
	proj := &messages.UnitProjection{
		UnitID:           "a1",
		Name:             "Test Unit",
		Description:      "The Test Unit",
		URLParameterName: "test-unit",
	}
	resultProj := &messages.UnitProjection{}

	db, err := sql.Open(
		"postgres",
		"user=postgres dbname=rooset_test_0 host=localhost sslmode=disable port=5433",
	)
	assert.ErrorIsNil(t, err)
	defer db.Close()

	tx, err := db.Begin()
	assert.ErrorIsNil(t, err)
	defer tx.Rollback()

	err = PersistMessages(tx, []messages.Message{proj})
	assert.ErrorIsNil(t, err)

	err = FetchProjection(tx, resultProj, "a1")
	assert.ErrorIsNil(t, err)

	assert.MessageEquals(t, "Projection recreated identically", resultProj, proj)
}

func TestFetchProjectionHandlesNonExistantProj(t *testing.T) {
	resultProj := &messages.UnitProjection{}
	expectedProj := &messages.UnitProjection{}

	db, err := sql.Open(
		"postgres",
		"user=postgres dbname=rooset_test_0 host=localhost sslmode=disable port=5433",
	)
	assert.ErrorIsNil(t, err)
	defer db.Close()

	tx, err := db.Begin()
	assert.ErrorIsNil(t, err)
	defer tx.Rollback()

	err = FetchProjection(tx, resultProj, "a1")
	assert.ErrorIsNil(t, err)

	assert.MessageEquals(t, "Leaves the projection along", resultProj, expectedProj)
}
