package docsvc

import (
	"crypto/sha256"
	"encoding/hex"

	"github.com/microcosm-cc/bluemonday"
	"github.com/pkg/errors"
	"github.com/tdewolff/minify/v2"
	"github.com/tdewolff/minify/v2/xml"
)

var policy *bluemonday.Policy

func init() {
	policy = bluemonday.UGCPolicy()
}

//processHTML
// - runs bluemonday basic sanitizer across document, as an extra layer of safety
// - removes any non-semantic whitespace
// - calculate a SHA256 hash of the resulting doc
// - return is sHA256, html
func processHTML(userHTML string) (string, string, error) {
	// remove whitespace
	m := minify.New()
	m.AddFunc("xml", xml.Minify)
	minifiedHTML, err := m.String("xml", userHTML)
	if err != nil {
		return "", "", errors.Wrap(err, "rooset: minification failed")
	}

	// sanitize
	sanitizedHTML := policy.Sanitize(minifiedHTML)

	// calculate the SHA
	h := sha256.New()
	h.Write([]byte(sanitizedHTML))

	return hex.EncodeToString(h.Sum(nil)), sanitizedHTML, nil
}
