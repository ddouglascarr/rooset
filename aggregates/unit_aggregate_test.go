package aggregates_test

import (
	"testing"

	"github.com/ddouglascarr/rooset/aggregates"
	"github.com/ddouglascarr/rooset/assert"
	"github.com/ddouglascarr/rooset/messages"
)

func TestCreateUnitCommand(t *testing.T) {
	aggregateFetcher := BuildTestAggregateFetcher([]messages.Message{})
	expectedEvt := &messages.UnitCreatedEvent{
		UnitID:           "unit0",
		RequesterID:      "member0",
		Name:             "Unit 0",
		Description:      "The Unit 0",
		URLParameterName: "unit-0",
	}

	evt, rejectionReason, err := aggregates.HandleCommand(
		aggregateFetcher,
		"UnitID",
		"unit0",
		&messages.CreateUnitCommand{
			UnitID:           "unit0",
			RequesterID:      "member0",
			Name:             "Unit 0",
			Description:      "The Unit 0",
			URLParameterName: "unit-0",
		},
	)

	assert.ErrorIsNil(t, err)
	assertNotRejected(t, rejectionReason)
	assert.MessageEquals(t, "event generated", expectedEvt, evt)
}

func TestCreateUnitCommandRejectsIfUnitAlreadyExists(t *testing.T) {
	aggregateFetcher := BuildTestAggregateFetcher([]messages.Message{
		&messages.UnitCreatedEvent{
			UnitID: "unit0",
		},
	})

	_, rejectionReason, err := aggregates.HandleCommand(
		aggregateFetcher,
		"UnitID",
		"unit0",
		&messages.CreateUnitCommand{
			UnitID:           "unit0",
			RequesterID:      "member0",
			Name:             "Unit 0",
			Description:      "The Unit 0",
			URLParameterName: "unit-0",
		},
	)

	assert.ErrorIsNil(t, err)
	assertRejected(t, rejectionReason)
}

func TestGrantPrivilege(t *testing.T) {
	aggregateFetcher := BuildTestAggregateFetcher([]messages.Message{
		&messages.UnitCreatedEvent{
			UnitID:      "unit0",
			RequesterID: "member0",
		},
	})

	evt, rejectionReason, _ := aggregates.HandleCommand(
		aggregateFetcher,
		"UnitID",
		"unit0",
		&messages.GrantPrivilegeCommand{
			UnitID:      "unit0",
			RequesterID: "member0",
			MemberID:    "member1",
			VotingRight: true,
			Weight:      1,
		},
	)

	assertNotRejected(t, rejectionReason)
	assert.MessageEquals(t, "event generated", evt, &messages.PrivilegeGrantedEvent{
		UnitID:      "unit0",
		RequesterID: "member0",
		MemberID:    "member1",
		VotingRight: true,
		Weight:      1,
	})
}

func TestGrantPrivilegeRejectsIfPrivilegeAlreadyExists(t *testing.T) {
	aggregateFetcher := BuildTestAggregateFetcher([]messages.Message{
		&messages.UnitCreatedEvent{
			UnitID:      "unit0",
			RequesterID: "member0",
		},
		&messages.PrivilegeGrantedEvent{
			UnitID:      "unit0",
			RequesterID: "member0",
			MemberID:    "member1",
			VotingRight: true,
			Weight:      1,
		},
	})

	_, rejectionReason, _ := aggregates.HandleCommand(
		aggregateFetcher,
		"UnitID",
		"unit0",
		&messages.GrantPrivilegeCommand{
			UnitID:      "unit0",
			RequesterID: "member0",
			MemberID:    "member1",
			VotingRight: true,
			Weight:      1,
		},
	)

	assertRejected(t, rejectionReason)
}

func TestRevokePrivilege(t *testing.T) {
	aggregateFetcher := BuildTestAggregateFetcher([]messages.Message{
		&messages.UnitCreatedEvent{
			UnitID:      "unit0",
			RequesterID: "member0",
		},
		&messages.PrivilegeGrantedEvent{
			UnitID:      "unit0",
			RequesterID: "member0",
			MemberID:    "member1",
			VotingRight: true,
			Weight:      2,
		},
	})

	evt, rejectionReason, _ := aggregates.HandleCommand(
		aggregateFetcher,
		"UnitID",
		"unit0",
		&messages.RevokePrivilegeCommand{
			UnitID:      "unit0",
			RequesterID: "member0",
			MemberID:    "member1",
		},
	)

	assertNotRejected(t, rejectionReason)
	assert.MessageEquals(t, "event generated", evt, &messages.PrivilegeRevokedEvent{
		UnitID:      "unit0",
		RequesterID: "member0",
		MemberID:    "member1",
		Weight:      2,
	})
}

func TestRevokePrivilegeRejectsIfAlreadyRevoked(t *testing.T) {
	aggregateFetcher := BuildTestAggregateFetcher([]messages.Message{
		&messages.UnitCreatedEvent{
			UnitID:      "unit0",
			RequesterID: "member0",
		},
		&messages.PrivilegeGrantedEvent{
			UnitID:      "unit0",
			RequesterID: "member0",
			MemberID:    "member1",
			VotingRight: true,
			Weight:      2,
		},
		&messages.PrivilegeRevokedEvent{
			UnitID:      "unit0",
			RequesterID: "member0",
			MemberID:    "member1",
			Weight:      2,
		},
	})

	_, rejectionReason, _ := aggregates.HandleCommand(
		aggregateFetcher,
		"UnitID",
		"unit0",
		&messages.RevokePrivilegeCommand{
			UnitID:      "unit0",
			RequesterID: "member0",
			MemberID:    "member1",
		},
	)

	assertRejected(t, rejectionReason)
}
