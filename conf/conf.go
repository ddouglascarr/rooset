package conf

import "os"

type gitlabConf struct {
	Host        string
	AccountName string
	BearerTk    string
}

type authConf struct {
	JWTKey string
}

var (
	//Gitlab contains gitlab settings
	Gitlab gitlabConf
	//Auth settings for rooset auth
	Auth authConf
)

func init() {
	Gitlab = gitlabConf{
		Host:        os.Getenv("ROOSET_GITLAB_HOST_INTERNAL"),
		AccountName: os.Getenv("ROOSET_GITLAB_ACCOUNT"),
		BearerTk:    os.Getenv("ROOSET_GITLAB_BEARER_TK"),
	}

	Auth = authConf{
		JWTKey: os.Getenv("ROOSET_JWT_KEY"),
	}
}
