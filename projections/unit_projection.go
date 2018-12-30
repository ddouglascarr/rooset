package projections

import (
	"database/sql"

	"github.com/ddouglascarr/rooset/messages"
	"github.com/ddouglascarr/rooset/storage"
)

// UnitProjectionEvtProcessor is a single event processor for updating unit projections
func UnitProjectionEvtProcessor(
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
		for _, msg := range eventSet.Evts {
			UpdateUnitProjection(&eventSet.Projection, msg)
		}

		out = append(out, &eventSet.Projection)
	}
	return out, nil
}

type projEventSet struct {
	Projection messages.UnitProjection
	Evts       []messages.Message
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
			evtSet.Evts = append(evtSet.Evts, msg)
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
	case *messages.UnitCreatedEvt:
		err = unitCreatedEvtHandler(evt, projection)
	case *messages.PrivilegeGrantedEvt:
		err = privilegeGrantedEvtHandler(evt, projection)
	case *messages.PrivilegeRevokedEvt:
		err = privilegeRevokedEvtHandler(evt, projection)
	}
	return err
}

func unitCreatedEvtHandler(
	evt *messages.UnitCreatedEvt,
	projection *messages.UnitProjection,
) error {
	projection.UnitID = evt.UnitID
	projection.Name = evt.Name
	projection.Description = evt.Description
	projection.URLParameterName = evt.URLParameterName
	projection.MemberCount = 0

	return nil
}

func privilegeGrantedEvtHandler(
	evt *messages.PrivilegeGrantedEvt,
	projection *messages.UnitProjection,
) error {
	projection.MemberCount = projection.MemberCount + 1
	projection.TotalWeight = projection.TotalWeight + evt.Weight

	return nil
}

func privilegeRevokedEvtHandler(
	evt *messages.PrivilegeRevokedEvt,
	projection *messages.UnitProjection,
) error {
	projection.MemberCount = projection.MemberCount - 1
	projection.TotalWeight = projection.TotalWeight - evt.Weight

	return nil
}
