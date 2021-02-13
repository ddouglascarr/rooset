package docsvc

import (
	"encoding/json"
	"fmt"
	"io"
	"net/http"

	"github.com/ddouglascarr/rooset/messages"
	"github.com/lestrrat-go/libxml2"
	"github.com/lestrrat-go/libxml2/xsd"
	"github.com/pkg/errors"
)

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

	http.HandleFunc("/rpc/docsvc.CreateDocReq", func(
		w http.ResponseWriter,
		r *http.Request,
	) {
		var claims = messages.CreateDocReqClaims{}
		var body = messages.CreateDocReqBody{}

		setupResponse(w, r)
		if r.Method == "OPTIONS" {
			return
		}

		err := parseAndValidateClaims(r, &claims, "docsvc.CreateDocReq")
		if err != nil {
			w.WriteHeader(http.StatusUnauthorized)
			io.WriteString(w, fmt.Sprintf(`{"errors": ["%s"]}`, err))
			return
		}

		err = json.NewDecoder(r.Body).Decode(&body)
		if err != nil {
			w.WriteHeader(http.StatusBadRequest)
			io.WriteString(w, fmt.Sprintf(`{"errors": ["%s"]}`,
				errors.Wrap(err, "docsvc: invalid payload")))
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

		baseDoc, err := getBlob(claims.BaseSHA)
		if err != nil {
			w.WriteHeader(http.StatusInternalServerError)
			io.WriteString(w, fmt.Sprintf(`{"errors": ["%s"]}`,
				errors.Wrap(err, "docsvc: system error fetching base doc")))
			return
		}

		modifiedSectionIDs, err := DiffDocs(baseDoc, []byte(doc))
		if err != nil {
			w.WriteHeader(http.StatusInternalServerError)
			io.WriteString(w, fmt.Sprintf(`{"errors": ["%s"]}`,
				errors.Wrap(err, "docsvc: system error comparing docs")))
			return
		}

		err = saveBlob(sHA, doc)
		if err != nil {
			w.WriteHeader(http.StatusInternalServerError)
			io.WriteString(w, fmt.Sprintf(`{"errors": ["%s"]}`,
				errors.Wrap(err, "docsvc: system error saving doc")))
			return
		}

		tk, err := BuildDocSHATk(sHA, claims.BaseSHA, modifiedSectionIDs)
		if err != nil {
			w.WriteHeader(http.StatusInternalServerError)
			io.WriteString(w, fmt.Sprintf(`{"errors": ["%s"]}`,
				err.Error()))
			return
		}
		respBody, err := json.Marshal(&messages.CreateDocResp{
			SHA:                sHA,
			BaseSHA:            claims.BaseSHA,
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
	})

	http.HandleFunc("/rpc/docsvc.GetDocReq", func(
		w http.ResponseWriter,
		r *http.Request,
	) {
		var claims = messages.GetDocReqClaims{}

		setupResponse(w, r)
		if r.Method == "OPTIONS" {
			return
		}

		w.Header().Set("Content-Type", "application/json")

		err := parseAndValidateClaims(r, &claims, "docsvc.CreateDocReq")
		if err != nil {
			w.WriteHeader(http.StatusUnauthorized)
			io.WriteString(w, fmt.Sprintf(`{"errors": ["%s"]}`, err))
			return
		}

		blob, err := getBlob(claims.SHA)
		if err != nil {
			w.WriteHeader(http.StatusInternalServerError)
			io.WriteString(w, fmt.Sprintf(`{"errors": ["%s"]}`,
				errors.Wrap(err, "rooset: system error fetching doc")))
			return
		}

		respBody, err := json.Marshal(&messages.GetDocResp{
			SHA:     claims.SHA,
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
	})

	http.ListenAndServe(":8080", nil)
}

// TODO: tighten this up
func setupResponse(w http.ResponseWriter, req *http.Request) {
	w.Header().Set("Access-Control-Allow-Origin", "*")
	w.Header().Set("Access-Control-Allow-Methods", "POST, GET, OPTIONS, PUT, DELETE")
	w.Header().Set("Access-Control-Allow-Headers", "Accept, Content-Type, Content-Length, Accept-Encoding, X-CSRF-Token, Authorization")
}
