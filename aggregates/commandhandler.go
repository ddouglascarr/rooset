package aggregates

import (
	"fmt"

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

// RejectionCode enum for different categories of command handler rejection
type RejectionCode int

// RejectionCode enums
const (
	UnauthorizedRejectionCode    = RejectionCode(0)
	AggregateStatusRejectionCode = RejectionCode(1)
	ImpossibleActionRectionCode  = RejectionCode(2)
)

// RejectionReason struct housing the code and description for the command handler rejection
type RejectionReason struct {
	Desc string
	Code RejectionCode
}

// RejectionError is the error type used by command handlers to reject commands
type RejectionError struct {
	Desc string
	Code RejectionCode
}

// NewRejectionError builds a RejectionError
func NewRejectionError(code RejectionCode, desc string) *RejectionError {
	return &RejectionError{Code: code, Desc: desc}
}

func (e *RejectionError) Error() string {
	return fmt.Sprintf("rooset: command rejected, %s", e.Desc)
}

// CommandRejectionReason gets the RejectionReason for a command handler rejection
func (e *RejectionError) CommandRejectionReason() *RejectionReason {
	return &RejectionReason{
		Code: e.Code,
		Desc: e.Desc,
	}
}

// GetRejectionReason checks if an error is a RejectionError and returns the
// RejectionReason if it is
func GetRejectionReason(err error) (reason *RejectionReason, ok bool) {
	if r, ok := err.(interface{ CommandRejectionReason() *RejectionReason }); ok && r != nil {
		return r.CommandRejectionReason(), true
	}
	return nil, false
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
) (messages.Message, error) {
	switch aRField {
	case "UnitID":
		unit := NewUnitAggregate(aRID)
		err := fetchAggregate(aRID, &unit)
		if err != nil {
			return nil, errors.Wrap(err, "fetch aggregate failed")
		}
		msg, err := HandleUnitCommand(&unit, cmd)
		return msg, err
	case "IssueID":
		issue := NewIssueAggregate(aRID)
		err := fetchAggregate(aRID, &issue)
		if err != nil {
			return nil, errors.Wrap(err, "fetch issue aggregate failed")
		}

		if createCmd, ok := cmd.(*messages.CreateIssueCommand); ok {
			issue.UnitID = createCmd.UnitID
		}
		issue.Unit = NewUnitAggregate(issue.UnitID)
		err = fetchAggregate(issue.UnitID, &issue.Unit)
		if err != nil {
			return nil, errors.Wrap(err, "fetch unit aggregate failed")
		}

		return HandleIssueCommand(&issue, cmd)
	}
	return nil, nil
}
