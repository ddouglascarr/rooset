package conf

import (
	"fmt"
	"os"
	"strconv"
	"time"
)

type dBConf struct {
	CmdName   string
	QueryName string
	CmdHost   string
	QueryHost string
	Port      string
	User      string
	Password  string
	CacheHost string
	CacheNum  string
}

type serverConf struct {
	Port           string
	SessionTimeout time.Duration
}

type githubConf struct {
	ClientID     string
	ClientSecret string
	AppID        int
	PrivateKey   string
	CallbackURI  string
}

// DB contains all app configurations for the databases
var (
	DB     dBConf
	Server serverConf
	Github githubConf
)

func init() {
	DB = dBConf{
		CmdName:   os.Getenv("ROOSET_CMD_DB"),
		QueryName: os.Getenv("ROOSET_QUERY_DB"),
		CmdHost:   os.Getenv("ROOSET_CMD_HOST"),
		QueryHost: os.Getenv("ROOSET_QUERY_HOST"),
		Port:      os.Getenv("PGPORT"),
		User:      os.Getenv("PGUSER"),
		Password:  os.Getenv("PGPASSWORD"),
		CacheHost: os.Getenv("ROOSET_CACHE_HOST"),
		CacheNum:  os.Getenv("ROOSET_CACHE_NUM"),
	}

	Server = serverConf{
		Port:           os.Getenv("ROOSET_SERVER_PORT"),
		SessionTimeout: time.Second * 600, // 10 minutes
	}

	appID, err := strconv.Atoi(os.Getenv("ROOSET_GITHUB_APP_ID"))
	if err != nil {
		panic(fmt.Sprintf("rooset: failed to parse ROOSET_GITHUB_APP_ID: %s", err))
	}
	Github = githubConf{
		ClientID:     os.Getenv("ROOSET_GITHUB_CLIENT_ID"),
		ClientSecret: os.Getenv("ROOSET_GITHUB_CLIENT_SECRET"),
		AppID:        appID,
		PrivateKey:   os.Getenv("ROOSET_GITHUB_APP_PRIVATE_KEY"),
		CallbackURI:  os.Getenv("ROOSET_GITHUB_CALLBACK_URI"),
	}
}
