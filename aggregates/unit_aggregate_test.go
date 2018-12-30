package aggregates_test

import (
	"testing"

	"github.com/ddouglascarr/rooset/aggregates"
	"github.com/ddouglascarr/rooset/assert"
	"github.com/ddouglascarr/rooset/messages"
)

func TestCreateUnitCmd(t *testing.T) {
	aggregateFetcher := BuildTestAggregateFetcher([]messages.Message{})
	expectedEvt := &messages.UnitCreatedEvt{
		UnitID:           "unit0",
		RequesterID:      "member0",
		Name:             "Unit 0",
		Description:      "The Unit 0",
		URLParameterName: "unit-0",
	}

	evt, err := aggregates.HandleCmd(
		aggregateFetcher,
		"UnitID",
		"unit0",
		&messages.CreateUnitCmd{
			UnitID:           "unit0",
			RequesterID:      "member0",
			Name:             "Unit 0",
			Description:      "The Unit 0",
			URLParameterName: "unit-0",
		},
	)

	assert.ErrorIsNil(t, err)
	assert.MessageEquals(t, "event generated", evt, expectedEvt)
}

func TestCreateUnitCmdRejectsIfUnitAlreadyExists(t *testing.T) {
	aggregateFetcher := BuildTestAggregateFetcher([]messages.Message{
		&messages.UnitCreatedEvt{
			UnitID: "unit0",
		},
	})

	_, err := aggregates.HandleCmd(
		aggregateFetcher,
		"UnitID",
		"unit0",
		&messages.CreateUnitCmd{
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
		&messages.UnitCreatedEvt{
			UnitID:      "unit0",
			RequesterID: "member0",
		},
	})

	evt, err := aggregates.HandleCmd(
		aggregateFetcher,
		"UnitID",
		"unit0",
		&messages.GrantPrivilegeCmd{
			UnitID:      "unit0",
			RequesterID: "member0",
			MemberID:    "member1",
			VotingRight: true,
			Weight:      1,
		},
	)

	assert.ErrorIsNil(t, err)
	assert.MessageEquals(t, "event generated", evt, &messages.PrivilegeGrantedEvt{
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
		&messages.UnitCreatedEvt{
			UnitID:      "unit0",
			RequesterID: "anotherMember",
		},
	})

	_, err := aggregates.HandleCmd(
		aggregateFetcher,
		"UnitID",
		"unit0",
		&messages.GrantPrivilegeCmd{
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
		&messages.UnitCreatedEvt{
			UnitID:      "unit0",
			RequesterID: "member0",
		},
		&messages.PrivilegeGrantedEvt{
			UnitID:      "unit0",
			RequesterID: "member0",
			MemberID:    "member1",
			VotingRight: true,
			Weight:      1,
		},
	})

	_, err := aggregates.HandleCmd(
		aggregateFetcher,
		"UnitID",
		"unit0",
		&messages.GrantPrivilegeCmd{
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
		&messages.UnitCreatedEvt{
			UnitID:      "unit0",
			RequesterID: "member0",
		},
		&messages.PrivilegeGrantedEvt{
			UnitID:      "unit0",
			RequesterID: "member0",
			MemberID:    "member1",
			VotingRight: true,
			Weight:      2,
		},
	})

	evt, err := aggregates.HandleCmd(
		aggregateFetcher,
		"UnitID",
		"unit0",
		&messages.RevokePrivilegeCmd{
			UnitID:      "unit0",
			RequesterID: "member0",
			MemberID:    "member1",
		},
	)

	assert.ErrorIsNil(t, err)
	assert.MessageEquals(t, "event generated", evt, &messages.PrivilegeRevokedEvt{
		UnitID:      "unit0",
		RequesterID: "member0",
		MemberID:    "member1",
		Weight:      2,
	})
}

func TestRevokePrivilegeRejectsIfAlreadyRevoked(t *testing.T) {
	aggregateFetcher := BuildTestAggregateFetcher([]messages.Message{
		&messages.UnitCreatedEvt{
			UnitID:      "unit0",
			RequesterID: "member0",
		},
		&messages.PrivilegeGrantedEvt{
			UnitID:      "unit0",
			RequesterID: "member0",
			MemberID:    "member1",
			VotingRight: true,
			Weight:      2,
		},
		&messages.PrivilegeRevokedEvt{
			UnitID:      "unit0",
			RequesterID: "member0",
			MemberID:    "member1",
			Weight:      2,
		},
	})

	_, err := aggregates.HandleCmd(
		aggregateFetcher,
		"UnitID",
		"unit0",
		&messages.RevokePrivilegeCmd{
			UnitID:      "unit0",
			RequesterID: "member0",
			MemberID:    "member1",
		},
	)

	assertRejected(t, err)
}

func TestCreateArea(t *testing.T) {
	aggregateFetcher := BuildTestAggregateFetcher([]messages.Message{
		&messages.UnitCreatedEvt{
			UnitID:      "unit0",
			RequesterID: "member0",
		},
	})

	evt, err := aggregates.HandleCmd(
		aggregateFetcher,
		"UnitID",
		"unit0",
		&messages.CreateAreaCmd{
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
		&messages.AreaCreatedEvt{
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
		&messages.UnitCreatedEvt{
			UnitID:      "unit0",
			RequesterID: "member0",
		},
		&messages.AreaCreatedEvt{
			UnitID:      "unit0",
			RequesterID: "member0",
			AreaID:      "area0",
			Name:        "test area",
			Description: "The Test Area",
		},
	})

	_, err := aggregates.HandleCmd(
		aggregateFetcher,
		"UnitID",
		"unit0",
		&messages.CreateAreaCmd{
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
		&messages.UnitCreatedEvt{
			UnitID:      "unit0",
			RequesterID: "member0",
		},
	})

	evt, err := aggregates.HandleCmd(
		aggregateFetcher,
		"UnitID",
		"unit0",
		&messages.CreatePolicyCmd{
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
		&messages.PolicyCreatedEvt{
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
		&messages.UnitCreatedEvt{
			UnitID:      "unit0",
			RequesterID: "member0",
		},
		&messages.PolicyCreatedEvt{
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

	_, err := aggregates.HandleCmd(
		aggregateFetcher,
		"UnitID",
		"unit0",
		&messages.CreatePolicyCmd{
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

func TestAllowAreaPolicyCmd(t *testing.T) {
	aggregateFetcher := BuildTestAggregateFetcher([]messages.Message{
		&messages.UnitCreatedEvt{
			UnitID:      "unit0",
			RequesterID: "member0",
		},
		&messages.PolicyCreatedEvt{
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
		&messages.AreaCreatedEvt{
			UnitID:      "unit0",
			RequesterID: "member0",
			AreaID:      "area0",
			Name:        "test area",
			Description: "The Test Area",
		},
	})

	evt, err := aggregates.HandleCmd(
		aggregateFetcher,
		"UnitID",
		"unit0",
		&messages.AllowAreaPolicyCmd{
			UnitID:      "unit0",
			RequesterID: "member0",
			AreaID:      "area0",
			PolicyID:    "policy0",
		},
	)

	assert.ErrorIsNil(t, err)
	assert.MessageEquals(
		t, "expected event", evt,
		&messages.AreaPolicyAllowedEvt{
			UnitID:      "unit0",
			RequesterID: "member0",
			AreaID:      "area0",
			PolicyID:    "policy0",
		},
	)
}

func TestAllowAreaPolicyCmdRejectesDuplicate(t *testing.T) {
	aggregateFetcher := BuildTestAggregateFetcher([]messages.Message{
		&messages.UnitCreatedEvt{
			UnitID:      "unit0",
			RequesterID: "member0",
		},
		&messages.PolicyCreatedEvt{
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
		&messages.AreaCreatedEvt{
			UnitID:      "unit0",
			RequesterID: "member0",
			AreaID:      "area0",
			Name:        "test area",
			Description: "The Test Area",
		},
		&messages.AreaPolicyAllowedEvt{
			UnitID:      "unit0",
			RequesterID: "member0",
			AreaID:      "area0",
			PolicyID:    "policy0",
		},
	})

	_, err := aggregates.HandleCmd(
		aggregateFetcher,
		"UnitID",
		"unit0",
		&messages.AllowAreaPolicyCmd{
			UnitID:      "unit0",
			RequesterID: "member0",
			AreaID:      "area0",
			PolicyID:    "policy0",
		},
	)

	assertRejected(t, err)
}

func TestDisallowAreaPolicyCmd(t *testing.T) {
	aggregateFetcher := BuildTestAggregateFetcher([]messages.Message{
		&messages.UnitCreatedEvt{
			UnitID:      "unit0",
			RequesterID: "member0",
		},
		&messages.PolicyCreatedEvt{
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
		&messages.AreaCreatedEvt{
			UnitID:      "unit0",
			RequesterID: "member0",
			AreaID:      "area0",
			Name:        "test area",
			Description: "The Test Area",
		},
		&messages.AreaPolicyAllowedEvt{
			UnitID:      "unit0",
			RequesterID: "member0",
			AreaID:      "area0",
			PolicyID:    "policy0",
		},
	})

	evt, err := aggregates.HandleCmd(
		aggregateFetcher,
		"UnitID",
		"unit0",
		&messages.DisallowAreaPolicyCmd{
			UnitID:      "unit0",
			RequesterID: "member0",
			AreaID:      "area0",
			PolicyID:    "policy0",
		},
	)

	assert.ErrorIsNil(t, err)
	assert.MessageEquals(
		t, "expected event", evt,
		&messages.AreaPolicyDisallowedEvt{
			UnitID:      "unit0",
			RequesterID: "member0",
			AreaID:      "area0",
			PolicyID:    "policy0",
		},
	)
}

func TestDisallowAreaPolicyCmdRejectesIfAlreadyNotAllowed(t *testing.T) {
	aggregateFetcher := BuildTestAggregateFetcher([]messages.Message{
		&messages.UnitCreatedEvt{
			UnitID:      "unit0",
			RequesterID: "member0",
		},
		&messages.PolicyCreatedEvt{
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
		&messages.AreaCreatedEvt{
			UnitID:      "unit0",
			RequesterID: "member0",
			AreaID:      "area0",
			Name:        "test area",
			Description: "The Test Area",
		},
	})

	_, err := aggregates.HandleCmd(
		aggregateFetcher,
		"UnitID",
		"unit0",
		&messages.DisallowAreaPolicyCmd{
			UnitID:      "unit0",
			RequesterID: "member0",
			AreaID:      "area0",
			PolicyID:    "policy0",
		},
	)

	assertRejected(t, err)
}
