package gitservice

import (
	"fmt"
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
		io.WriteString(res, fmt.Sprintf(`{"hello":"%s"}`, jWTPayload.RepositoryName))
	}))
	http.ListenAndServe(":8080", nil)
}
