package gitservice

import (
	"fmt"
	"io"
	"net/http"

	jwt "github.com/dgrijalva/jwt-go"
	"github.com/pkg/errors"
)

//JWTHandlerFunc http handler function that requires the JWT payload.
type JWTHandlerFunc func(
	w http.ResponseWriter,
	r *http.Request,
	claims *Claims,
)

//Operation enum of operations allowed in claim
type Operation string

const (
	//CreateInitiative create a new or competing initiative
	CreateInitiative = Operation("CreateInitiative")
	//UpdateInitiative update an initiative
	UpdateInitiative = Operation("UpdateInitiative")
)

//Valid validates Operation enum
func (o Operation) Valid() error {
	switch o {
	case CreateInitiative:
	case UpdateInitiative:
	default:
		return fmt.Errorf("Invalid Operation: %s", o)
	}
	return nil
}

//Claims represents the payload sent by the JWT token
type Claims struct {
	RepositoryName       string `json:"RepositoryName"`
	AreaID               int64
	InitiativeBranchName string
	Operation            Operation
}

//Valid makes it a Claims object
func (p *Claims) Valid() error {
	if err := p.Operation.Valid(); err != nil {
		return err
	}
	return nil
}

func keyFunc(token *jwt.Token) (interface{}, error) {
	// TODO: handle expiry ?
	if _, ok := token.Method.(*jwt.SigningMethodHMAC); !ok {
		return nil, fmt.Errorf("Unexpected signing method: %v", token.Header["alg"])
	}
	// TODO: get key from config
	return []byte("development"), nil
}

//ValidatedJWT closure for wrapping handlers which rely on a JWT payload
//responsds with error codes if the JWT token is invalid
func ValidatedJWT(f JWTHandlerFunc) http.HandlerFunc {
	return func(w http.ResponseWriter, r *http.Request) {
		w.Header().Set("Content-Type", "application/json")

		var claims Claims
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

		token, err := jwt.ParseWithClaims(tokenHeader, &claims, keyFunc)
		if err != nil {
			w.WriteHeader(http.StatusUnauthorized)
			io.WriteString(w, errors.Wrap(err, "Invalid auth token").Error())
			return
		}

		if !token.Valid {
			w.WriteHeader(http.StatusUnauthorized)
			io.WriteString(w, `{"errors":["Token not verified"]}`)
			return
		}

		f(w, r, &claims)
	}
}
