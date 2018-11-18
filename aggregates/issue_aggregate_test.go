package aggregates_test

import (
	"testing"

	"github.com/ddouglascarr/rooset/aggregates"
	"github.com/ddouglascarr/rooset/assert"
	"github.com/ddouglascarr/rooset/messages"
)

func TestCreateIssueCommand(t *testing.T) {
	aggregateFetcher := BuildTestAggregateFetcher(buildBaseFixture())
	evt, err := aggregates.HandleCommand(
		aggregateFetcher,
		"IssueID",
		"issue0",
		&messages.CreateIssueCommand{
			IssueID:      "issue0",
			InitiativeID: "initiative0",
			UnitID:       "unit0",
			RequesterID:  "member1",
			PolicyID:     "policy0",
			AreaID:       "area0",
			Body:         "Lets do A",
		},
	)

	assert.ErrorIsNil(t, err)
	assert.MessageEquals(
		t,
		"expected event",
		evt,
		&messages.IssueCreatedEvent{
			IssueID:      "issue0",
			InitiativeID: "initiative0",
			UnitID:       "unit0",
			RequesterID:  "member1",
			PolicyID:     "policy0",
			AreaID:       "area0",
			Body:         "Lets do A",
		},
	)
}

func TestCreateIssueCommandFailsOnNonMembershipt(t *testing.T) {
	aggregateFetcher := BuildTestAggregateFetcher(buildBaseFixture())
	_, err := aggregates.HandleCommand(
		aggregateFetcher,
		"IssueID",
		"issue0",
		&messages.CreateIssueCommand{
			IssueID:      "issue0",
			InitiativeID: "initiative0",
			UnitID:       "unit0",
			RequesterID:  "nonmember",
			PolicyID:     "policy0",
			AreaID:       "area0",
			Body:         "Lets do A",
		},
	)

	assertRejected(t, err)
}

func TestCreateIssueCommandFailsForNonInitiativeRightMember(t *testing.T) {
	fixture := append(buildBaseFixture(),
		&messages.PrivilegeGrantedEvent{
			UnitID:          "unit0",
			RequesterID:     "member0",
			MemberID:        "member2",
			VotingRight:     true,
			InitiativeRight: false,
			ManagementRight: false,
			Weight:          1,
		},
	)
	aggregateFetcher := BuildTestAggregateFetcher(fixture)
	_, err := aggregates.HandleCommand(
		aggregateFetcher,
		"IssueID",
		"issue0",
		&messages.CreateIssueCommand{
			IssueID:      "issue0",
			InitiativeID: "initiative0",
			UnitID:       "unit0",
			RequesterID:  "member2",
			PolicyID:     "policy0",
			AreaID:       "area0",
			Body:         "Lets do A",
		},
	)

	assertRejected(t, err)
}

func TestCreateIssueCommandFailsOnMissingArea(t *testing.T) {
	aggregateFetcher := BuildTestAggregateFetcher(buildBaseFixture())
	_, err := aggregates.HandleCommand(
		aggregateFetcher,
		"IssueID",
		"issue0",
		&messages.CreateIssueCommand{
			IssueID:      "issue0",
			InitiativeID: "initiative0",
			UnitID:       "unit0",
			RequesterID:  "member1",
			PolicyID:     "policy0",
			AreaID:       "area1",
			Body:         "Lets do A",
		},
	)

	assertRejected(t, err)
}

func TestCreateIssueCommandFailsOnIncorrectPolicy(t *testing.T) {
	fixture := append(buildBaseFixture())
	aggregateFetcher := BuildTestAggregateFetcher(fixture)
	_, err := aggregates.HandleCommand(
		aggregateFetcher,
		"IssueID",
		"issue0",
		&messages.CreateIssueCommand{
			IssueID:      "issue0",
			InitiativeID: "initiative0",
			UnitID:       "unit0",
			RequesterID:  "member1",
			PolicyID:     "policy2",
			AreaID:       "area0",
			Body:         "Lets do A",
		},
	)

	assertRejected(t, err)
}

func TestCreateIssueCommandFailsOnDuplicate(t *testing.T) {
	fixture := append(buildBaseFixture(),
		&messages.IssueCreatedEvent{
			IssueID:      "issue0",
			InitiativeID: "initiative0",
			UnitID:       "unit0",
			RequesterID:  "member1",
			PolicyID:     "policy0",
			AreaID:       "area0",
			Body:         "Lets do A",
		},
	)
	aggregateFetcher := BuildTestAggregateFetcher(fixture)
	_, err := aggregates.HandleCommand(
		aggregateFetcher,
		"IssueID",
		"issue0",
		&messages.CreateIssueCommand{
			IssueID:      "issue0",
			InitiativeID: "initiative0",
			UnitID:       "unit0",
			RequesterID:  "member1",
			PolicyID:     "policy0",
			AreaID:       "area0",
			Body:         "Lets do A",
		},
	)

	assertRejected(t, err)
}

func TestCreateInitiativeRejected(t *testing.T) {
	fixture := append(buildBaseFixture(),
		&messages.IssueCreatedEvent{
			IssueID:      "issue0",
			InitiativeID: "initiative0",
			UnitID:       "unit0",
			RequesterID:  "member1",
			PolicyID:     "policy0",
			AreaID:       "area0",
			Body:         "Lets do A",
		},
	)
	aggregateFetcher := BuildTestAggregateFetcher(fixture)

	evt, err := aggregates.HandleCommand(
		aggregateFetcher,
		"IssueID",
		"issue0",
		&messages.CreateInitiativeRejectedCommand{
			IssueID:      "issue0",
			InitiativeID: "initiative0",
			Reason:       "not good folks",
		},
	)
	assert.ErrorIsNil(t, err)
	assert.MessageEquals(
		t,
		"expected event",
		evt,
		&messages.InitiativeCreatedRejectedEvent{
			IssueID:      "issue0",
			InitiativeID: "initiative0",
			Reason:       "not good folks",
		},
	)
}

func TestCreateInitiativeRejectedFailsOnDuplicate(t *testing.T) {
	fixture := append(buildBaseFixture(),
		&messages.IssueCreatedEvent{
			IssueID:      "issue0",
			InitiativeID: "initiative0",
			UnitID:       "unit0",
			RequesterID:  "member1",
			PolicyID:     "policy0",
			AreaID:       "area0",
			Body:         "Lets do A",
		},
		&messages.InitiativeCreatedRejectedEvent{
			IssueID:      "issue0",
			InitiativeID: "initiative0",
		},
	)
	aggregateFetcher := BuildTestAggregateFetcher(fixture)

	_, err := aggregates.HandleCommand(
		aggregateFetcher,
		"IssueID",
		"issue0",
		&messages.CreateInitiativeRejectedCommand{
			IssueID:      "issue0",
			InitiativeID: "initiative0",
		},
	)
	assertRejected(t, err)
}

func TestCreateInitiativeAccepted(t *testing.T) {
	fixture := append(buildBaseFixture(),
		&messages.IssueCreatedEvent{
			IssueID:      "issue0",
			InitiativeID: "initiative0",
			UnitID:       "unit0",
			RequesterID:  "member1",
			PolicyID:     "policy0",
			AreaID:       "area0",
			Body:         "Lets do A",
		},
	)
	aggregateFetcher := BuildTestAggregateFetcher(fixture)

	evt, err := aggregates.HandleCommand(
		aggregateFetcher,
		"IssueID",
		"issue0",
		&messages.CreateInitiativeAcceptedCommand{
			IssueID:      "issue0",
			InitiativeID: "initiative0",
		},
	)
	assert.ErrorIsNil(t, err)
	assert.MessageEquals(
		t,
		"expected event",
		evt,
		&messages.InitiativeCreatedAcceptedEvent{
			IssueID:      "issue0",
			InitiativeID: "initiative0",
			UnitID:       "unit0",
			PolicyID:     "policy0",
			AreaID:       "area0",
			Body:         "Lets do A",
		},
	)
}

func TestCreateInitiativeAcceptedFailsOnDuplicate(t *testing.T) {
	fixture := append(buildBaseFixture(),
		&messages.IssueCreatedEvent{
			IssueID:      "issue0",
			InitiativeID: "initiative0",
			UnitID:       "unit0",
			RequesterID:  "member1",
			PolicyID:     "policy0",
			AreaID:       "area0",
			Body:         "Lets do A",
		},
		&messages.InitiativeCreatedAcceptedEvent{
			IssueID:      "issue0",
			InitiativeID: "initiative0",
			UnitID:       "unit0",
			RequesterID:  "member1",
			PolicyID:     "policy0",
			AreaID:       "area0",
			Body:         "Lets do A",
		},
	)
	aggregateFetcher := BuildTestAggregateFetcher(fixture)

	_, err := aggregates.HandleCommand(
		aggregateFetcher,
		"IssueID",
		"issue0",
		&messages.CreateInitiativeAcceptedCommand{
			IssueID:      "issue0",
			InitiativeID: "initiative0",
		},
	)
	assertRejected(t, err)
}

func TestCreateInitiative(t *testing.T) {
	fixture := append(buildBaseFixture(),
		&messages.IssueCreatedEvent{
			IssueID:      "issue0",
			InitiativeID: "initiative0",
			UnitID:       "unit0",
			RequesterID:  "member1",
			PolicyID:     "policy0",
			AreaID:       "area0",
			Body:         "Lets do A",
		},
		&messages.InitiativeCreatedAcceptedEvent{
			IssueID:      "issue0",
			InitiativeID: "initiative0",
			UnitID:       "unit0",
			RequesterID:  "member1",
			PolicyID:     "policy0",
			AreaID:       "area0",
			Body:         "Lets do A",
		},
	)
	aggregateFetcher := BuildTestAggregateFetcher(fixture)
	evt, err := aggregates.HandleCommand(
		aggregateFetcher,
		"IssueID",
		"issue0",
		&messages.CreateInitiativeRequestedCommand{
			IssueID:      "issue0",
			InitiativeID: "initiative1",
			RequesterID:  "member1",
			Body:         "Lets do B",
		},
	)

	assert.ErrorIsNil(t, err)
	assert.MessageEquals(
		t,
		"expected event",
		evt,
		&messages.InitiativeCreatedRequestedEvent{
			IssueID:      "issue0",
			InitiativeID: "initiative1",
			UnitID:       "unit0",
			RequesterID:  "member1",
			PolicyID:     "policy0",
			AreaID:       "area0",
			Body:         "Lets do B",
		},
	)
}

func TestCreateInitiativeFailsOnDuplicateInitial(t *testing.T) {
	fixture := append(buildBaseFixture(),
		&messages.IssueCreatedEvent{
			IssueID:      "issue0",
			InitiativeID: "initiative0",
			UnitID:       "unit0",
			RequesterID:  "member1",
			PolicyID:     "policy0",
			AreaID:       "area0",
			Body:         "Lets do A",
		},
	)
	aggregateFetcher := BuildTestAggregateFetcher(fixture)
	_, err := aggregates.HandleCommand(
		aggregateFetcher,
		"IssueID",
		"issue0",
		&messages.CreateInitiativeRequestedCommand{
			IssueID:      "issue0",
			InitiativeID: "initiative0",
			RequesterID:  "member1",
			Body:         "Lets do B",
		},
	)

	assertRejected(t, err)
}

func TestCreateInitiativeFailsOnDuplicate(t *testing.T) {
	fixture := append(buildBaseFixture(),
		&messages.IssueCreatedEvent{
			IssueID:      "issue0",
			InitiativeID: "initiative0",
			UnitID:       "unit0",
			RequesterID:  "member1",
			PolicyID:     "policy0",
			AreaID:       "area0",
			Body:         "Lets do A",
		},
		&messages.InitiativeCreatedAcceptedEvent{
			IssueID:      "issue0",
			InitiativeID: "initiative0",
			UnitID:       "unit0",
			RequesterID:  "member1",
			PolicyID:     "policy0",
			AreaID:       "area0",
			Body:         "Lets do A",
		},
		&messages.InitiativeCreatedRequestedEvent{
			IssueID:      "issue0",
			InitiativeID: "initiative1",
			UnitID:       "unit0",
			RequesterID:  "member1",
			PolicyID:     "policy0",
			AreaID:       "area0",
			Body:         "Lets do B",
		},
	)
	aggregateFetcher := BuildTestAggregateFetcher(fixture)
	_, err := aggregates.HandleCommand(
		aggregateFetcher,
		"IssueID",
		"issue0",
		&messages.CreateInitiativeRequestedCommand{
			IssueID:      "issue0",
			InitiativeID: "initiative1",
			RequesterID:  "member1",
			Body:         "Lets do B",
		},
	)

	assertRejected(t, err)
}

//
// utils
//

func buildBaseFixture() []messages.Message {
	return []messages.Message{
		&messages.UnitCreatedEvent{
			UnitID:      "unit0",
			RequesterID: "member0",
		},
		&messages.PrivilegeGrantedEvent{
			UnitID:          "unit0",
			RequesterID:     "member0",
			MemberID:        "member1",
			VotingRight:     true,
			InitiativeRight: true,
			ManagementRight: false,
			Weight:          1,
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
	}
}
