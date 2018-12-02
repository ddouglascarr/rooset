package server

import (
	"fmt"
	"net/http"
	"time"

	"github.com/ddouglascarr/rooset/conf"
	"github.com/ddouglascarr/rooset/messages"
	"github.com/ddouglascarr/rooset/storage"
	"github.com/julienschmidt/httprouter"
	"github.com/pkg/errors"
)

// AuthenticatedHandlerFunc is an http.HandlerFunc with an extra Session argument
type AuthenticatedHandlerFunc func(
	w http.ResponseWriter,
	r *http.Request,
	p httprouter.Params,
	s *messages.Session,
)

// Authenticate is a higher-order-function to adapt project handlers that
// should be authenticated with the http.HandlerFunc. Any view wrapped in
// this will be only be run if the user is logged in, and they will be provided
// with a Session to use.
// TODO: also do CSRF verification of POST, PUT
func Authenticate(f AuthenticatedHandlerFunc) httprouter.Handle {
	return func(w http.ResponseWriter, r *http.Request, p httprouter.Params) {
		cookie, err := r.Cookie("sessiontk")
		if err != nil {
			if err == http.ErrNoCookie {
				// If the cookie is not set, return an unauthorized status
				http.Redirect(w, r, "/login", 302)
				return
			}
			fmt.Println(err)
			return
		}
		session, err := storage.FetchSession(cacheDB, cookie.Value)
		if err != nil {
			fmt.Print(err)
			http.Redirect(w, r, "/login", 302)
			return
		}

		http.SetCookie(w, &http.Cookie{
			Name:    "sessiontk",
			Value:   cookie.Value,
			Expires: time.Now().Add(conf.Server.SessionTimeout),
		})
		http.SetCookie(w, &http.Cookie{
			Name:    "csrftk",
			Value:   session.CSRFTk,
			Expires: time.Now().Add(conf.Server.SessionTimeout),
		})

		f(w, r, p, session)
	}
}

// beginSession creates a session object, persists it to the cache, and sets the cookie
// header
func beginSession(w http.ResponseWriter, user storage.User) error {
	sessionTk := messages.GenID()
	// TODO: username stuff
	session := &messages.Session{
		UserID:   user.ID,
		Username: user.Username,
		CSRFTk:   messages.GenID(),
	}

	if err := storage.PersistSession(cacheDB, sessionTk, session); err != nil {
		return errors.Wrap(err, "rooset: failed to begin session")
	}

	http.SetCookie(w, &http.Cookie{
		Name:    "sessiontk",
		Value:   sessionTk,
		Path:    "/",
		Expires: time.Now().Add(conf.Server.SessionTimeout),
	})

	return nil
}

// LoginPageProps props for login page. Use Errors to notify if user is unauthorized
type LoginPageProps struct {
	Errors []string
}

func loginGet(w http.ResponseWriter, r *http.Request, _ httprouter.Params) {
	props := LoginPageProps{}
	renderPage(w, r, "LoginPage", false, &props)
}
