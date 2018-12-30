package projections_test

import (
	"testing"

	"github.com/ddouglascarr/rooset/assert"
	"github.com/ddouglascarr/rooset/messages"
	"github.com/ddouglascarr/rooset/projections"
)

func TestUnitCreatedEvtHandler(t *testing.T) {
	evt := &messages.UnitCreatedEvt{
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

	err := projections.UpdateUnitProjection(projection, evt)
	assert.ErrorIsNil(t, err)

	assert.Equals(t, "Projection UnitID set", projection.UnitID, "a1")
	assert.MessageEquals(t, "Projection Updated", []messages.Message{projection}, expectedProjection)
}

func TestPrivilegeGrantedEvtHandler(t *testing.T) {
	projection := &messages.UnitProjection{
		UnitID:      "a1",
		MemberCount: 10,
		TotalWeight: 12,
	}
	evt := &messages.PrivilegeGrantedEvt{
		UnitID:   "a1",
		MemberID: "123",
		Weight:   2,
	}

	err := projections.UpdateUnitProjection(projection, evt)
	assert.ErrorIsNil(t, err)

	assert.Equals(t, "Member Count Incremented", projection.MemberCount, uint32(11))
	assert.Equals(t, "Total Weight Incremented", projection.TotalWeight, uint32(14))
}

func TestPrivilegeRevokedEvtHandler(t *testing.T) {
	projection := &messages.UnitProjection{
		UnitID:      "a1",
		MemberCount: 10,
		TotalWeight: 10,
	}
	evt := &messages.PrivilegeRevokedEvt{
		UnitID: "a1",
		Weight: 2,
	}

	err := projections.UpdateUnitProjection(projection, evt)
	assert.ErrorIsNil(t, err)

	assert.Equals(t, "Member Count Decremented", projection.MemberCount, uint32(9))
	assert.Equals(t, "Total Weight Decremented", projection.TotalWeight, uint32(8))
}
