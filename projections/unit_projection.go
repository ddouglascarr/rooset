package projections

import (
	"database/sql"

	"github.com/ddouglascarr/rooset/messages"
	"github.com/ddouglascarr/rooset/storage"
)

// UnitProjectionEventProcessor is a single event processor for updating unit projections
func UnitProjectionEventProcessor(
	_ *sql.Tx,
	targetTx *sql.Tx,
	msgs []messages.Message,
) ([]messages.Message, error) {
	out := []messages.Message{}
	eventSets := make(map[string]projEventSet)
	if err := populateEventSets(&eventSets, msgs); err != nil {
		return out, err
	}
	for id, eventSet := range eventSets {
		proj := messages.UnitProjection{}
		if err := storage.FetchProjection(targetTx, &proj, id); err != nil {
			return out, err
		}
		eventSet.Projection = proj
	}

	for _, eventSet := range eventSets {
		for _, msg := range eventSet.Events {
			UpdateUnitProjection(&eventSet.Projection, msg)
		}

		out = append(out, &eventSet.Projection)
	}
	return out, nil
}

type projEventSet struct {
	Projection messages.UnitProjection
	Events     []messages.Message
}

type unitIDPossesor interface {
	GetUnitID() string
}

// TODO: generate this using proj.AggregateIDField and each event theres
// a handler for
func populateEventSets(
	eventSets *map[string]projEventSet,
	msgs []messages.Message,
) error {
	for _, msg := range msgs {
		evt, ok := msg.(unitIDPossesor)
		if ok {
			evtSet := (*eventSets)[evt.GetUnitID()]
			evtSet.Events = append(evtSet.Events, msg)
			(*eventSets)[evt.GetUnitID()] = evtSet
		}

	}
	return nil
}

// UpdateUnitProjection as named
// TODO: generate this using proj.AggregateIDField and each event theres
// a handler for
func UpdateUnitProjection(
	projection *messages.UnitProjection,
	msg messages.Message,
) error {
	err := error(nil)
	switch evt := msg.(type) {
	case *messages.UnitCreatedEvent:
		err = unitCreatedEventHandler(evt, projection)
	case *messages.PrivilegeGrantedEvent:
		err = privilegeGrantedEventHandler(evt, projection)
	case *messages.PrivilegeRevokedEvent:
		err = privilegeRevokedEventHandler(evt, projection)
	}
	return err
}

func unitCreatedEventHandler(
	evt *messages.UnitCreatedEvent,
	projection *messages.UnitProjection,
) error {
	projection.UnitID = evt.UnitID
	projection.Name = evt.Name
	projection.Description = evt.Description
	projection.URLParameterName = evt.URLParameterName
	projection.MemberCount = 0

	return nil
}

func privilegeGrantedEventHandler(
	evt *messages.PrivilegeGrantedEvent,
	projection *messages.UnitProjection,
) error {
	projection.MemberCount = projection.MemberCount + 1
	projection.TotalWeight = projection.TotalWeight + evt.Weight

	return nil
}

func privilegeRevokedEventHandler(
	evt *messages.PrivilegeRevokedEvent,
	projection *messages.UnitProjection,
) error {
	projection.MemberCount = projection.MemberCount - 1
	projection.TotalWeight = projection.TotalWeight - evt.Weight

	return nil
}
