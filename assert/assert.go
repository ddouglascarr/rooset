package assert

import (
	"fmt"
	proto "github.com/golang/protobuf/proto"
	"testing"
)

func Equals(t *testing.T, msg string, a interface{}, b interface{}) {
	if a != b {
		t.Log(msg)
		t.Log(fmt.Sprintf("%#v != %#v", a, b))
		t.Fail()
	}
}

func MessageEquals(t *testing.T, msg string, a proto.Message, b proto.Message) {
	if !proto.Equal(a, b) {
		t.Log(fmt.Sprintf("expected: \n%#v\n actual: \n%#v", a, b))
		t.Fail()
	}
}

func ErrorNotNil(t *testing.T, err error) {
	if err != nil {
		t.Log(fmt.Sprintf("Error: %s", err))
		t.FailNow()
	}
}
