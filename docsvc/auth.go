package docsvc

import (
	"fmt"
	"net/http"

	"github.com/ddouglascarr/rooset/conf"
	jwt "github.com/dgrijalva/jwt-go"
	"github.com/pkg/errors"
)

func keyFunc(token *jwt.Token) (interface{}, error) {
	// TODO: handle expiry ?
	if _, ok := token.Method.(*jwt.SigningMethodHMAC); !ok {
		return nil, fmt.Errorf("Unexpected signing method: %v", token.Header["alg"])
	}
	// TODO: get key from config
	return []byte("development"), nil
}

type Claims interface {
	Valid() error
	Op() []string
}

func parseAndValidateClaims(
	r *http.Request,
	claims Claims,
	opName string,
) error {
	var tokenHeader string
	for i, tk := range r.Header["Authorization"] {
		tokenHeader = tk
		if i == 1 {
			return errors.New("auth: more than one Authorization header")
		}
	}

	if tokenHeader == "" {
		return errors.New("auth: no authorization header")
	}

	tk, err := jwt.ParseWithClaims(tokenHeader, claims, keyFunc)
	if err != nil {
		return errors.Wrap(err, "auth: parsing token failed")
	}

	if !tk.Valid {
		return errors.New("auth: invalid token")
	}

	err = checkOpClaim(claims, opName)
	if err != nil {
		return err
	}

	return nil
}

func checkOpClaim(claims Claims, opName string) error {
	isAuthed := isStringInSlice(claims.Op(), opName)
	if !isAuthed {
		return errors.Errorf("auth: claims.Op does not include %s", opName)
	}

	return nil
}

//BuildDocSHATk builds a jwt token to be passed to lffronted detailing a new
// document.
// TODO: statically type claims somehow
func BuildDocSHATk(sHA string, baseSHA string, modifiedSectionIDs []string) (string, error) {
	token := jwt.NewWithClaims(jwt.SigningMethodHS256, jwt.MapClaims{
		"SHA":                sHA,
		"BaseSHA":            baseSHA,
		"ModifiedSectionIDs": modifiedSectionIDs,
	})

	tkStr, err := token.SignedString([]byte(conf.Auth.JWTKey))
	if err != nil {
		return "", errors.Wrap(err, "rooset: failed to sign doc SHA")
	}

	return tkStr, nil
}

func isStringInSlice(s []string, v string) bool {
	for _, s := range s {
		if s == v {
			return true
		}
	}
	return false
}
