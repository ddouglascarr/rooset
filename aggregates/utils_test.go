package aggregates_test

import (
	"testing"

	"github.com/ddouglascarr/rooset/aggregates"
	"github.com/ddouglascarr/rooset/messages"
)

// BuildTestAggregateFetcher given a slice of events ...
// - will only return those events which match the aggregate ID
// - this means you could have 2 aggregates with the same set of events.
func BuildTestAggregateFetcher(evts []messages.Message) aggregates.AggregateFetcher {
	return func(aRID string, aggregate aggregates.Aggregate) error {
		for _, evt := range evts {
			if cmdARID, _ := messages.GetAggregateRootID(evt); cmdARID == aRID {
				aggregate.HandleEvent(evt)
			}
		}
		return nil
	}
}

func assertRejected(t *testing.T, reason aggregates.RejectionReason) {
	if reason == nil {
		t.Log("should have rejected")
		t.FailNow()
	}
}

func assertNotRejected(t *testing.T, reason aggregates.RejectionReason) {
	if reason != nil {
		t.Log("rejected")
		t.FailNow()
	}
}
