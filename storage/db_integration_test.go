// +build integration

package storage

import (
	"database/sql"
	"errors"
	"fmt"
	"os"
	"strings"
	"testing"

	"github.com/ddouglascarr/rooset/assert"
	"github.com/ddouglascarr/rooset/conf"
	"github.com/ddouglascarr/rooset/messages"
	_ "github.com/lib/pq"
)

func TestFetchMessagesSinceSeq(t *testing.T) {
	evt := &messages.UnitCreatedEvt{
		UnitID:           "a1",
		RequesterID:      "b1",
		Name:             "Test Unit",
		Description:      "The Test Unit",
		URLParameterName: "test-unit",
	}

	db, err := sql.Open(
		"postgres",
		fmt.Sprintf(
			"user=postgres dbname=%s host=%s sslmode=disable port=%s",
			conf.DB.CmdName, conf.DB.CmdHost, os.Getenv("ROOSET_INTEGRATION_TEST_PORT")),
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

	assert.MessageEquals(t, "message is correct", []messages.Message{result}, evt)
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
		fmt.Sprintf(
			"user=postgres dbname=%s host=%s sslmode=disable port=%s",
			conf.DB.CmdName, conf.DB.CmdHost, os.Getenv("ROOSET_INTEGRATION_TEST_PORT")),
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

	assert.MessageEquals(
		t,
		"Projection recreated identically",
		[]messages.Message{resultProj},
		proj,
	)
}

func TestFetchProjectionHandlesNonExistantProj(t *testing.T) {
	resultProj := &messages.UnitProjection{}
	expectedProj := &messages.UnitProjection{}

	db, err := sql.Open(
		"postgres",
		fmt.Sprintf(
			"user=postgres dbname=%s host=%s sslmode=disable port=%s",
			conf.DB.CmdName, conf.DB.CmdHost, os.Getenv("ROOSET_INTEGRATION_TEST_PORT")),
	)
	assert.ErrorIsNil(t, err)
	defer db.Close()

	tx, err := db.Begin()
	assert.ErrorIsNil(t, err)
	defer tx.Rollback()

	err = FetchProjection(tx, resultProj, "a1")
	assert.ErrorIsNil(t, err)

	assert.MessageEquals(
		t,
		"Leaves the projection along",
		[]messages.Message{resultProj},
		expectedProj,
	)
}

type concatAggregate struct {
	Result string
}

func (a *concatAggregate) HandleEvt(msg messages.Message) error {
	evt, ok := msg.(*messages.UnitCreatedEvt)
	if !ok {
		return errors.New("oops")
	}
	var sb strings.Builder
	sb.WriteString(a.Result)
	sb.WriteString(evt.Description)
	a.Result = sb.String()
	return nil
}

func TestFetchAggregate(t *testing.T) {
	db, err := sql.Open(
		"postgres",
		fmt.Sprintf(
			"user=postgres dbname=%s host=%s sslmode=disable port=%s",
			conf.DB.CmdName, conf.DB.CmdHost, os.Getenv("ROOSET_INTEGRATION_TEST_PORT")),
	)
	assert.ErrorIsNil(t, err)
	defer db.Close()

	tx, err := db.Begin()
	assert.ErrorIsNil(t, err)
	defer tx.Rollback()

	err = PersistMessages(tx, []messages.Message{
		&messages.UnitCreatedEvt{UnitID: "a", Description: "1"},
		&messages.UnitCreatedEvt{UnitID: "a", Description: "2"},
		&messages.UnitCreatedEvt{UnitID: "a", Description: "3"},
	})
	assert.ErrorIsNil(t, err)

	aggregate := &concatAggregate{}
	err = FetchAggregate(tx, "a", aggregate)
	assert.ErrorIsNil(t, err)

	assert.Equals(t, "aggregate built", aggregate.Result, "123")
}
