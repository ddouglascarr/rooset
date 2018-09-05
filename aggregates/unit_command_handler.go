package aggregates

import "github.com/ddouglascarr/rooset/messages"

// HandleUnitCommand is the Unit command handler
func HandleUnitCommand(unit *UnitAggregate, msg messages.Message) (messages.Message, RejectionReason) {
	switch cmd := msg.(type) {
	case *messages.CreateUnitCommand:
		return createUnit(unit, cmd)
	case *messages.GrantPrivilegeCommand:
		return grantPrivilege(unit, cmd)
	case *messages.RevokePrivilegeCommand:
		return revokePrivilege(unit, cmd)
	default:
		return nil, nil
	}
}

func createUnit(
	unit *UnitAggregate,
	cmd *messages.CreateUnitCommand,
) (messages.Message, RejectionReason) {
	if rej := assertStatus(unit.Status, []Status{Uninitialized}); rej != nil {
		return nil, rej
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
) (messages.Message, RejectionReason) {
	if rej := assertStatus(unit.Status, []Status{Ready}); rej != nil {
		return nil, rej
	}

	if !isAdministrator(unit, cmd.RequesterID) {
		return nil, NewRejectionReason("requester is not an administrator")
	}

	_, ok := unit.Members[cmd.MemberID]
	if ok {
		return nil, NewRejectionReason("membership already granted")
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
) (messages.Message, RejectionReason) {
	if unit.Status != Ready {
		return nil, NewRejectionReason("unit does not exist")
	}
	if !isAdministrator(unit, cmd.RequesterID) {
		return nil, NewRejectionReason("requester is not an administrator")
	}

	privilege, ok := unit.Members[cmd.MemberID]
	if !ok {
		return nil, NewRejectionReason("membership not present")
	}

	return &messages.PrivilegeRevokedEvent{
		UnitID:      cmd.UnitID,
		RequesterID: cmd.RequesterID,
		MemberID:    cmd.MemberID,
		Weight:      privilege.Weight,
	}, nil
}

//
// utils
//

func isAdministrator(unit *UnitAggregate, requesterID string) bool {
	memberPrivilege, ok := unit.Members[requesterID]
	if !ok || !memberPrivilege.ManagementRight {
		return false
	}
	return true
}

func assertStatus(status Status, acceptable []Status) RejectionReason {
	for _, s := range acceptable {
		if status == s {
			return nil
		}
	}
	return NewRejectionReason("aggregate in wrong state")
}
