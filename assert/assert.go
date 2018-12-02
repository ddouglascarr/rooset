package assert

import (
	"encoding/json"
	"fmt"
	"reflect"
	"testing"

	"github.com/ddouglascarr/rooset/messages"
	proto "github.com/golang/protobuf/proto"
)

// Equals asserts simple equality
func Equals(t *testing.T, msg string, a interface{}, b interface{}) {
	if a != b {
		t.Log(msg)
		t.Log(fmt.Sprintf("%#v != %#v", a, b))
		t.Fail()
	}
}

// MessageEquals asserts that two project messages are deeply equal
func MessageEquals(t *testing.T, msg string, a []messages.Message, b proto.Message) {
	if len(a) != 1 {
		t.Log(fmt.Sprintf("expected one message, got %d", len(a)))
		t.Fail()
	}
	if !proto.Equal(a[0], b) {
		t.Log(fmt.Sprintf("expected: \n%#v\n actual: \n%#v", b, a[0]))
		t.Fail()
	}
}

// JSONEquals asserts 2 JSON strings are deeply equal
func JSONEquals(t *testing.T, actualJSON, expectedJSON string) {
	var oActual interface{}
	var oExpected interface{}

	err := json.Unmarshal([]byte(actualJSON), &oActual)
	if err != nil {
		t.Log("Invalid actualJSON json")
		t.FailNow()
	}

	err = json.Unmarshal([]byte(expectedJSON), &oExpected)
	if err != nil {
		t.Log("Invalid expectedJSON")
		t.FailNow()
	}

	if reflect.DeepEqual(oActual, oExpected) != true {
		t.Log(fmt.Sprintf("expected:\n%#v\nactual:\n%#v\n", actualJSON, expectedJSON))
	}
}

// ErrorIsNil asserts that an error is nil
func ErrorIsNil(t *testing.T, err error) {
	if err != nil {
		t.Log(fmt.Sprintf("Error: %s", err))
		t.FailNow()
	}
}
