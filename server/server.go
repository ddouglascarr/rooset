package server

import (
	"database/sql"
	"fmt"
	"net/http"

	"github.com/gomodule/redigo/redis"
	"github.com/julienschmidt/httprouter"

	"github.com/ddouglascarr/rooset/conf"
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

	router.ServeFiles("/dist/*filepath", http.Dir("pages/dist"))
	router.ServeFiles("/static/*filepath", http.Dir("pages/static"))

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
		fmt.Sprintf(
			"user=%s dbname=%s host=%s sslmode=disable",
			conf.DB.User, conf.DB.CmdName, conf.DB.CmdHost,
		),
	)
	if err != nil {
		panic(err)
	}

	cacheDB, err = redis.DialURL(
		fmt.Sprintf(
			"redis://%s:6379/%s",
			conf.DB.CacheHost, conf.DB.CacheNum,
		),
	)
	if err != nil {
		panic(err)
	}
}
