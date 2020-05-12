package docsvc

import (
	"fmt"
	"io/ioutil"

	"github.com/ddouglascarr/rooset/conf"
	"github.com/pkg/errors"
)

//saveBlob persists a blob
// TODO: something production worthy. S3? postgres?
func saveBlob(sHA string, doc string) error {
	return ioutil.WriteFile(
		fmt.Sprintf("%s/%s", conf.Docsvc.BlobDir, sHA),
		[]byte(doc),
		0644,
	)
}

func getBlob(sHA string) ([]byte, error) {
	blob, err := ioutil.ReadFile(
		fmt.Sprintf("%s/%s", conf.Docsvc.BlobDir, sHA),
	)
	if err != nil {
		return blob, errors.Wrap(err, "rooset: failed to get doc")
	}
	return blob, nil
}
