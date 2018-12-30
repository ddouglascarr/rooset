package aggregates

import "github.com/ddouglascarr/rooset/messages"

// HandleUnitCmd is the Unit command handler
func HandleUnitCmd(unit *UnitAggregate, msg messages.Message) ([]messages.Message, error) {
	switch cmd := msg.(type) {
	case *messages.CreateUnitCmd:
		return createUnit(unit, cmd)
	case *messages.GrantPrivilegeCmd:
		return grantPrivilege(unit, cmd)
	case *messages.RevokePrivilegeCmd:
		return revokePrivilege(unit, cmd)
	case *messages.CreateAreaCmd:
		return createArea(unit, cmd)
	case *messages.CreatePolicyCmd:
		return createPolicy(unit, cmd)
	case *messages.AllowAreaPolicyCmd:
		return allowAreaPolicy(unit, cmd)
	case *messages.DisallowAreaPolicyCmd:
		return disallowAreaPolicy(unit, cmd)
	default:
		return []messages.Message{}, nil
	}
}

func createUnit(
	unit *UnitAggregate,
	cmd *messages.CreateUnitCmd,
) ([]messages.Message, error) {
	if err := assertStatus(unit.Status, []UnitStatus{Uninitialized}); err != nil {
		return nil, err
	}
	return []messages.Message{
		&messages.UnitCreatedEvt{
			UnitID:           cmd.UnitID,
			RequesterID:      cmd.RequesterID,
			Name:             cmd.Name,
			Description:      cmd.Description,
			URLParameterName: cmd.URLParameterName,
		},
	}, nil
}

func grantPrivilege(
	unit *UnitAggregate,
	cmd *messages.GrantPrivilegeCmd,
) ([]messages.Message, error) {
	if err := assertStatus(unit.Status, []UnitStatus{Ready}); err != nil {
		return nil, err
	}
	if err := assertIsManager(cmd.RequesterID, unit.Members); err != nil {
		return nil, err
	}

	_, ok := unit.Members[cmd.MemberID]
	if ok {
		return nil, NewRejectionError(ImpossibleActionRectionCode, "membership already granted")
	}

	return []messages.Message{
		&messages.PrivilegeGrantedEvt{
			UnitID:          cmd.UnitID,
			RequesterID:     cmd.RequesterID,
			MemberID:        cmd.MemberID,
			VotingRight:     cmd.VotingRight,
			InitiativeRight: cmd.InitiativeRight,
			ManagementRight: cmd.ManagementRight,
			Weight:          cmd.Weight,
		},
	}, nil
}

func revokePrivilege(
	unit *UnitAggregate,
	cmd *messages.RevokePrivilegeCmd,
) ([]messages.Message, error) {
	if err := assertStatus(unit.Status, []UnitStatus{Ready}); err != nil {
		return nil, err
	}
	if err := assertIsManager(cmd.RequesterID, unit.Members); err != nil {
		return nil, err
	}

	privilege, ok := unit.Members[cmd.MemberID]
	if !ok {
		return nil, NewRejectionError(ImpossibleActionRectionCode, "membership not present")
	}

	return []messages.Message{
		&messages.PrivilegeRevokedEvt{
			UnitID:      cmd.UnitID,
			RequesterID: cmd.RequesterID,
			MemberID:    cmd.MemberID,
			Weight:      privilege.Weight,
		},
	}, nil
}

func createArea(
	unit *UnitAggregate,
	cmd *messages.CreateAreaCmd,
) ([]messages.Message, error) {
	if err := assertStatus(unit.Status, []UnitStatus{Ready}); err != nil {
		return nil, err
	}
	if err := assertIsManager(cmd.RequesterID, unit.Members); err != nil {
		return nil, err
	}

	if _, ok := unit.Areas[cmd.AreaID]; ok == true {
		return nil, NewRejectionError(ImpossibleActionRectionCode, "area already exists")
	}

	return []messages.Message{
		&messages.AreaCreatedEvt{
			UnitID:      cmd.UnitID,
			RequesterID: cmd.RequesterID,
			AreaID:      cmd.AreaID,
			Name:        cmd.Name,
			Description: cmd.Description,
		},
	}, nil
}

func createPolicy(
	unit *UnitAggregate,
	cmd *messages.CreatePolicyCmd,
) ([]messages.Message, error) {
	if err := assertStatus(unit.Status, []UnitStatus{Ready}); err != nil {
		return nil, err
	}
	if err := assertIsManager(cmd.RequesterID, unit.Members); err != nil {
		return nil, err
	}

	if _, ok := unit.Policies[cmd.PolicyID]; ok {
		return nil, NewRejectionError(ImpossibleActionRectionCode, "policy already exists")
	}

	return []messages.Message{
		&messages.PolicyCreatedEvt{
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
			IssueQuorumDen:      cmd.IssueQuorumDen,
			InitiativeQuorumNum: cmd.InitiativeQuorumNum,
			InitiativeQuorumDen: cmd.InitiativeQuorumDen,
		},
	}, nil

}

func deactivatePolicy(
	unit *UnitAggregate,
	cmd *messages.CreatePolicyCmd,
) ([]messages.Message, error) {
	if err := assertStatus(unit.Status, []UnitStatus{Ready}); err != nil {
		return nil, err
	}
	if err := assertIsManager(cmd.RequesterID, unit.Members); err != nil {
		return nil, err
	}

	if _, ok := unit.Policies[cmd.PolicyID]; !ok {
		return nil, NewRejectionError(ImpossibleActionRectionCode, "policy not found")
	}

	return []messages.Message{
		&messages.PolicyDeactivatedEvt{
			UnitID:      cmd.UnitID,
			RequesterID: cmd.RequesterID,
			PolicyID:    cmd.PolicyID,
		},
	}, nil
}

func allowAreaPolicy(
	unit *UnitAggregate,
	cmd *messages.AllowAreaPolicyCmd,
) ([]messages.Message, error) {
	if err := assertStatus(unit.Status, []UnitStatus{Ready}); err != nil {
		return nil, err
	}
	if err := assertIsManager(cmd.RequesterID, unit.Members); err != nil {
		return nil, err
	}
	if _, ok := unit.Policies[cmd.PolicyID]; !ok {
		return nil, NewRejectionError(ImpossibleActionRectionCode, "unit policy not found")
	}

	area, ok := unit.Areas[cmd.AreaID]
	if !ok {
		return nil, NewRejectionError(ImpossibleActionRectionCode, "area not found")
	}

	if _, ok := area.Policies[cmd.PolicyID]; ok {
		return nil, NewRejectionError(ImpossibleActionRectionCode, "area policy already allowed")
	}

	return []messages.Message{
		&messages.AreaPolicyAllowedEvt{
			UnitID:      cmd.UnitID,
			RequesterID: cmd.RequesterID,
			PolicyID:    cmd.PolicyID,
			AreaID:      cmd.AreaID,
		},
	}, nil
}

func disallowAreaPolicy(
	unit *UnitAggregate,
	cmd *messages.DisallowAreaPolicyCmd,
) ([]messages.Message, error) {
	if err := assertStatus(unit.Status, []UnitStatus{Ready}); err != nil {
		return nil, err
	}
	if err := assertIsManager(cmd.RequesterID, unit.Members); err != nil {
		return nil, err
	}
	if _, ok := unit.Policies[cmd.PolicyID]; !ok {
		return nil, NewRejectionError(ImpossibleActionRectionCode, "unit policy not found")
	}

	area, ok := unit.Areas[cmd.AreaID]
	if !ok {
		return nil, NewRejectionError(ImpossibleActionRectionCode, "area not found")
	}

	if _, ok := area.Policies[cmd.PolicyID]; !ok {
		return nil, NewRejectionError(ImpossibleActionRectionCode, "area policy already not allowed")
	}

	return []messages.Message{
		&messages.AreaPolicyDisallowedEvt{
			UnitID:      cmd.UnitID,
			RequesterID: cmd.RequesterID,
			PolicyID:    cmd.PolicyID,
			AreaID:      cmd.AreaID,
		},
	}, nil

}

//
// utils
//

func assertStatus(status UnitStatus, acceptable []UnitStatus) error {
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
