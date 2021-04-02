package docsvc

import (
	_ "embed"
	"encoding/json"
	"io"
	"net/http"

	"github.com/ddouglascarr/rooset/messages"
	"github.com/lestrrat-go/libxml2/xsd"
	"github.com/pkg/errors"
)

// TODO: move stuff out of handlers
// TODO: proper server
// TODO: work out error handling

//go:embed roosetdoc.xsd
var docSchemaBytes []byte

//go:embed roosetdesc.xsd
var descSchemaBytes []byte

//Run starts a server on 8080
func Run() {
	docSchema, err := xsd.Parse(docSchemaBytes)
	if err != nil {
		panic(err)
	}
	defer docSchema.Free()

	descSchema, err := xsd.Parse(descSchemaBytes)
	if err != nil {
		panic(err)
	}
	defer descSchema.Free()

	http.HandleFunc("/rpc/docsvc.CreateRev", func(
		w http.ResponseWriter,
		r *http.Request,
	) {
		var claims = messages.CreateRevReqClaims{}
		var body = messages.CreateRevReqBody{}

		setupResponse(w, r)
		if r.Method == "OPTIONS" {
			return
		}

		err := parseAndValidateClaims(r, &claims, "docsvc.CreateRev")
		if err != nil {
			handleHTTPDocSvcError(w, err)
			return
		}

		err = json.NewDecoder(r.Body).Decode(&body)
		if err != nil {
			handleHTTPDocSvcError(w, &docSvcErr{badReq, "invalid payload"})
			return
		}

		resp, err := createRev(docSchema, &messages.CreateRevReq{
			Content: body.Content,
			BaseSHA: claims.BaseSHA,
			UserID:  claims.UserID,
		})
		if err != nil {
			handleHTTPDocSvcError(w, err)
			return
		}

		respBody, err := json.Marshal(resp)
		if err != nil {
			handleHTTPDocSvcError(w, errors.Wrap(
				err, "docsvc: failed to marshall response"))
			return
		}
		w.WriteHeader(http.StatusOK)
		io.WriteString(w, string(respBody))
	})

	http.HandleFunc("/rpc/docsvc.CreateDesc", func(
		w http.ResponseWriter,
		r *http.Request,
	) {
		var claims = messages.CreateDescReqClaims{}
		var body = messages.CreateDescReqBody{}

		setupResponse(w, r)
		if r.Method == "OPTIONS" {
			return
		}

		err := parseAndValidateClaims(r, &claims, "docsvc.CreateDesc")
		if err != nil {
			handleHTTPDocSvcError(w, err)
			return
		}

		err = json.NewDecoder(r.Body).Decode(&body)
		if err != nil {
			handleHTTPDocSvcError(w, &docSvcErr{badReq, "invalid payload"})
			return
		}

		resp, err := createDesc(descSchema, &messages.CreateDescReq{
			Content: body.Content,
		})
		if err != nil {
			handleHTTPDocSvcError(w, err)
			return
		}

		respBody, err := json.Marshal(resp)
		if err != nil {
			handleHTTPDocSvcError(w, errors.Wrap(
				err, "docsvc: failed to marshall response"))
			return
		}
		w.WriteHeader(http.StatusOK)
		io.WriteString(w, string(respBody))
	})

	http.HandleFunc("/rpc/docsvc.Get", func(
		w http.ResponseWriter,
		r *http.Request,
	) {
		var claims = messages.GetReqClaims{}
		var body = messages.GetReqBody{}

		setupResponse(w, r)
		if r.Method == "OPTIONS" {
			return
		}

		w.Header().Set("Content-Type", "application/json")

		err := parseAndValidateClaims(r, &claims, "docsvc.Get")
		if err != nil {
			handleHTTPDocSvcError(w, err)
			return
		}

		err = json.NewDecoder(r.Body).Decode(&body)
		if err != nil {
			handleHTTPDocSvcError(w, &docSvcErr{badReq, "invalid payload"})
			return
		}

		blob, err := getBlob(body.SHA)
		if err != nil {
			handleHTTPDocSvcError(w, errors.Wrap(err, "docsvc: failed to fetch doc"))
			return
		}

		respBody, err := json.Marshal(&messages.GetResp{
			SHA:     body.SHA,
			Content: string(blob),
		})
		if err != nil {
			handleHTTPDocSvcError(w, errors.Wrap(err, "docsvc: failed to marshall response"))
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
