package aggregates

import "github.com/ddouglascarr/rooset/messages"

// UnitStatus aggregate status enum
type UnitStatus int

const (
	// Uninitialized Status Enum value
	Uninitialized = UnitStatus(0)
	// Ready Status Enum value
	Ready = UnitStatus(1)
)

type memberPrivilege struct {
	VotingRight     bool
	InitiativeRight bool
	ManagementRight bool
	Weight          uint32
}

// TODO: is this right? I was after something like a Set in Python. should I just point at the unit policy?
type area struct {
	Name        string
	Description string
	Policies    map[string]areaPolicy
}

type policy struct{}

type areaPolicy struct{}

// UnitAggregate is the aggregate root of pretty much everything
type UnitAggregate struct {
	Status   UnitStatus
	UnitID   string
	Members  map[string]memberPrivilege
	Areas    map[string]area
	Policies map[string]policy
}

// NewUnitAggregate UnitAggregate constructor
func NewUnitAggregate(aRID string) UnitAggregate {
	agg := UnitAggregate{UnitID: aRID}
	agg.Members = make(map[string]memberPrivilege)
	agg.Areas = make(map[string]area)
	agg.Policies = make(map[string]policy)
	return agg
}

// HandleEvt is the root event handler
func (unit *UnitAggregate) HandleEvt(msg messages.Message) error {
	switch evt := msg.(type) {
	case *messages.UnitCreatedEvt:
		unit.unitCreated(evt)
	case *messages.PrivilegeGrantedEvt:
		unit.privilegeGranted(evt)
	case *messages.PrivilegeRevokedEvt:
		unit.privilegeRevoked(evt)
	case *messages.AreaCreatedEvt:
		unit.areaCreated(evt)
	case *messages.PolicyCreatedEvt:
		unit.policyCreated(evt)
	case *messages.AreaPolicyAllowedEvt:
		unit.areaPolicyAllowed(evt)
	case *messages.AreaPolicyDisallowedEvt:
		unit.areaPolicyDisallowed(evt)
	}
	return nil
}

func (unit *UnitAggregate) unitCreated(evt *messages.UnitCreatedEvt) {
	unit.Status = Ready
	unit.Members[evt.RequesterID] = memberPrivilege{
		VotingRight:     false,
		InitiativeRight: false,
		ManagementRight: true,
		Weight:          1,
	}
}

func (unit *UnitAggregate) privilegeGranted(evt *messages.PrivilegeGrantedEvt) {
	unit.Members[evt.MemberID] = memberPrivilege{
		VotingRight:     evt.VotingRight,
		InitiativeRight: evt.InitiativeRight,
		ManagementRight: evt.ManagementRight,
		Weight:          evt.Weight,
	}
}

func (unit *UnitAggregate) privilegeRevoked(evt *messages.PrivilegeRevokedEvt) {
	delete(unit.Members, evt.MemberID)
}

func (unit *UnitAggregate) areaCreated(evt *messages.AreaCreatedEvt) {
	a := area{
		Name:        evt.Name,
		Description: evt.Description,
	}
	a.Policies = make(map[string]areaPolicy)
	unit.Areas[evt.AreaID] = a
}

func (unit *UnitAggregate) policyCreated(evt *messages.PolicyCreatedEvt) {
	unit.Policies[evt.PolicyID] = policy{}
}

func (unit *UnitAggregate) areaPolicyAllowed(evt *messages.AreaPolicyAllowedEvt) {
	a, ok := unit.Areas[evt.AreaID]
	if !ok {
		return
	}

	a.Policies[evt.PolicyID] = areaPolicy{}
}

func (unit *UnitAggregate) areaPolicyDisallowed(evt *messages.AreaPolicyDisallowedEvt) {
	a, ok := unit.Areas[evt.AreaID]
	if !ok {
		return
	}
	delete(a.Policies, evt.PolicyID)
}
