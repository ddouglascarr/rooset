package aggregates

import "github.com/ddouglascarr/rooset/messages"

// HandleIssueCommand issue command handler
func HandleIssueCommand(issue *IssueAggregate, msg messages.Message) (
	[]messages.Message,
	error,
) {
	switch cmd := msg.(type) {
	case *messages.CreateIssueCommand:
		return createIssue(issue, cmd)
	case *messages.CreateInitiativeRequestedCommand:
		return createInitiative(issue, cmd)
	case *messages.CreateInitiativeRejectedCommand:
		return createInitiativeRejected(issue, cmd)
	case *messages.CreateInitiativeAcceptedCommand:
		return createInitiativeAccepted(issue, cmd)
	default:
		return []messages.Message{}, nil
	}
}

func createIssue(
	issue *IssueAggregate,
	cmd *messages.CreateIssueCommand,
) ([]messages.Message, error) {
	if err := assertIssueStatus(issue.Status, []IssueStatus{IssueUninitialized}); err != nil {
		return nil, err
	}

	if err := assertInitiativeRight(cmd.RequesterID, &issue.Unit); err != nil {
		return nil, err
	}

	if err := assertAreaPolicyAllowed(cmd.AreaID, cmd.PolicyID, &issue.Unit); err != nil {
		return nil, err
	}

	return []messages.Message{
		&messages.IssueCreatedEvent{
			IssueID:      cmd.IssueID,
			InitiativeID: cmd.InitiativeID,
			UnitID:       cmd.UnitID,
			RequesterID:  cmd.RequesterID,
			PolicyID:     cmd.PolicyID,
			AreaID:       cmd.AreaID,
			Body:         cmd.Body,
		},
	}, nil
}

func createInitiative(
	issue *IssueAggregate,
	cmd *messages.CreateInitiativeRequestedCommand,
) ([]messages.Message, error) {
	if err := assertIssueStatus(
		issue.Status,
		[]IssueStatus{IssueAdmission, IssueDiscussion, IssueVerification, IssueVoting},
	); err != nil {
		return nil, err
	}

	if err := assertInitiativeRight(cmd.RequesterID, &issue.Unit); err != nil {
		return nil, err
	}

	if err := assertInitiativeStatus(
		issue.Initiatives[cmd.InitiativeID],
		[]InitiativeStatus{InitiativeIdle}); err != nil {
		return nil, err
	}

	if _, ok := issue.Initiatives[cmd.InitiativeID]; ok {
		return nil, NewRejectionError(ImpossibleActionRectionCode, "initiative already exists")
	}

	return []messages.Message{
		&messages.InitiativeCreatedRequestedEvent{
			IssueID:      cmd.IssueID,
			InitiativeID: cmd.InitiativeID,
			UnitID:       issue.UnitID,
			RequesterID:  cmd.RequesterID,
			PolicyID:     issue.PolicyID,
			AreaID:       issue.AreaID,
			Body:         cmd.Body,
		},
	}, nil
}

func createInitiativeRejected(
	issue *IssueAggregate,
	cmd *messages.CreateInitiativeRejectedCommand,
) ([]messages.Message, error) {
	if err := assertInitiativeStatus(
		issue.Initiatives[cmd.InitiativeID],
		[]InitiativeStatus{InitiativeValidating}); err != nil {
		return nil, err
	}

	return []messages.Message{
		&messages.InitiativeCreatedRejectedEvent{
			IssueID:      cmd.IssueID,
			InitiativeID: cmd.InitiativeID,
			Reason:       cmd.Reason,
		},
	}, nil
}

func createInitiativeAccepted(
	issue *IssueAggregate,
	cmd *messages.CreateInitiativeAcceptedCommand,
) ([]messages.Message, error) {
	i := issue.Initiatives[cmd.InitiativeID]

	if err := assertInitiativeStatus(
		i,
		[]InitiativeStatus{InitiativeValidating}); err != nil {
		return nil, err
	}

	return []messages.Message{
		&messages.InitiativeCreatedAcceptedEvent{
			IssueID:      cmd.IssueID,
			InitiativeID: cmd.InitiativeID,
			UnitID:       issue.UnitID,
			PolicyID:     issue.PolicyID,
			AreaID:       issue.AreaID,
			Body:         i.Body,
		},
	}, nil
}

//
// utils
//

func assertIssueStatus(status IssueStatus, acceptable []IssueStatus) error {
	for _, s := range acceptable {
		if status == s {
			return nil
		}
	}
	return NewRejectionError(AggregateStatusRejectionCode, "aggregate is in wrong state")
}

func assertInitiativeStatus(
	initiative initiative,
	acceptable []InitiativeStatus,
) error {
	for _, s := range acceptable {
		if initiative.Status == s {
			return nil
		}
	}
	return NewRejectionError(AggregateStatusRejectionCode, "initiative is in the wrong state")
}

func assertInitiativeRight(memberID string, unit *UnitAggregate) error {
	member, ok := unit.Members[memberID]
	if !ok {
		return NewRejectionError(UnauthorizedRejectionCode, "requester is not a member of unit")
	}
	if !member.InitiativeRight {
		return NewRejectionError(UnauthorizedRejectionCode, "member does not have initiative rights")
	}
	return nil
}

func assertAreaPolicyAllowed(areaID string, policyID string, unit *UnitAggregate) error {

	area, ok := unit.Areas[areaID]
	if !ok {
		return NewRejectionError(ImpossibleActionRectionCode, "area not found")
	}

	if _, ok := area.Policies[policyID]; !ok {
		return NewRejectionError(ImpossibleActionRectionCode, "policy not allowed in area")
	}

	return nil
}
