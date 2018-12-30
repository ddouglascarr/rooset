package github

import (
	"fmt"
	"io/ioutil"
	"net/http"
	"time"

	"github.com/ddouglascarr/rooset/conf"
	jwt "github.com/dgrijalva/jwt-go"
	"github.com/pkg/errors"
)

// NewGithubAppJWT builds a new JSON Web Token, valid for 10 minutes,
//   signed with ROOSET_GITHUB_APP_PRIVATE_KEY
func NewGithubAppJWT() (string, error) {
	var claims jwt.MapClaims
	claims = make(map[string]interface{})

	now := time.Now().UTC().Unix()
	claims["iat"] = now
	claims["exp"] = now + 10
	claims["iss"] = conf.Github.AppID

	key, err := jwt.ParseRSAPrivateKeyFromPEM([]byte(conf.Github.PrivateKey))
	if err != nil {
		return "", errors.Wrap(err, "rooset: failed to sign token")
	}

	tk := jwt.NewWithClaims(jwt.SigningMethodRS256, claims)
	tkStr, err := tk.SignedString(key)
	if err != nil {
		return "", errors.Wrap(err, "rooset: failed to sign token")
	}

	return tkStr, nil
}

// QueryApp queries the Github app endpoint. Use to verify token setup
func QueryApp() (string, error) {
	tk, err := NewGithubAppJWT()
	if err != nil {
		return "", err
	}
	client := &http.Client{
		Timeout: time.Second * 10,
	}
	req, err := http.NewRequest("GET", "https://api.github.com/app", nil)
	if err != nil {
		return "", errors.Wrap(err, "rooset: failed to GET app endpoint")
	}
	req.Header.Add("Authorization", fmt.Sprintf("Bearer %s", tk))
	req.Header.Add("Accept", "application/vnd.github.machine-man-preview+json")

	resp, err := client.Do(req)
	if err != nil {
		return "", errors.Wrap(err, "rooset: query failed")
	}

	body, err := ioutil.ReadAll(resp.Body)
	if err != nil {
		return "", errors.Wrap(err, "rooset: failed to parse response body")
	}

	return string(body), nil
}
