package conf

import (
	"os"
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

// DB contains all app configurations for the databases
var DB dBConf

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
}
