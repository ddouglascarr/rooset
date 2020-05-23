package main

import (
	"encoding/json"
	"fmt"
	"os"

	"github.com/ddouglascarr/rooset/conf"
	"github.com/dgrijalva/jwt-go"
	"github.com/pkg/errors"
)

func keyFunc(token *jwt.Token) (interface{}, error) {
	// TODO: handle expiry ?
	if _, ok := token.Method.(*jwt.SigningMethodHMAC); !ok {
		return nil, fmt.Errorf("Unexpected signing method: %v", token.Header["alg"])
	}
	// TODO: get key from config
	return []byte(conf.Auth.JWTKey), nil
}

// This is just here because I couldn't be bothered
// to find a jwt library that works for lua
// TODO: find a JWT library that works for lua
func main() {
	tkStr := os.Args[1]

	token, err := jwt.Parse(tkStr, keyFunc)
	if err != nil {
		panic(errors.Wrap(err, "rooset: failed to validate token"))
	}

	result, err := json.Marshal(struct {
		Claims interface{}
	}{Claims: token.Claims})
	if err != nil {
		panic(errors.Wrap(err, "rooset: failed to marshal validated token"))
	}

	fmt.Println(string(result))
}
