package docsvc

import (
	"fmt"
	"io"
	"net/http"

	"github.com/ddouglascarr/rooset/messages"
	"github.com/gogo/protobuf/jsonpb"
	"github.com/golang/protobuf/proto"
	"github.com/lestrrat-go/libxml2"
	"github.com/lestrrat-go/libxml2/xsd"
	"github.com/pkg/errors"
)

// bundle the schema in the binary
//go:generate go run ../cmd/includetext.go docsvc schemaStr ./roosetdoc.xsd ./roosetdoc.go

// TODO: move stuff out of handlers
// TODO: proper server
// TODO: work out error handling

//Run starts a server on 8080
func Run() {
	// try validation:
	schema, err := xsd.Parse([]byte(schemaStr))
	if err != nil {
		panic(err)
	}
	defer schema.Free()

	http.HandleFunc("/rpc/messages.CreateDocReq", withAuthenticatedMessage("messages.CreateDocReq", func(
		w http.ResponseWriter,
		r *http.Request,
		msg proto.Message,
	) {
		body, ok := msg.(*messages.CreateDocReq)
		if !ok {
			w.WriteHeader(http.StatusInternalServerError)
			io.WriteString(w, fmt.Sprintf(`{"errors": ["%s"]}`,
				"rooset: auth system error"))
			return
		}

		sHA, doc, err := processHTML(body.Content)
		if err != nil {
			w.WriteHeader(http.StatusInternalServerError)
			io.WriteString(w, fmt.Sprintf(`{"errors": ["%s"]}`,
				"rooset: invalid document"))
			return
		}

		xmlDoc, err := libxml2.ParseString(doc)
		if err != nil {
			w.WriteHeader(http.StatusInternalServerError)
			io.WriteString(w, fmt.Sprintf(`{"errors": ["%s"]}`,
				"rooset: invalid document"))
			return
		}

		if err := schema.Validate(xmlDoc); err != nil {
			w.WriteHeader(http.StatusInternalServerError)
			io.WriteString(w, fmt.Sprintf(`{"errors": ["%s"]}`,
				"rooset: invalid doc"))
			return
		}

		baseDoc, err := getBlob(body.BaseSHA)
		if err != nil {
			w.WriteHeader(http.StatusInternalServerError)
			io.WriteString(w, fmt.Sprintf(`{"errors": ["%s"]}`,
				errors.Wrap(err, "rooset: system error fetching base doc")))
			return
		}

		modifiedSectionIDs, err := DiffDocs(baseDoc, []byte(doc))
		if err != nil {
			w.WriteHeader(http.StatusInternalServerError)
			io.WriteString(w, fmt.Sprintf(`{"errors": ["%s"]}`,
				errors.Wrap(err, "rooset: system error comparing docs")))
			return
		}

		err = saveBlob(sHA, doc)
		if err != nil {
			w.WriteHeader(http.StatusInternalServerError)
			io.WriteString(w, fmt.Sprintf(`{"errors": ["%s"]}`,
				errors.Wrap(err, "rooset: system error saving doc")))
			return
		}

		tk, err := BuildDocSHATk(sHA, body.BaseSHA, modifiedSectionIDs)
		if err != nil {
			w.WriteHeader(http.StatusInternalServerError)
			io.WriteString(w, fmt.Sprintf(`{"errors": ["%s"]}`,
				err.Error()))
			return
		}
		m := jsonpb.Marshaler{}
		respBody, err := m.MarshalToString(&messages.CreateDocResp{
			SHA:                sHA,
			BaseSHA:            body.BaseSHA,
			ModifiedSectionIDs: modifiedSectionIDs,
			Tk:                 tk,
		})
		if err != nil {
			w.WriteHeader(http.StatusInternalServerError)
			io.WriteString(w, fmt.Sprintf(`{"errors": ["%s"]}`,
				errors.Wrap(err, "rooset: failed to marshal response").Error()))
			return
		}
		w.WriteHeader(http.StatusOK)
		io.WriteString(w, string(respBody))
	}))

	http.HandleFunc("/rpc/messages.GetDocReq", withAuthenticatedMessage("messages.GetDocReq", func(
		w http.ResponseWriter,
		r *http.Request,
		msg proto.Message,
	) {
		body, ok := msg.(*messages.GetDocReq)
		if !ok {
			w.WriteHeader(http.StatusInternalServerError)
			io.WriteString(w, fmt.Sprintf(`{"errors": ["%s"]}`,
				"rooset: auth system error"))
			return
		}

		blob, err := getBlob(body.SHA)
		if err != nil {
			w.WriteHeader(http.StatusInternalServerError)
			io.WriteString(w, fmt.Sprintf(`{"errors": ["%s"]}`,
				errors.Wrap(err, "rooset: system error fetching doc")))
			return
		}

		m := jsonpb.Marshaler{}
		respBody, err := m.MarshalToString(&messages.GetDocResp{
			SHA:     body.SHA,
			Content: string(blob),
		})
		if err != nil {
			w.WriteHeader(http.StatusInternalServerError)
			io.WriteString(w, fmt.Sprintf(`{"errors": ["%s"]}`,
				errors.Wrap(err, "rooset: failed to marshal response").Error()))
			return
		}

		w.WriteHeader(http.StatusOK)
		io.WriteString(w, string(respBody))
	}))

	http.ListenAndServe(":8080", nil)
}
