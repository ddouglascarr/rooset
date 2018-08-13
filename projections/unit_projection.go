package projections

import (
	"database/sql"

	"github.com/ddouglascarr/rooset/db/projectionproc"
	"github.com/ddouglascarr/rooset/messages"
	proto "github.com/golang/protobuf/proto"
)

func UnitProjectionEventProcessor(
	_ *sql.Tx,
	targetTx *sql.Tx,
	containers []messages.MessageContainer,
) ([]messages.MessageContainer, error) {
	out := []messages.MessageContainer{}
	eventSets := make(map[string]projEventSet)
	if err := populateEventSets(&eventSets, containers); err != nil {
		return out, err
	}
	for id, eventSet := range eventSets {
		proj := messages.UnitProjection{}
		if err := projectionproc.FetchProjection(targetTx, &proj, id); err != nil {
			return out, err
		}
		eventSet.Projection = proj
	}

	for _, eventSet := range eventSets {
		for _, container := range eventSet.EventContainers {
			UpdateUnitProjection(&eventSet.Projection, container)
		}

		projectionContainer, err := messages.WrapMessage(&eventSet.Projection)
		if err != nil {
			return out, err
		}

		out = append(out, projectionContainer)
	}
	return out, nil
}

type projEventSet struct {
	Projection      messages.UnitProjection
	EventContainers []messages.MessageContainer
}

// TODO: generate this using proj.AggregateIDField and each event theres
// a handler for
func populateEventSets(
	eventSets *map[string]projEventSet,
	containers []messages.MessageContainer,
) error {
	for _, container := range containers {
		switch container.MessageType {
		case "messages.UnitCreatedEvent":
			evt := messages.UnitCreatedEvent{}
			if err := proto.Unmarshal(container.Payload, &evt); err != nil {
				return err
			}
			evtSet := (*eventSets)[evt.UnitID]
			evtSet.EventContainers = append(evtSet.EventContainers, container)
			(*eventSets)[evt.UnitID] = evtSet
		case "messages.PrivilegeGrantedEvent":
			evt := messages.PrivilegeGrantedEvent{}
			if err := proto.Unmarshal(container.Payload, &evt); err != nil {
				return err
			}
			evtSet := (*eventSets)[evt.UnitID]
			evtSet.EventContainers = append(evtSet.EventContainers, container)
			(*eventSets)[evt.UnitID] = evtSet
		case "messages.PrivilegeRevokedEvent":
			evt := messages.PrivilegeRevokedEvent{}
			if err := proto.Unmarshal(container.Payload, &evt); err != nil {
				return err
			}
			evtSet := (*eventSets)[evt.UnitID]
			evtSet.EventContainers = append(evtSet.EventContainers, container)
			(*eventSets)[evt.UnitID] = evtSet
		}
	}
	return nil
}

// TODO: generate this using proj.AggregateIDField and each event theres
// a handler for
func UpdateUnitProjection(
	projection *messages.UnitProjection,
	evtContainer messages.MessageContainer,
) error {

	switch evtContainer.MessageType {
	case "messages.UnitCreatedEvent":
		evt := messages.UnitCreatedEvent{}
		if err := proto.Unmarshal(evtContainer.Payload, &evt); err != nil {
			return err
		}
		unitCreatedEventHandler(&evt, projection)
	case "messages.PrivilegeGrantedEvent":
		evt := messages.PrivilegeGrantedEvent{}
		if err := proto.Unmarshal(evtContainer.Payload, &evt); err != nil {
			return err
		}
		privilegeGrantedEventHandler(&evt, projection)
	case "messages.PrivilegeRevokedEvent":
		evt := messages.PrivilegeRevokedEvent{}
		if err := proto.Unmarshal(evtContainer.Payload, &evt); err != nil {
			return err
		}
		privilegeRevokedEventHandler(&evt, projection)
	default:
		return nil
	}
	return nil
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
