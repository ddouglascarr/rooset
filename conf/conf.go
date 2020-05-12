package conf

import "os"

type authConf struct {
	JWTKey string
}

type docsvcConf struct {
	BlobDir string
}

var (
	//Auth settings for rooset auth
	Auth authConf
	//Docsvc settings for docsvc
	Docsvc docsvcConf
)

func init() {
	Auth = authConf{
		JWTKey: os.Getenv("ROOSET_JWT_KEY"),
	}

	Docsvc = docsvcConf{
		BlobDir: "/var/lib/docsvc/blobs",
	}
}
