package aggregates

import "github.com/ddouglascarr/rooset/messages"

// Status aggregate status enum
type Status int

const (
	// Uninitialized Status Enum value
	Uninitialized = Status(0)
	// Ready Status Enum value
	Ready = Status(1)
)

type memberPrivilege struct {
	VotingRight     bool
	InitiativeRight bool
	ManagementRight bool
	Weight          uint32
}

type area struct {
	Name        string
	Description string
}

type policy struct{}

// UnitAggregate is the aggregate root of pretty much everything
type UnitAggregate struct {
	Status   Status
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

// HandleEvent is the root event handler
func (unit *UnitAggregate) HandleEvent(msg messages.Message) error {
	switch evt := msg.(type) {
	case *messages.UnitCreatedEvent:
		unit.unitCreated(evt)
	case *messages.PrivilegeGrantedEvent:
		unit.privilegeGranted(evt)
	case *messages.PrivilegeRevokedEvent:
		unit.privilegeRevoked(evt)
	case *messages.AreaCreatedEvent:
		unit.areaCreated(evt)
	case *messages.PolicyCreatedEvent:
		unit.policyCreated(evt)
	}
	return nil
}

func (unit *UnitAggregate) unitCreated(evt *messages.UnitCreatedEvent) {
	unit.Status = Ready
	unit.Members[evt.RequesterID] = memberPrivilege{
		VotingRight:     false,
		InitiativeRight: false,
		ManagementRight: true,
		Weight:          1,
	}
}

func (unit *UnitAggregate) privilegeGranted(evt *messages.PrivilegeGrantedEvent) {
	unit.Members[evt.MemberID] = memberPrivilege{
		VotingRight:     evt.VotingRight,
		InitiativeRight: evt.InitiativeRight,
		ManagementRight: evt.ManagementRight,
		Weight:          evt.Weight,
	}
}

func (unit *UnitAggregate) privilegeRevoked(evt *messages.PrivilegeRevokedEvent) {
	delete(unit.Members, evt.MemberID)
}

func (unit *UnitAggregate) areaCreated(evt *messages.AreaCreatedEvent) {
	unit.Areas[evt.AreaID] = area{
		Name:        evt.Name,
		Description: evt.Description,
	}
}

func (unit *UnitAggregate) policyCreated(evt *messages.PolicyCreatedEvent) {
	unit.Policies[evt.PolicyID] = policy{}
}
