package aggregates_test

import (
	"testing"

	"github.com/ddouglascarr/rooset/aggregates"
	"github.com/ddouglascarr/rooset/assert"
	"github.com/ddouglascarr/rooset/messages"
)

func TestCreateIssueCmd(t *testing.T) {
	aggregateFetcher := BuildTestAggregateFetcher(buildBaseFixture())
	evts, err := aggregates.HandleCmd(
		aggregateFetcher,
		"IssueID",
		"issue0",
		&messages.CreateIssueCmd{
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
		evts,
		&messages.IssueCreatedEvt{
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

func TestCreateIssueCmdFailsOnNonMembershipt(t *testing.T) {
	aggregateFetcher := BuildTestAggregateFetcher(buildBaseFixture())
	_, err := aggregates.HandleCmd(
		aggregateFetcher,
		"IssueID",
		"issue0",
		&messages.CreateIssueCmd{
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

func TestCreateIssueCmdFailsForNonInitiativeRightMember(t *testing.T) {
	fixture := append(buildBaseFixture(),
		&messages.PrivilegeGrantedEvt{
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
	_, err := aggregates.HandleCmd(
		aggregateFetcher,
		"IssueID",
		"issue0",
		&messages.CreateIssueCmd{
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

func TestCreateIssueCmdFailsOnMissingArea(t *testing.T) {
	aggregateFetcher := BuildTestAggregateFetcher(buildBaseFixture())
	_, err := aggregates.HandleCmd(
		aggregateFetcher,
		"IssueID",
		"issue0",
		&messages.CreateIssueCmd{
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

func TestCreateIssueCmdFailsOnIncorrectPolicy(t *testing.T) {
	fixture := append(buildBaseFixture())
	aggregateFetcher := BuildTestAggregateFetcher(fixture)
	_, err := aggregates.HandleCmd(
		aggregateFetcher,
		"IssueID",
		"issue0",
		&messages.CreateIssueCmd{
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

func TestCreateIssueCmdFailsOnDuplicate(t *testing.T) {
	fixture := append(buildBaseFixture(),
		&messages.IssueCreatedEvt{
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
	_, err := aggregates.HandleCmd(
		aggregateFetcher,
		"IssueID",
		"issue0",
		&messages.CreateIssueCmd{
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
		&messages.IssueCreatedEvt{
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

	evt, err := aggregates.HandleCmd(
		aggregateFetcher,
		"IssueID",
		"issue0",
		&messages.CreateInitiativeRejectedCmd{
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
		&messages.InitiativeCreatedRejectedEvt{
			IssueID:      "issue0",
			InitiativeID: "initiative0",
			Reason:       "not good folks",
		},
	)
}

func TestCreateInitiativeRejectedFailsOnDuplicate(t *testing.T) {
	fixture := append(buildBaseFixture(),
		&messages.IssueCreatedEvt{
			IssueID:      "issue0",
			InitiativeID: "initiative0",
			UnitID:       "unit0",
			RequesterID:  "member1",
			PolicyID:     "policy0",
			AreaID:       "area0",
			Body:         "Lets do A",
		},
		&messages.InitiativeCreatedRejectedEvt{
			IssueID:      "issue0",
			InitiativeID: "initiative0",
		},
	)
	aggregateFetcher := BuildTestAggregateFetcher(fixture)

	_, err := aggregates.HandleCmd(
		aggregateFetcher,
		"IssueID",
		"issue0",
		&messages.CreateInitiativeRejectedCmd{
			IssueID:      "issue0",
			InitiativeID: "initiative0",
		},
	)
	assertRejected(t, err)
}

func TestCreateInitiativeAccepted(t *testing.T) {
	fixture := append(buildBaseFixture(),
		&messages.IssueCreatedEvt{
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

	evt, err := aggregates.HandleCmd(
		aggregateFetcher,
		"IssueID",
		"issue0",
		&messages.CreateInitiativeAcceptedCmd{
			IssueID:      "issue0",
			InitiativeID: "initiative0",
		},
	)
	assert.ErrorIsNil(t, err)
	assert.MessageEquals(
		t,
		"expected event",
		evt,
		&messages.InitiativeCreatedAcceptedEvt{
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
		&messages.IssueCreatedEvt{
			IssueID:      "issue0",
			InitiativeID: "initiative0",
			UnitID:       "unit0",
			RequesterID:  "member1",
			PolicyID:     "policy0",
			AreaID:       "area0",
			Body:         "Lets do A",
		},
		&messages.InitiativeCreatedAcceptedEvt{
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

	_, err := aggregates.HandleCmd(
		aggregateFetcher,
		"IssueID",
		"issue0",
		&messages.CreateInitiativeAcceptedCmd{
			IssueID:      "issue0",
			InitiativeID: "initiative0",
		},
	)
	assertRejected(t, err)
}

func TestCreateInitiative(t *testing.T) {
	fixture := append(buildBaseFixture(),
		&messages.IssueCreatedEvt{
			IssueID:      "issue0",
			InitiativeID: "initiative0",
			UnitID:       "unit0",
			RequesterID:  "member1",
			PolicyID:     "policy0",
			AreaID:       "area0",
			Body:         "Lets do A",
		},
		&messages.InitiativeCreatedAcceptedEvt{
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
	evt, err := aggregates.HandleCmd(
		aggregateFetcher,
		"IssueID",
		"issue0",
		&messages.CreateInitiativeRequestedCmd{
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
		&messages.InitiativeCreatedRequestedEvt{
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
		&messages.IssueCreatedEvt{
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
	_, err := aggregates.HandleCmd(
		aggregateFetcher,
		"IssueID",
		"issue0",
		&messages.CreateInitiativeRequestedCmd{
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
		&messages.IssueCreatedEvt{
			IssueID:      "issue0",
			InitiativeID: "initiative0",
			UnitID:       "unit0",
			RequesterID:  "member1",
			PolicyID:     "policy0",
			AreaID:       "area0",
			Body:         "Lets do A",
		},
		&messages.InitiativeCreatedAcceptedEvt{
			IssueID:      "issue0",
			InitiativeID: "initiative0",
			UnitID:       "unit0",
			RequesterID:  "member1",
			PolicyID:     "policy0",
			AreaID:       "area0",
			Body:         "Lets do A",
		},
		&messages.InitiativeCreatedRequestedEvt{
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
	_, err := aggregates.HandleCmd(
		aggregateFetcher,
		"IssueID",
		"issue0",
		&messages.CreateInitiativeRequestedCmd{
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
		&messages.UnitCreatedEvt{
			UnitID:      "unit0",
			RequesterID: "member0",
		},
		&messages.PrivilegeGrantedEvt{
			UnitID:          "unit0",
			RequesterID:     "member0",
			MemberID:        "member1",
			VotingRight:     true,
			InitiativeRight: true,
			ManagementRight: false,
			Weight:          1,
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
	}
}
