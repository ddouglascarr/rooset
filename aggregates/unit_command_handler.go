package aggregates

import "github.com/ddouglascarr/rooset/messages"

// HandleUnitCommand is the Unit command handler
func HandleUnitCommand(unit *UnitAggregate, msg messages.Message) (messages.Message, error) {
	switch cmd := msg.(type) {
	case *messages.CreateUnitCommand:
		return createUnit(unit, cmd)
	case *messages.GrantPrivilegeCommand:
		return grantPrivilege(unit, cmd)
	case *messages.RevokePrivilegeCommand:
		return revokePrivilege(unit, cmd)
	case *messages.CreateAreaCommand:
		return createArea(unit, cmd)
	case *messages.CreatePolicyCommand:
		return createPolicy(unit, cmd)
	default:
		return nil, nil
	}
}

func createUnit(
	unit *UnitAggregate,
	cmd *messages.CreateUnitCommand,
) (messages.Message, error) {
	if err := assertStatus(unit.Status, []Status{Uninitialized}); err != nil {
		return nil, err
	}
	return &messages.UnitCreatedEvent{
		UnitID:           cmd.UnitID,
		RequesterID:      cmd.RequesterID,
		Name:             cmd.Name,
		Description:      cmd.Description,
		URLParameterName: cmd.URLParameterName,
	}, nil
}

func grantPrivilege(
	unit *UnitAggregate,
	cmd *messages.GrantPrivilegeCommand,
) (messages.Message, error) {
	if err := assertStatus(unit.Status, []Status{Ready}); err != nil {
		return nil, err
	}
	if err := assertIsManager(cmd.RequesterID, unit.Members); err != nil {
		return nil, err
	}

	_, ok := unit.Members[cmd.MemberID]
	if ok {
		return nil, NewRejectionError(ImpossibleActionRectionCode, "membership already granted")
	}

	return &messages.PrivilegeGrantedEvent{
		UnitID:          cmd.UnitID,
		RequesterID:     cmd.RequesterID,
		MemberID:        cmd.MemberID,
		VotingRight:     cmd.VotingRight,
		InitiativeRight: cmd.InitiativeRight,
		ManagementRight: cmd.ManagementRight,
		Weight:          cmd.Weight,
	}, nil
}

func revokePrivilege(
	unit *UnitAggregate,
	cmd *messages.RevokePrivilegeCommand,
) (messages.Message, error) {
	if err := assertStatus(unit.Status, []Status{Ready}); err != nil {
		return nil, err
	}
	if err := assertIsManager(cmd.RequesterID, unit.Members); err != nil {
		return nil, err
	}

	privilege, ok := unit.Members[cmd.MemberID]
	if !ok {
		return nil, NewRejectionError(ImpossibleActionRectionCode, "membership not present")
	}

	return &messages.PrivilegeRevokedEvent{
		UnitID:      cmd.UnitID,
		RequesterID: cmd.RequesterID,
		MemberID:    cmd.MemberID,
		Weight:      privilege.Weight,
	}, nil
}

func createArea(
	unit *UnitAggregate,
	cmd *messages.CreateAreaCommand,
) (messages.Message, error) {
	if err := assertStatus(unit.Status, []Status{Ready}); err != nil {
		return nil, err
	}
	if err := assertIsManager(cmd.RequesterID, unit.Members); err != nil {
		return nil, err
	}

	if _, ok := unit.Areas[cmd.AreaID]; ok == true {
		return nil, NewRejectionError(ImpossibleActionRectionCode, "area already exists")
	}

	return &messages.AreaCreatedEvent{
		UnitID:      cmd.UnitID,
		RequesterID: cmd.RequesterID,
		AreaID:      cmd.AreaID,
		Name:        cmd.Name,
		Description: cmd.Description,
	}, nil
}

func createPolicy(
	unit *UnitAggregate,
	cmd *messages.CreatePolicyCommand,
) (messages.Message, error) {
	if err := assertStatus(unit.Status, []Status{Ready}); err != nil {
		return nil, err
	}
	if err := assertIsManager(cmd.RequesterID, unit.Members); err != nil {
		return nil, err
	}

	if _, ok := unit.Policies[cmd.PolicyID]; ok {
		return nil, NewRejectionError(ImpossibleActionRectionCode, "policy already exists")
	}

	return &messages.PolicyCreatedEvent{
		UnitID:      cmd.UnitID,
		RequesterID: cmd.RequesterID,
		PolicyID:    cmd.PolicyID,
		Description: cmd.Description,

		MinAdmissionDuration: cmd.MinAdmissionDuration,
		MaxAdmissionDuration: cmd.MaxAdmissionDuration,
		DiscussionDuration:   cmd.DiscussionDuration,
		VerificationDuration: cmd.VerificationDuration,
		VotingDuration:       cmd.VotingDuration,

		IssueQuorumNum:      cmd.IssueQuorumNum,
		IssueQuroumDen:      cmd.IssueQuroumDen,
		InitiativeQuorumNum: cmd.InitiativeQuorumNum,
		InitiativeQuorumDen: cmd.InitiativeQuorumDen,
	}, nil

}

func deactivatePolicy(
	unit *UnitAggregate,
	cmd *messages.CreatePolicyCommand,
) (messages.Message, error) {
	if err := assertStatus(unit.Status, []Status{Ready}); err != nil {
		return nil, err
	}
	if err := assertIsManager(cmd.RequesterID, unit.Members); err != nil {
		return nil, err
	}

	if _, ok := unit.Policies[cmd.PolicyID]; !ok {
		return nil, NewRejectionError(ImpossibleActionRectionCode, "policy not found")
	}

	return &messages.PolicyDeactivatedEvent{
		UnitID:      cmd.UnitID,
		RequesterID: cmd.RequesterID,
		PolicyID:    cmd.PolicyID,
	}, nil
}

//
// utils
//

func assertStatus(status Status, acceptable []Status) error {
	for _, s := range acceptable {
		if status == s {
			return nil
		}
	}
	return NewRejectionError(AggregateStatusRejectionCode, "aggregate in wrong state")
}

func assertIsManager(requesterID string, members map[string]memberPrivilege) error {
	member, ok := members[requesterID]
	if !ok {
		return NewRejectionError(UnauthorizedRejectionCode, "unknown requester")
	}
	if !member.ManagementRight {
		return NewRejectionError(UnauthorizedRejectionCode, "requester is not a manager")
	}
	return nil
}
