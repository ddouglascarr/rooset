package docsvc

import (
	"fmt"
	"net/http"

	"github.com/ddouglascarr/rooset/conf"
	"github.com/ddouglascarr/rooset/messages"
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
			return &docSvcErr{authErr, "more than one Authorization header"}
		}
	}

	if tokenHeader == "" {
		return &docSvcErr{authErr, "no authorization header"}
	}

	tk, err := jwt.ParseWithClaims(tokenHeader, claims, keyFunc)
	if err != nil {
		return &docSvcErr{
			authErr,
			fmt.Sprintf("parsing auth token failed, %s", err)}
	}

	if !tk.Valid {
		return &docSvcErr{authErr, "invalid auth token"}
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
		return &docSvcErr{notAuthorizedErr,
			fmt.Sprintf("auth: claims.Op does not include %s", opName)}
	}

	return nil
}

//BuildDocSHATk builds a jwt token to be passed to lffronted detailing a new
// document.
// TODO: statically type claims somehow
func BuildDocSHATk(sHA string, baseSHA string, modifiedSectionIDs []string) (messages.JWTStr, error) {
	token := jwt.NewWithClaims(jwt.SigningMethodHS256, jwt.MapClaims{
		"sha":                  sHA,
		"base_sha":             baseSHA,
		"modified_section_ids": modifiedSectionIDs,
	})

	tkStr, err := token.SignedString([]byte(conf.Auth.JWTKey))
	if err != nil {
		return "", errors.Wrap(err, "rooset: failed to sign doc SHA")
	}

	return tkStr, nil
}

// TODO: statically type claims somehow
func buildDescRespTk(sHA string) (messages.JWTStr, error) {
	token := jwt.NewWithClaims(jwt.SigningMethodHS256, jwt.MapClaims{
		"sha": sHA,
	})

	tkStr, err := token.SignedString([]byte(conf.Auth.JWTKey))
	if err != nil {
		return "", errors.Wrap(err, "rooset: failed to sign desc SHA")
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
