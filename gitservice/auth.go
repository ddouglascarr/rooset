package gitservice

import (
	"fmt"
	"io"
	"net/http"

	jwt "github.com/dgrijalva/jwt-go"
)

//JWTHandlerFunc http handler function that requires the JWT payload.
type JWTHandlerFunc func(
	w http.ResponseWriter,
	r *http.Request,
	jWTPayload JWTPayload,
)

//JWTPayload represents the payload sent by the JWT token
type JWTPayload struct {
	UnitID         int64
	RepositoryName string
}

//Valid makes it a Claims object
func (p *JWTPayload) Valid() error {
	// TODO: any validation actually required here?
	return nil
}

//ValidatedJWT closure for wrapping handlers which rely on a JWT payload
//responsds with error codes if the JWT token is invalid
func ValidatedJWT(f JWTHandlerFunc) http.HandlerFunc {
	return func(w http.ResponseWriter, r *http.Request) {
		w.Header().Set("Content-Type", "application/json")
		var claims JWTPayload

		var tokenHeader string
		for i, tk := range r.Header["Authorization"] {
			tokenHeader = tk
			if i == 1 {
				w.WriteHeader(http.StatusUnauthorized)
				io.WriteString(w, `{"errors": ["More than one Authorization header"]}`)
				return
			}
		}

		if tokenHeader == "" {
			w.WriteHeader(http.StatusUnauthorized)
			io.WriteString(w, `{"errors":["No Authorization Header"]}`)
			return
		}

		token, err := jwt.ParseWithClaims(tokenHeader, &claims, func(token *jwt.Token) (interface{}, error) {
			if _, ok := token.Method.(*jwt.SigningMethodHMAC); !ok {
				return nil, fmt.Errorf("Unexpected signing method: %v", token.Header["alg"])
			}
			// TODO: get key from config
			return []byte("development"), nil
		})
		if err != nil {
			w.WriteHeader(http.StatusUnauthorized)
			io.WriteString(w, fmt.Sprintf(`{"errors":["Invalid auth token", "%s"]}`, err))
			return
		}

		if !token.Valid {
			w.WriteHeader(http.StatusUnauthorized)
			io.WriteString(w, `{"errors":["Token not verified"]}`)
			return
		}

		f(w, r, claims)
	}
}
