package server

import (
	"encoding/json"
	"fmt"
	"io/ioutil"
	"net/http"
	"net/url"
	"time"

	"github.com/ddouglascarr/rooset/conf"
	"github.com/ddouglascarr/rooset/storage"
	"github.com/julienschmidt/httprouter"
	"github.com/pkg/errors"
)

func githubLogin(w http.ResponseWriter, r *http.Request, _ httprouter.Params) {
	props := DevLoginPageProps{}

	// first check that the user doesn't have a session already
	_, err := r.Cookie("sessiontk")
	if err != http.ErrNoCookie {
		if err == nil {
			// user has a session already. Redirect to welcome
			http.Redirect(w, r, "/welcome", http.StatusFound)
			return
		}
		// something went wrong with the session.
		// TODO: error page?
		props.Errors = []string{err.Error()}
		w.WriteHeader(http.StatusInternalServerError)
		renderPage(w, r, "DevLoginPage", true, &props)
		return
	}

	// TODO: nonce in status
	http.Redirect(
		w,
		r,
		fmt.Sprintf(
			"https://github.com/login/oauth/authorize?client_id=%s&redirect_uri=%s",
			conf.Github.ClientID,
			url.QueryEscape(conf.Github.CallbackURI),
		),
		http.StatusFound,
	)
}

func githubCallback(w http.ResponseWriter, r *http.Request, _ httprouter.Params) {
	props := DevLoginPageProps{}

	codes, ok := r.URL.Query()["code"]
	if !ok || len(codes) != 1 {
		// TODO: error page?
		props.Errors = []string{"no code"}
		w.WriteHeader(http.StatusInternalServerError)
		renderPage(w, r, "DevLoginPage", true, &props)
		return
	}
	code := codes[0]

	githubProfile, err := fetchGithubProfile(code)
	if err != nil {
		// TODO: error page?
		props.Errors = []string{err.Error()}
		w.WriteHeader(http.StatusInternalServerError)
		renderPage(w, r, "DevLoginPage", true, &props)
		return
	}

	user, err := storage.FetchOrCreateGithubUser(cmdDB, githubProfile)
	if err != nil {
		// TODO: error page?
		props.Errors = []string{err.Error()}
		w.WriteHeader(http.StatusInternalServerError)
		renderPage(w, r, "DevLoginPage", true, &props)
		return
	}

	if err := beginSession(w, user); err != nil {
		// TODO: error page?
		props.Errors = []string{err.Error()}
		w.WriteHeader(http.StatusInternalServerError)
		renderPage(w, r, "DevLoginPage", true, &props)
		return
	}

	http.Redirect(w, r, "/welcome", http.StatusFound)
}

// fetches the Github profile from github's auth api
// see: https://developer.github.com/apps/building-github-apps/identifying-and-authorizing-users-for-github-apps/
func fetchGithubProfile(code string) (storage.GithubProfile, error) {
	githubProfile := storage.GithubProfile{}

	accessTk, err := fetchGithubProfileAccessTk(code)
	if err != nil {
		return githubProfile, errors.Wrap(err, "rooset: failed to fetch Github profile")
	}

	client := &http.Client{
		Timeout: time.Second * 10,
	}
	req, err := http.NewRequest("GET", "https://api.github.com/user", nil)
	if err != nil {
		return githubProfile, errors.Wrap(err, "rooset: failed to fetch Github profile")
	}
	req.Header.Add("Authorization", fmt.Sprintf("token %s", accessTk))
	req.Header.Add("Accept", "application/json")

	resp, err := client.Do(req)
	if err != nil {
		return githubProfile, errors.Wrap(err, "rooset: failed to fetch Github profile")
	}
	defer resp.Body.Close()

	body, err := ioutil.ReadAll(resp.Body)
	if err != nil {
		return githubProfile, errors.Wrap(err, "rooset: failed to fetch Github Profile")
	}

	if err := json.Unmarshal(body, &githubProfile); err != nil {
		return githubProfile, errors.Wrap(err, "rooset: failed to fetch Github profile")
	}

	return githubProfile, nil
}

func fetchGithubProfileAccessTk(code string) (string, error) {
	resp, err := http.PostForm(
		"https://github.com/login/oauth/access_token",
		url.Values{
			"client_id":     {conf.Github.ClientID},
			"client_secret": {conf.Github.ClientSecret},
			"code":          {code},
		},
	)
	if err != nil {
		return "", errors.Wrap(err, "rooset: failed to get github profile auth token")
	}
	defer resp.Body.Close()

	body, err := ioutil.ReadAll(resp.Body)
	if err != nil {
		return "", errors.Wrap(err, "rooset: failed to get github profile auth token")
	}

	values, err := url.ParseQuery(string(body))
	if err != nil {
		return "", errors.Wrap(err, "rooset: failed to get github profile auth token")
	}

	return values.Get("access_token"), nil
}
