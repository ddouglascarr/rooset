package gitservice

import (
	"encoding/json"
	"fmt"
	"io"
	"net/http"

	"github.com/ddouglascarr/rooset/messages"
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
		decoder := json.NewDecoder(req.Body)
		err := decoder.Decode(&body)
		if err != nil {
			res.WriteHeader(http.StatusBadRequest)
			io.WriteString(res, fmt.Sprintf(`{"errors": ["%s"]}`,
				errors.Wrap(err, "rooset: invalid request body").Error()))
			return
		}

		gitRecord, err := createInitiative(
			claims.RepositoryName,
			claims.AreaID,
			body.FileActions,
		)
		if err != nil {
			res.WriteHeader(http.StatusInternalServerError)
			io.WriteString(res, fmt.Sprintf(`{"errors": ["%s"]}`,
				errors.Wrap(err, "rooset: git commit failed").Error()))
			return
		}

		respBody, err := json.Marshal(messages.NewInitiativeResp{GitRecord: *gitRecord})
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
		decoder := json.NewDecoder(req.Body)
		err := decoder.Decode(&body)
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

		respBody, err := json.Marshal(messages.ListAreaBlobsResp{BlobRecords: blobRecords})
		if err != nil {
			res.WriteHeader(http.StatusInternalServerError)
			io.WriteString(res, fmt.Sprintf(`{"errors": ["%s"]}`,
				errors.Wrap(err, "rooset: failed to marshal response").Error()))
			return
		}
		res.WriteHeader(http.StatusOK)
		io.WriteString(res, string(respBody))
	}))

	http.HandleFunc("/get-blob", ValidatedJWT(func(
		res http.ResponseWriter,
		req *http.Request,
		claims *Claims,
	) {
		var body messages.GetBlobReq
		decoder := json.NewDecoder(req.Body)
		err := decoder.Decode(&body)
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

		respBody, err := json.Marshal(messages.GetBlobResp{Blob: *blob})
		if err != nil {
			res.WriteHeader(http.StatusInternalServerError)
			io.WriteString(res, fmt.Sprintf(`{"errors": ["%s"]}`,
				errors.Wrap(err, "rooset: failed to marshal response").Error()))
			return
		}
		res.WriteHeader(http.StatusOK)
		io.WriteString(res, string(respBody))
	}))

	http.ListenAndServe(":8080", nil)
}
