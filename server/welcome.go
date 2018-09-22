package server

import (
	"fmt"
	"net/http"

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

func homepage(w http.ResponseWriter, r *http.Request, _ httprouter.Params) {
	fmt.Fprint(w, "Rooset")
}
