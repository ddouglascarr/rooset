package gitservice

import (
	"fmt"
	"io"
	"net/http"

	jwt "github.com/dgrijalva/jwt-go"
)

//Run starts a server on 8080
func Run() {
	http.HandleFunc("/doc", ValidatedJWT(func(
		res http.ResponseWriter,
		req *http.Request,
		token *jwt.Token,
	) {
		var claims NewInitiativeClaims
		if _, err := jwt.ParseWithClaims(token.Raw, &claims, keyFunc); err != nil {
			res.WriteHeader(http.StatusUnauthorized)
			io.WriteString(res, fmt.Sprintf(`{"errors: ["%s"]}`, err.Error()))
			return
		}

		res.WriteHeader(http.StatusOK)
		io.WriteString(res, fmt.Sprintf(`{"hello":"%s"}`, claims.RepositoryName))
	}))

	http.ListenAndServe(":8080", nil)
}
