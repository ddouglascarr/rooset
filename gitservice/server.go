package gitservice

import (
	"encoding/json"
	"fmt"
	"io"
	"net/http"

	"github.com/pkg/errors"
)

//NewInitiativeReq request body for /new-initiative
type NewInitiativeReq struct {
	Actions []Action
}

//NewInitiativeResp response body for /new-initiative
type NewInitiativeResp struct {
	GitRecord GitRecord
}

//Run starts a server on 8080
func Run() {
	http.HandleFunc("/new-initiative", ValidatedJWT(func(
		res http.ResponseWriter,
		req *http.Request,
		claims *Claims,
	) {
		var body NewInitiativeReq
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
			body.Actions,
		)
		if err != nil {
			res.WriteHeader(http.StatusInternalServerError)
			io.WriteString(res, fmt.Sprintf(`{"errors": ["%s"]}`,
				errors.Wrap(err, "rooset: git commit failed").Error()))
			return
		}

		respBody, err := json.Marshal(NewInitiativeResp{GitRecord: *gitRecord})
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
