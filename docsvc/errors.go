package docsvc

import (
	"fmt"
	"io"
	"net/http"
)

const (
	badReq           = 400
	authErr          = 401
	notAuthorizedErr = 403
	generalErr       = 500
)

type docSvcErr struct {
	Code int
	Msg  string
}

func (d *docSvcErr) Error() string {
	return fmt.Sprintf("docsvc: %d %s", d.Code, d.Msg)
}

func handleHTTPDocSvcError(w http.ResponseWriter, err error) {
	switch e := err.(type) {
	case *docSvcErr:
		w.WriteHeader(e.Code)
	default:
		w.WriteHeader(http.StatusInternalServerError)
	}
	io.WriteString(w, fmt.Sprintf(`{"errors": ["%s"]}`, err))
}
