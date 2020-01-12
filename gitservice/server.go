package gitservice

import (
	"fmt"
	"io"
	"net/http"

	"github.com/ddouglascarr/rooset/gitlab"
	"github.com/ddouglascarr/rooset/messages"
	"github.com/gogo/protobuf/jsonpb"
	"github.com/pkg/errors"
)

//Run starts a server on 8080
func Run() {
	http.HandleFunc("/new-initiative", ValidatedJWT(func(
		res http.ResponseWriter,
		req *http.Request,
		claims *Claims,
	) {
		var body messages.NewInitiativeReq
		err := jsonpb.Unmarshal(req.Body, &body)
		if err != nil {
			res.WriteHeader(http.StatusBadRequest)
			io.WriteString(res, fmt.Sprintf(`{"errors": ["%s"]}`,
				errors.Wrap(err, "rooset: invalid request body").Error()))
			return
		}

		CommitRecord, err := createInitiative(
			claims.RepositoryName,
			claims.AreaID,
			body.Content,
		)
		if err != nil {
			res.WriteHeader(http.StatusInternalServerError)
			io.WriteString(res, fmt.Sprintf(`{"errors": ["%s"]}`,
				errors.Wrap(err, "rooset: git commit failed").Error()))
			return
		}

		tk, err := BuildCommitRecordTk(CommitRecord.SHA, CommitRecord.BranchName)
		if err != nil {
			res.WriteHeader(http.StatusInternalServerError)
			io.WriteString(res, fmt.Sprintf(`{"errors": ["%s"]}`,
				err.Error()))
			return
		}
		m := jsonpb.Marshaler{}
		respBody, err := m.MarshalToString(&messages.NewInitiativeResp{
			CommitRecord: CommitRecord,
			Tk:           tk,
		})
		if err != nil {
			res.WriteHeader(http.StatusInternalServerError)
			io.WriteString(res, fmt.Sprintf(`{"errors": ["%s"]}`,
				errors.Wrap(err, "rooset: failed to marshal response").Error()))
			return
		}
		res.WriteHeader(http.StatusOK)
		io.WriteString(res, string(respBody))
	}))

	http.HandleFunc("/list-area-blobs", ValidatedJWT(func(
		res http.ResponseWriter,
		req *http.Request,
		claims *Claims,
	) {
		var body messages.ListAreaBlobsReq
		err := jsonpb.Unmarshal(req.Body, &body)
		if err != nil {
			res.WriteHeader(http.StatusBadRequest)
			io.WriteString(res, fmt.Sprintf(`{"errors": ["%s"]}`,
				errors.Wrap(err, "rooset: invalid request body").Error()))
			return
		}

		blobRecords, err := listAreaFiles(claims.RepositoryName, claims.AreaID, body.SHA)
		if err != nil {
			res.WriteHeader(http.StatusInternalServerError)
			io.WriteString(res, fmt.Sprintf(`{"errors": ["%s"]}`,
				errors.Wrap(err, "rooset: git operation failed").Error()))
			return
		}

		m := jsonpb.Marshaler{}
		respBody, err := m.MarshalToString(&messages.ListAreaBlobsResp{BlobRecords: blobRecords})
		if err != nil {
			res.WriteHeader(http.StatusInternalServerError)
			io.WriteString(res, fmt.Sprintf(`{"errors": ["%s"]}`,
				errors.Wrap(err, "rooset: failed to marshal response").Error()))
			return
		}
		res.WriteHeader(http.StatusOK)
		io.WriteString(res, string(respBody))
	}))

	http.HandleFunc("/get-doc", ValidatedJWT(func(
		res http.ResponseWriter,
		req *http.Request,
		claims *Claims,
	) {
		var body messages.GetDocReq
		err := jsonpb.Unmarshal(req.Body, &body)
		if err != nil {
			res.WriteHeader(http.StatusBadRequest)
			io.WriteString(res, fmt.Sprintf(`{"errors": ["%s"]}`,
				errors.Wrap(err, "rooset: invalid request body").Error()))
			return
		}

		blob, err := gitlab.GetDoc(claims.RepositoryName, claims.AreaID, body.GitRef)
		if err != nil {
			res.WriteHeader(http.StatusInternalServerError)
			io.WriteString(res, fmt.Sprintf(`{"errors": ["%s"]}`,
				errors.Wrap(err, "rooset: git operation failed").Error()))
			return
		}

		m := jsonpb.Marshaler{}
		respBody, err := m.MarshalToString(&messages.GetDocResp{Blob: blob})
		if err != nil {
			res.WriteHeader(http.StatusInternalServerError)
			io.WriteString(res, fmt.Sprintf(`{"errors": ["%s"]}`,
				errors.Wrap(err, "rooset: failed to marshal response").Error()))
			return
		}
		res.WriteHeader(http.StatusOK)
		io.WriteString(res, respBody)
	}))

	http.HandleFunc("/get-blob", ValidatedJWT(func(
		res http.ResponseWriter,
		req *http.Request,
		claims *Claims,
	) {
		var body messages.GetBlobReq
		err := jsonpb.Unmarshal(req.Body, &body)
		if err != nil {
			res.WriteHeader(http.StatusBadRequest)
			io.WriteString(res, fmt.Sprintf(`{"errors": ["%s"]}`,
				errors.Wrap(err, "rooset: invalid request body").Error()))
			return
		}

		blob, err := getBlob(claims.RepositoryName, body.SHA)
		if err != nil {
			res.WriteHeader(http.StatusInternalServerError)
			io.WriteString(res, fmt.Sprintf(`{"errors": ["%s"]}`,
				errors.Wrap(err, "rooset: git operation failed").Error()))
			return
		}

		m := jsonpb.Marshaler{}
		respBody, err := m.MarshalToString(&messages.GetBlobResp{Blob: blob})
		if err != nil {
			res.WriteHeader(http.StatusInternalServerError)
			io.WriteString(res, fmt.Sprintf(`{"errors": ["%s"]}`,
				errors.Wrap(err, "rooset: failed to marshal response").Error()))
			return
		}
		res.WriteHeader(http.StatusOK)
		io.WriteString(res, respBody)
	}))

	http.ListenAndServe(":8080", nil)
}
