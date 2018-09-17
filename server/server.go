package server

import (
	"database/sql"
	"fmt"
	"net/http"

	"github.com/ddouglascarr/rooset/messages"
	"github.com/gomodule/redigo/redis"
	"github.com/julienschmidt/httprouter"
)

var (
	cmdDB   *sql.DB
	cacheDB redis.Conn
)

// ListenAndServe starts the application server
func ListenAndServe() error {

	initiDBs()
	defer cmdDB.Close()
	defer cacheDB.Close()

	router := httprouter.New()
	router.GET("/", homepage)
	router.GET("/signup", signupGet)
	router.POST("/signup", signupPost)
	router.GET("/login", loginGet)
	router.POST("/login", loginPost)
	router.GET("/welcome", Authenticate(welcome))

	srv := http.Server{
		Addr:    ":12345",
		Handler: router,
	}

	if err := srv.ListenAndServe(); err != nil {
		return err
	}
	return nil
}

func initiDBs() {
	var err error

	cmdDB, err = sql.Open(
		"postgres",
		"user=postgres dbname=rooset_test_0 host=localhost sslmode=disable",
	)
	if err != nil {
		panic(err)
	}

	cacheDB, err = redis.DialURL("redis://localhost:6379/9")
	if err != nil {
		panic(err)
	}
}

func welcome(w http.ResponseWriter, r *http.Request, _ httprouter.Params, s *messages.Session) {
	fmt.Fprintf(w, "Welcome %s %s %s", s.UserID, s.Username, s.CSRFTk)
}

func homepage(w http.ResponseWriter, r *http.Request, _ httprouter.Params) {
	fmt.Fprint(w, "Rooset")
}
