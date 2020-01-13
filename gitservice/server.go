package gitservice

import (
	"fmt"
	"io"
	"net/http"

	"github.com/ddouglascarr/rooset/gitlab"
	"github.com/ddouglascarr/rooset/messages"
	"github.com/golang/protobuf/jsonpb"
	"github.com/golang/protobuf/proto"
	"github.com/pkg/errors"
)

//Run starts a server on 8080
func Run() {
	http.HandleFunc("/rpc/messages.NewInitiativeReq", withAuthenticatedMessage("messages.NewInitiativeReq", func(
		w http.ResponseWriter,
		r *http.Request,
		msg proto.Message,
	) {
		body, ok := msg.(*messages.NewInitiativeReq)
		if !ok {
			w.WriteHeader(http.StatusInternalServerError)
			io.WriteString(w, fmt.Sprintf(`{"errors": ["%s"]}`,
				"rooset: auth system error"))
			return
		}

		CommitRecord, err := gitlab.CreateInitiative(body)

		if err != nil {
			w.WriteHeader(http.StatusInternalServerError)
			io.WriteString(w, fmt.Sprintf(`{"errors": ["%s"]}`,
				errors.Wrap(err, "rooset: git commit failed").Error()))
			return
		}

		tk, err := BuildCommitRecordTk(CommitRecord.SHA, CommitRecord.BranchName)
		if err != nil {
			w.WriteHeader(http.StatusInternalServerError)
			io.WriteString(w, fmt.Sprintf(`{"errors": ["%s"]}`,
				err.Error()))
			return
		}
		m := jsonpb.Marshaler{}
		respBody, err := m.MarshalToString(&messages.NewInitiativeResp{
			CommitRecord: CommitRecord,
			Tk:           tk,
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

		blob, err := gitlab.GetDoc(body)
		if err != nil {
			w.WriteHeader(http.StatusInternalServerError)
			io.WriteString(w, fmt.Sprintf(`{"errors": ["%s"]}`,
				errors.Wrap(err, "rooset: git operation failed").Error()))
			return
		}

		m := jsonpb.Marshaler{}
		respBody, err := m.MarshalToString(&messages.GetDocResp{Blob: blob})
		if err != nil {
			w.WriteHeader(http.StatusInternalServerError)
			io.WriteString(w, fmt.Sprintf(`{"errors": ["%s"]}`,
				errors.Wrap(err, "rooset: failed to marshal response").Error()))
			return
		}
		w.WriteHeader(http.StatusOK)
		io.WriteString(w, respBody)
	}))
	http.ListenAndServe(":8080", nil)
}
