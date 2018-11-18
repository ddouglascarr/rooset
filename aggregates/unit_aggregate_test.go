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

	evt, err := aggregates.HandleCommand(
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
	assert.MessageEquals(t, "event generated", expectedEvt, evt)
}

func TestCreateUnitCommandRejectsIfUnitAlreadyExists(t *testing.T) {
	aggregateFetcher := BuildTestAggregateFetcher([]messages.Message{
		&messages.UnitCreatedEvent{
			UnitID: "unit0",
		},
	})

	_, err := aggregates.HandleCommand(
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

	assertRejected(t, err)
}

func TestGrantPrivilege(t *testing.T) {
	aggregateFetcher := BuildTestAggregateFetcher([]messages.Message{
		&messages.UnitCreatedEvent{
			UnitID:      "unit0",
			RequesterID: "member0",
		},
	})

	evt, err := aggregates.HandleCommand(
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

	assert.ErrorIsNil(t, err)
	assert.MessageEquals(t, "event generated", evt, &messages.PrivilegeGrantedEvent{
		UnitID:      "unit0",
		RequesterID: "member0",
		MemberID:    "member1",
		VotingRight: true,
		Weight:      1,
	})
}

// tests manager assertion
func TestGrantPrivilegeRejectsIfRequesterNotManager(t *testing.T) {
	aggregateFetcher := BuildTestAggregateFetcher([]messages.Message{
		&messages.UnitCreatedEvent{
			UnitID:      "unit0",
			RequesterID: "anotherMember",
		},
	})

	_, err := aggregates.HandleCommand(
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

	assertRejected(t, err)
}

// tests status assertion
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

	_, err := aggregates.HandleCommand(
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

	assertRejected(t, err)
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

	evt, err := aggregates.HandleCommand(
		aggregateFetcher,
		"UnitID",
		"unit0",
		&messages.RevokePrivilegeCommand{
			UnitID:      "unit0",
			RequesterID: "member0",
			MemberID:    "member1",
		},
	)

	assert.ErrorIsNil(t, err)
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

	_, err := aggregates.HandleCommand(
		aggregateFetcher,
		"UnitID",
		"unit0",
		&messages.RevokePrivilegeCommand{
			UnitID:      "unit0",
			RequesterID: "member0",
			MemberID:    "member1",
		},
	)

	assertRejected(t, err)
}

func TestCreateArea(t *testing.T) {
	aggregateFetcher := BuildTestAggregateFetcher([]messages.Message{
		&messages.UnitCreatedEvent{
			UnitID:      "unit0",
			RequesterID: "member0",
		},
	})

	evt, err := aggregates.HandleCommand(
		aggregateFetcher,
		"UnitID",
		"unit0",
		&messages.CreateAreaCommand{
			UnitID:      "unit0",
			RequesterID: "member0",
			AreaID:      "area0",
			Name:        "test area",
			Description: "The Test Area",
		},
	)

	assert.ErrorIsNil(t, err)
	assert.MessageEquals(
		t,
		"event created",
		evt,
		&messages.AreaCreatedEvent{
			UnitID:      "unit0",
			RequesterID: "member0",
			AreaID:      "area0",
			Name:        "test area",
			Description: "The Test Area",
		},
	)
}

func TestCreateAreaRejectsIfAreaAlreadyExists(t *testing.T) {
	aggregateFetcher := BuildTestAggregateFetcher([]messages.Message{
		&messages.UnitCreatedEvent{
			UnitID:      "unit0",
			RequesterID: "member0",
		},
		&messages.AreaCreatedEvent{
			UnitID:      "unit0",
			RequesterID: "member0",
			AreaID:      "area0",
			Name:        "test area",
			Description: "The Test Area",
		},
	})

	_, err := aggregates.HandleCommand(
		aggregateFetcher,
		"UnitID",
		"unit0",
		&messages.CreateAreaCommand{
			UnitID:      "unit0",
			RequesterID: "member0",
			AreaID:      "area0",
			Name:        "test area",
			Description: "The Test Area",
		},
	)

	assertRejected(t, err)
}

func TestCreatePolicy(t *testing.T) {
	aggregateFetcher := BuildTestAggregateFetcher([]messages.Message{
		&messages.UnitCreatedEvent{
			UnitID:      "unit0",
			RequesterID: "member0",
		},
	})

	evt, err := aggregates.HandleCommand(
		aggregateFetcher,
		"UnitID",
		"unit0",
		&messages.CreatePolicyCommand{
			UnitID:      "unit0",
			RequesterID: "member0",
			PolicyID:    "policy0",
			Description: "test policy",

			MinAdmissionDuration: 604800000, // one week
			MaxAdmissionDuration: 604800000,
			DiscussionDuration:   1209600000, // two weeks
			VerificationDuration: 1209600000,
			VotingDuration:       1209600000,

			IssueQuorumNum:      1,
			IssueQuorumDen:      10,
			InitiativeQuorumNum: 1,
			InitiativeQuorumDen: 5,
		},
	)

	assert.ErrorIsNil(t, err)
	assert.MessageEquals(
		t, "expected event", evt,
		&messages.PolicyCreatedEvent{
			UnitID:      "unit0",
			RequesterID: "member0",
			PolicyID:    "policy0",
			Description: "test policy",

			MinAdmissionDuration: 604800000, // one week
			MaxAdmissionDuration: 604800000,
			DiscussionDuration:   1209600000, // two weeks
			VerificationDuration: 1209600000,
			VotingDuration:       1209600000,

			IssueQuorumNum:      1,
			IssueQuorumDen:      10,
			InitiativeQuorumNum: 1,
			InitiativeQuorumDen: 5,
		},
	)
}

func TestCreatePolicyRejectsDuplicate(t *testing.T) {
	aggregateFetcher := BuildTestAggregateFetcher([]messages.Message{
		&messages.UnitCreatedEvent{
			UnitID:      "unit0",
			RequesterID: "member0",
		},
		&messages.PolicyCreatedEvent{
			UnitID:      "unit0",
			RequesterID: "member0",
			PolicyID:    "policy0",
			Description: "test policy",

			MinAdmissionDuration: 604800000, // one week
			MaxAdmissionDuration: 604800000,
			DiscussionDuration:   1209600000, // two weeks
			VerificationDuration: 1209600000,
			VotingDuration:       1209600000,

			IssueQuorumNum:      1,
			IssueQuorumDen:      10,
			InitiativeQuorumNum: 1,
			InitiativeQuorumDen: 5,
		},
	})

	_, err := aggregates.HandleCommand(
		aggregateFetcher,
		"UnitID",
		"unit0",
		&messages.CreatePolicyCommand{
			UnitID:      "unit0",
			RequesterID: "member0",
			PolicyID:    "policy0",
			Description: "test policy",

			MinAdmissionDuration: 604800000, // one week
			MaxAdmissionDuration: 604800000,
			DiscussionDuration:   1209600000, // two weeks
			VerificationDuration: 1209600000,
			VotingDuration:       1209600000,

			IssueQuorumNum:      1,
			IssueQuorumDen:      10,
			InitiativeQuorumNum: 1,
			InitiativeQuorumDen: 5,
		},
	)

	assertRejected(t, err)
}

func TestAllowAreaPolicyCommand(t *testing.T) {
	aggregateFetcher := BuildTestAggregateFetcher([]messages.Message{
		&messages.UnitCreatedEvent{
			UnitID:      "unit0",
			RequesterID: "member0",
		},
		&messages.PolicyCreatedEvent{
			UnitID:      "unit0",
			RequesterID: "member0",
			PolicyID:    "policy0",
			Description: "test policy",

			MinAdmissionDuration: 604800000, // one week
			MaxAdmissionDuration: 604800000,
			DiscussionDuration:   1209600000, // two weeks
			VerificationDuration: 1209600000,
			VotingDuration:       1209600000,

			IssueQuorumNum:      1,
			IssueQuorumDen:      10,
			InitiativeQuorumNum: 1,
			InitiativeQuorumDen: 5,
		},
		&messages.AreaCreatedEvent{
			UnitID:      "unit0",
			RequesterID: "member0",
			AreaID:      "area0",
			Name:        "test area",
			Description: "The Test Area",
		},
	})

	evt, err := aggregates.HandleCommand(
		aggregateFetcher,
		"UnitID",
		"unit0",
		&messages.AllowAreaPolicyCommand{
			UnitID:      "unit0",
			RequesterID: "member0",
			AreaID:      "area0",
			PolicyID:    "policy0",
		},
	)

	assert.ErrorIsNil(t, err)
	assert.MessageEquals(
		t, "expected event", evt,
		&messages.AreaPolicyAllowedEvent{
			UnitID:      "unit0",
			RequesterID: "member0",
			AreaID:      "area0",
			PolicyID:    "policy0",
		},
	)
}

func TestAllowAreaPolicyCommandRejectesDuplicate(t *testing.T) {
	aggregateFetcher := BuildTestAggregateFetcher([]messages.Message{
		&messages.UnitCreatedEvent{
			UnitID:      "unit0",
			RequesterID: "member0",
		},
		&messages.PolicyCreatedEvent{
			UnitID:      "unit0",
			RequesterID: "member0",
			PolicyID:    "policy0",
			Description: "test policy",

			MinAdmissionDuration: 604800000, // one week
			MaxAdmissionDuration: 604800000,
			DiscussionDuration:   1209600000, // two weeks
			VerificationDuration: 1209600000,
			VotingDuration:       1209600000,

			IssueQuorumNum:      1,
			IssueQuorumDen:      10,
			InitiativeQuorumNum: 1,
			InitiativeQuorumDen: 5,
		},
		&messages.AreaCreatedEvent{
			UnitID:      "unit0",
			RequesterID: "member0",
			AreaID:      "area0",
			Name:        "test area",
			Description: "The Test Area",
		},
		&messages.AreaPolicyAllowedEvent{
			UnitID:      "unit0",
			RequesterID: "member0",
			AreaID:      "area0",
			PolicyID:    "policy0",
		},
	})

	_, err := aggregates.HandleCommand(
		aggregateFetcher,
		"UnitID",
		"unit0",
		&messages.AllowAreaPolicyCommand{
			UnitID:      "unit0",
			RequesterID: "member0",
			AreaID:      "area0",
			PolicyID:    "policy0",
		},
	)

	assertRejected(t, err)
}

func TestDisallowAreaPolicyCommand(t *testing.T) {
	aggregateFetcher := BuildTestAggregateFetcher([]messages.Message{
		&messages.UnitCreatedEvent{
			UnitID:      "unit0",
			RequesterID: "member0",
		},
		&messages.PolicyCreatedEvent{
			UnitID:      "unit0",
			RequesterID: "member0",
			PolicyID:    "policy0",
			Description: "test policy",

			MinAdmissionDuration: 604800000, // one week
			MaxAdmissionDuration: 604800000,
			DiscussionDuration:   1209600000, // two weeks
			VerificationDuration: 1209600000,
			VotingDuration:       1209600000,

			IssueQuorumNum:      1,
			IssueQuorumDen:      10,
			InitiativeQuorumNum: 1,
			InitiativeQuorumDen: 5,
		},
		&messages.AreaCreatedEvent{
			UnitID:      "unit0",
			RequesterID: "member0",
			AreaID:      "area0",
			Name:        "test area",
			Description: "The Test Area",
		},
		&messages.AreaPolicyAllowedEvent{
			UnitID:      "unit0",
			RequesterID: "member0",
			AreaID:      "area0",
			PolicyID:    "policy0",
		},
	})

	evt, err := aggregates.HandleCommand(
		aggregateFetcher,
		"UnitID",
		"unit0",
		&messages.DisallowAreaPolicyCommand{
			UnitID:      "unit0",
			RequesterID: "member0",
			AreaID:      "area0",
			PolicyID:    "policy0",
		},
	)

	assert.ErrorIsNil(t, err)
	assert.MessageEquals(
		t, "expected event", evt,
		&messages.AreaPolicyDisallowedEvent{
			UnitID:      "unit0",
			RequesterID: "member0",
			AreaID:      "area0",
			PolicyID:    "policy0",
		},
	)
}

func TestDisallowAreaPolicyCommandRejectesIfAlreadyNotAllowed(t *testing.T) {
	aggregateFetcher := BuildTestAggregateFetcher([]messages.Message{
		&messages.UnitCreatedEvent{
			UnitID:      "unit0",
			RequesterID: "member0",
		},
		&messages.PolicyCreatedEvent{
			UnitID:      "unit0",
			RequesterID: "member0",
			PolicyID:    "policy0",
			Description: "test policy",

			MinAdmissionDuration: 604800000, // one week
			MaxAdmissionDuration: 604800000,
			DiscussionDuration:   1209600000, // two weeks
			VerificationDuration: 1209600000,
			VotingDuration:       1209600000,

			IssueQuorumNum:      1,
			IssueQuorumDen:      10,
			InitiativeQuorumNum: 1,
			InitiativeQuorumDen: 5,
		},
		&messages.AreaCreatedEvent{
			UnitID:      "unit0",
			RequesterID: "member0",
			AreaID:      "area0",
			Name:        "test area",
			Description: "The Test Area",
		},
	})

	_, err := aggregates.HandleCommand(
		aggregateFetcher,
		"UnitID",
		"unit0",
		&messages.DisallowAreaPolicyCommand{
			UnitID:      "unit0",
			RequesterID: "member0",
			AreaID:      "area0",
			PolicyID:    "policy0",
		},
	)

	assertRejected(t, err)
}
