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

// UnitAggregate is the aggregate root of pretty much everything
type UnitAggregate struct {
	Status  Status
	UnitID  string
	Members map[string]memberPrivilege
}

// NewUnitAggregate UnitAggregate constructor
func NewUnitAggregate(aRID string) UnitAggregate {
	agg := UnitAggregate{UnitID: aRID}
	agg.Members = make(map[string]memberPrivilege)
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
