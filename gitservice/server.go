package gitservice

import (
	"io"
	"net/http"
)

//Run starts a server on 8080
func Run() {
	http.HandleFunc("/doc", ValidatedJWT(func(
		res http.ResponseWriter,
		req *http.Request,
		jWTPayload JWTPayload,
	) {
		res.WriteHeader(http.StatusOK)
		io.WriteString(res, `{"hello":"world"}`)
	}))
	http.ListenAndServe(":8080", nil)
}
