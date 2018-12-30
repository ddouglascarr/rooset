package aggregates

import "github.com/ddouglascarr/rooset/messages"

// IssueStatus aggregate status enum
type IssueStatus int

// InitiativeStatus initiative status enum
type InitiativeStatus int

const (
	// IssueUninitialized enum value
	IssueUninitialized = IssueStatus(0)
	// IssueValidating enum value, this initial initiative of the issue is being asyncronously validated
	IssueValidating = IssueStatus(7)
	// IssueValidationFailed enum value, the initial initiative of the issue failed validation
	IssueValidationFailed = IssueStatus(8)
	// IssueRejected enum value, the issue was withdrawn or did not pass quorums
	IssueRejected = IssueStatus(1)
	// IssueAdmission enum value, the issue has been introduced but has not reached quorum
	IssueAdmission = IssueStatus(2)
	// IssueDiscussion enum value, issue phase
	IssueDiscussion = IssueStatus(3)
	// IssueVerification enum value, issue phase
	IssueVerification = IssueStatus(4)
	// IssueVoting enum value, issue phase
	IssueVoting = IssueStatus(5)
	// IssueAccepted enum value, an initiative from the issue has been adopted
	IssueAccepted = IssueStatus(6)

	// InitiativeIdle enum value
	InitiativeIdle = InitiativeStatus(0)
	// InitiativeValidating enum value, validating the first draft of the initiative
	InitiativeValidating = InitiativeStatus(1)
	// InitiativeRevalidating enum value, validating a subsequent draft of the initiative
	InitiativeRevalidating = InitiativeStatus(2)
	// InitiativeReady enum value
	InitiativeReady = InitiativeStatus(3)
	// InitiativeInvalid enum value
	InitiativeInvalid = InitiativeStatus(4)
)

type initiator struct{}

type initiative struct {
	InitiativeID string
	Status       InitiativeStatus
	Initiators   map[string]initiator
	Active       bool
	Body         string
}

// IssueAggregate an aggregate for Issues
type IssueAggregate struct {
	Status      IssueStatus
	IssueID     string
	UnitID      string
	Unit        UnitAggregate
	AreaID      string
	PolicyID    string
	Initiatives map[string]initiative
}

// NewIssueAggregate IssueAggregate constructor
func NewIssueAggregate(aRID string) IssueAggregate {
	agg := IssueAggregate{IssueID: aRID}
	agg.Initiatives = make(map[string]initiative)
	return agg
}

// HandleEvt is the root event handle for the Issue aggregate
func (issue *IssueAggregate) HandleEvt(msg messages.Message) error {
	switch evt := msg.(type) {
	case *messages.IssueCreatedEvt:
		issue.issueCreated(evt)
	case *messages.InitiativeCreatedRequestedEvt:
		issue.initiativeCreatedRequested(evt)
	case *messages.InitiativeCreatedRejectedEvt:
		issue.initiativeCreatedRejected(evt)
	case *messages.InitiativeCreatedAcceptedEvt:
		issue.initiativeCreatedAccepted(evt)
	}
	return nil
}

func (issue *IssueAggregate) issueCreated(evt *messages.IssueCreatedEvt) {
	issue.Status = IssueValidating
	issue.IssueID = evt.IssueID
	issue.UnitID = evt.UnitID
	issue.AreaID = evt.AreaID
	issue.PolicyID = evt.PolicyID

	i := initiative{
		InitiativeID: evt.InitiativeID,
		Status:       InitiativeValidating,
		Initiators:   make(map[string]initiator),
		Active:       true,
		Body:         evt.Body,
	}
	i.Initiators[evt.RequesterID] = initiator{}
	issue.Initiatives[evt.InitiativeID] = i
}

func (issue *IssueAggregate) initiativeCreatedRequested(
	evt *messages.InitiativeCreatedRequestedEvt,
) {
	i := initiative{
		InitiativeID: evt.InitiativeID,
		Status:       InitiativeValidating,
		Initiators:   make(map[string]initiator),
		Active:       true,
		Body:         evt.Body,
	}
	i.Initiators[evt.RequesterID] = initiator{}
	issue.Initiatives[evt.InitiativeID] = i
}

func (issue *IssueAggregate) initiativeCreatedRejected(
	evt *messages.InitiativeCreatedRejectedEvt,
) {
	i := issue.Initiatives[evt.InitiativeID]
	i.Status = InitiativeInvalid
	issue.Initiatives[evt.InitiativeID] = i

	if issue.Status == IssueValidating {
		issue.Status = IssueValidationFailed
	}
}

func (issue *IssueAggregate) initiativeCreatedAccepted(
	evt *messages.InitiativeCreatedAcceptedEvt,
) {
	i := issue.Initiatives[evt.InitiativeID]
	i.Status = InitiativeReady
	issue.Initiatives[evt.InitiativeID] = i

	if issue.Status == IssueValidating {
		issue.Status = IssueAdmission
	}
}
