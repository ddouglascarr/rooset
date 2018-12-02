package server

import (
	"fmt"
	"go/build"
	"html/template"
	"net/http"

	"github.com/ddouglascarr/rooset/messages"
	"github.com/ddouglascarr/rooset/storage"
	"github.com/julienschmidt/httprouter"
	"golang.org/x/crypto/bcrypt"
)

var (
	signupPageTmpl *template.Template
)

func init() {
	signupPageTmpl = template.Must(template.ParseFiles(build.Default.GOPATH + "/src/github.com/ddouglascarr/rooset/server/tmpl/signup.html"))
}

type signupFields struct {
	Username string
	Password string
}

// SignupPageProps are the props for the signup page
type SignupPageProps struct {
	Fields signupFields
	Errors []string
}

func devSignupGet(w http.ResponseWriter, r *http.Request, _ httprouter.Params) {
	props := SignupPageProps{}
	err := signupPageTmpl.Execute(w, props)
	if err != nil {
		w.WriteHeader(http.StatusInternalServerError)
		fmt.Fprint(w, err)
	}
}

// TODO: double cookie CSRF protection?
func devSignupPost(w http.ResponseWriter, r *http.Request, _ httprouter.Params) {
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

// DevLoginPageProps yep
type DevLoginPageProps struct {
	Errors   []string
	Username string
}

// TODO: I think this needs double cookie CSRF?
func devLoginGet(w http.ResponseWriter, r *http.Request, _ httprouter.Params) {
	props := DevLoginPageProps{Errors: []string{}}
	renderPage(w, r, "DevLoginPage", true, &props)
}

// TODO: I think this needs double cookie CSRF?
func devLoginPost(w http.ResponseWriter, r *http.Request, _ httprouter.Params) {
	props := DevLoginPageProps{
		Username: r.FormValue("Username"),
	}

	badRequest := func(msg string) {
		props.Errors = []string{msg}
		w.WriteHeader(http.StatusBadRequest)
		renderPage(w, r, "DevLoginPage", true, &props)
	}

	if err := r.ParseForm(); err != nil {
		// TODO: error message rerender?
		fmt.Println(err)
		return
	}

	user, err := storage.FetchUser(cmdDB, r.FormValue("Username"))
	if err != nil {
		badRequest("Wrong Username or password")
		return
	}

	if err := bcrypt.CompareHashAndPassword(
		[]byte(user.Password),
		[]byte(r.FormValue("Password")),
	); err != nil {
		badRequest("Wrong Username or password")
		return
	}

	if err := beginSession(w, user); err != nil {
		props.Errors = []string{"System Error"}
		w.WriteHeader(http.StatusInternalServerError)
		renderPage(w, r, "DevLoginPage", true, &props)
		return
	}

	http.Redirect(w, r, "/welcome", http.StatusFound)
}
