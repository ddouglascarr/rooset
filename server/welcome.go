package server

import (
	"net/http"

	"github.com/ddouglascarr/rooset/conf"
	"github.com/ddouglascarr/rooset/messages"
	"github.com/julienschmidt/httprouter"
)

type welcomeProps struct {
	Name string
}

func welcome(w http.ResponseWriter, r *http.Request, _ httprouter.Params, s *messages.Session) {
	renderPage(w, r, "WelcomePage", true, &welcomeProps{
		Name: s.Username,
	})
}

type homeProps struct {
	GithubClientID    string
	GithubCallbackURI string
}

func homepage(w http.ResponseWriter, r *http.Request, _ httprouter.Params) {
	renderPage(w, r, "HomePage", false, &homeProps{
		GithubClientID:    conf.Github.ClientID,
		GithubCallbackURI: conf.Github.CallbackURI,
	})
}
