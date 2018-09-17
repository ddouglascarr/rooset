package server

import (
	"fmt"
	"html/template"
	"net/http"
	"time"

	"github.com/ddouglascarr/rooset/messages"
	"github.com/ddouglascarr/rooset/storage"
	"github.com/julienschmidt/httprouter"
	"golang.org/x/crypto/bcrypt"
)

const (
	cookieTimeout = 600 * time.Second
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
			Expires: time.Now().Add(cookieTimeout),
		})
		http.SetCookie(w, &http.Cookie{
			Name:    "csrftk",
			Value:   session.CSRFTk,
			Expires: time.Now().Add(cookieTimeout),
		})

		f(w, r, p, session)
	}

}

var (
	loginPageTmpl  *template.Template
	signupPageTmpl *template.Template
)

func init() {
	loginPageTmpl = template.Must(template.ParseFiles("server/tmpl/login.html"))
	signupPageTmpl = template.Must(template.ParseFiles("server/tmpl/signup.html"))
}

type signupFields struct {
	Email    string
	Username string
	Password string
}

type signupProps struct {
	Fields signupFields
	Errors []string
}

func signupGet(w http.ResponseWriter, r *http.Request, _ httprouter.Params) {
	props := signupProps{}
	err := signupPageTmpl.Execute(w, props)
	if err != nil {
		w.WriteHeader(http.StatusInternalServerError)
		fmt.Fprint(w, err)
	}
}

// TODO: double cookie CSRF protection?
func signupPost(w http.ResponseWriter, r *http.Request, _ httprouter.Params) {
	// TODO: development only. Needs to be like way better.
	user := storage.User{ID: messages.GenID()}

	if err := r.ParseForm(); err != nil {
		// TODO: error message rerender?
		fmt.Println(err)
		return
	}

	barePwd := r.FormValue("Password")
	hashedPwd, err := bcrypt.GenerateFromPassword([]byte(barePwd), 12)
	if err != nil {
		// TODO: error message rerender?
		fmt.Println(err)
		return
	}
	if err := r.ParseForm(); err != nil {
		// TODO: error message rerender?
		fmt.Println(err)
		return
	}
	user.Password = string(hashedPwd)
	user.Email = r.FormValue("Email")
	user.Username = r.FormValue("Username")

	err = storage.PersistNewUser(cmdDB, user)
	if err != nil {
		// TODO: error message rerender?
		fmt.Println(err)
		return
	}
	if err := r.ParseForm(); err != nil {
		// TODO: error message rerender?
		fmt.Println(err)
		return
	}

	http.Redirect(w, r, "/login", http.StatusFound)
}

type loginPageProps struct {
	Errors []string
	Email  string
}

// TODO: I think this needs double cookie CSRF?
func loginGet(w http.ResponseWriter, r *http.Request, _ httprouter.Params) {
	props := loginPageProps{Errors: []string{}}
	err := loginPageTmpl.Execute(w, props)
	if err != nil {
		w.WriteHeader(http.StatusInternalServerError)
		fmt.Fprint(w, err)
	}

}

// TODO: I think this needs double cookie CSRF?
func loginPost(w http.ResponseWriter, r *http.Request, _ httprouter.Params) {
	props := loginPageProps{
		Email: r.FormValue("Email"),
	}

	badRequest := func(msg string) {
		props.Errors = []string{msg}
		w.WriteHeader(http.StatusBadRequest)
		loginPageTmpl.Execute(w, props)
	}

	if err := r.ParseForm(); err != nil {
		// TODO: error message rerender?
		fmt.Println(err)
		return
	}

	user, err := storage.FetchUser(cmdDB, r.FormValue("Email"))
	if err != nil {
		badRequest("Wrong Email or password")
		return
	}

	if err := bcrypt.CompareHashAndPassword(
		[]byte(user.Password),
		[]byte(r.FormValue("Password")),
	); err != nil {
		badRequest("Wrong Email or password")
		return
	}

	sessionTk := messages.GenID()
	// TODO: username stuff
	session := &messages.Session{
		UserID:   user.ID,
		Username: user.Username,
		CSRFTk:   messages.GenID(),
	}

	if err := storage.PersistSession(cacheDB, sessionTk, session); err != nil {
		props.Errors = []string{"System Error"}
		w.WriteHeader(http.StatusInternalServerError)
		loginPageTmpl.Execute(w, props)
		return
	}

	http.SetCookie(w, &http.Cookie{
		Name:    "sessiontk",
		Value:   sessionTk,
		Expires: time.Now().Add(120 * time.Second),
	})

	http.Redirect(w, r, "/welcome", http.StatusFound)
}
