package aggregates

import (
	"github.com/ddouglascarr/rooset/messages"
	"github.com/pkg/errors"
)

// Aggregate are populated by events, and are used by the commandhandler to
// generate more events
type Aggregate interface {
	HandleEvent(messages.Message) error
}

// AggregateFetcher calls an external db to populate an aggregate using it's previous events
type AggregateFetcher func(aRID string, aggregate Aggregate) error

// RejectionReason should be returned whenever a command violates business rules
type RejectionReason *string

// NewRejectionReason creates a rejection reason
func NewRejectionReason(reason string) RejectionReason {
	return RejectionReason(&reason)
}

// HandleCommand is the root command handler for all aggregates
// It returns
// - an event message which is nil if no event was emitted
// - a failure reason, which is empty if the command was accepted
// - an error, which is not nil if there was a system problem
func HandleCommand(
	fetchAggregate AggregateFetcher,
	aRField string,
	aRID string,
	cmd messages.Message,
) (messages.Message, RejectionReason, error) {
	switch aRField {
	case "UnitID":
		unit := NewUnitAggregate(aRID)
		err := fetchAggregate(aRID, &unit)
		if err != nil {
			return nil, nil, errors.Wrap(err, "fetch aggregate failed")
		}
		msg, rejectionReason := HandleUnitCommand(&unit, cmd)
		return msg, rejectionReason, nil
	}
	return nil, nil, nil
}
