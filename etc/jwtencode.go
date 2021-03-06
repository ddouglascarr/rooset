package main

import (
	"encoding/json"
	"fmt"
	"os"

	"github.com/ddouglascarr/rooset/conf"
	"github.com/dgrijalva/jwt-go"
	"github.com/pkg/errors"
)

// quick and dirty jwt encoder for lffrontend to use until I work
// out how to call the libjwt from lua
// TOOD: work out how to call libjwt from lua
func main() {
	claimsStr := os.Args[1]
	var claimsMap jwt.MapClaims

	json.Unmarshal([]byte(claimsStr), &claimsMap)
	token := jwt.NewWithClaims(jwt.SigningMethodHS256, claimsMap)
	tkStr, err := token.SignedString([]byte(conf.Auth.JWTKey))
	if err != nil {
		panic(errors.Wrap(err, "rooset: failed to sign doc SHA"))
	}

	fmt.Println(tkStr)
}
