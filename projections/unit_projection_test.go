package projections_test

import (
	"github.com/ddouglascarr/rooset/assert"
	"github.com/ddouglascarr/rooset/messages"
	"github.com/ddouglascarr/rooset/projections"
	"testing"
)

func TestUnitCreatedEventHandler(t *testing.T) {
	evt := &messages.UnitCreatedEvent{
		UnitID:           "a1",
		RequesterID:      "b1",
		Name:             "Test Unit",
		Description:      "The Test Unit",
		URLParameterName: "test-unit",
	}
	expectedProjection := &messages.UnitProjection{
		UnitID:           "a1",
		Name:             "Test Unit",
		Description:      "The Test Unit",
		URLParameterName: "test-unit",
	}
	projection := &messages.UnitProjection{}
	evtContainer, err := messages.WrapMessage(evt)
	if err != nil {
		t.Log(err)
		t.Fail()
	}

	err = projections.UpdateUnitProjection(projection, evtContainer)
	assert.ErrorNotNil(t, err)

	assert.Equals(t, "Projection UnitID set", projection.UnitID, "a1")
	assert.MessageEquals(t, "Projection Updated", projection, expectedProjection)
}

func TestPrivilegeGrantedEventHandler(t *testing.T) {
	projection := &messages.UnitProjection{
		UnitID:      "a1",
		MemberCount: 10,
		TotalWeight: 12,
	}
	evtContainer, err := messages.WrapMessage(&messages.PrivilegeGrantedEvent{
		UnitID: "a1",
		Weight: 2,
	})
	assert.ErrorNotNil(t, err)

	err = projections.UpdateUnitProjection(projection, evtContainer)
	assert.ErrorNotNil(t, err)

	assert.Equals(t, "Member Count Incremented", projection.MemberCount, uint32(11))
	assert.Equals(t, "Total Weight Incremented", projection.TotalWeight, uint32(14))
}

func TestPrivilegeRevokedEventHandler(t *testing.T) {
	projection := &messages.UnitProjection{
		UnitID:      "a1",
		MemberCount: 10,
		TotalWeight: 10,
	}
	evtContainer, err := messages.WrapMessage(&messages.PrivilegeRevokedEvent{
		UnitID: "a1",
		Weight: 2,
	})
	assert.ErrorNotNil(t, err)

	err = projections.UpdateUnitProjection(projection, evtContainer)
	assert.ErrorNotNil(t, err)

	assert.Equals(t, "Member Count Decremented", projection.MemberCount, uint32(9))
	assert.Equals(t, "Total Weight Decremented", projection.TotalWeight, uint32(8))
}
