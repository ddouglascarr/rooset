package docsvc

import (
	"crypto/sha256"
	"encoding/hex"
	"fmt"

	"github.com/lestrrat-go/libxml2"
	"github.com/lestrrat-go/libxml2/xsd"
	"github.com/microcosm-cc/bluemonday"
	"github.com/pkg/errors"
	"github.com/tdewolff/minify/v2"
	"github.com/tdewolff/minify/v2/xml"
)

var policy *bluemonday.Policy

func init() {
	policy = bluemonday.UGCPolicy()
}

type HTMLStr = string
type SHA = string

//processHTML
// - runs bluemonday basic sanitizer across document, as an extra layer of safety
// - removes any non-semantic whitespace
// - calculate a SHA256 hash of the resulting doc
// - return is sHA256, html
func processHTML(userHTML string) (SHA, HTMLStr, error) {
	// remove whitespace
	m := minify.New()
	m.AddFunc("xml", xml.Minify)
	minifiedHTML, err := m.String("xml", userHTML)
	if err != nil {
		return "", "", errors.Wrap(err, "rooset: processHTML minification failed")
	}

	// sanitize
	sanitizedHTML := policy.Sanitize(minifiedHTML)

	// calculate the SHA
	h := sha256.New()
	h.Write([]byte(sanitizedHTML))

	return hex.EncodeToString(h.Sum(nil)), sanitizedHTML, nil
}

func processAndValidate(schema *xsd.Schema, body string) (SHA, HTMLStr, error) {
	sHA, doc, err := processHTML(body)
	if err != nil {
		return sHA, doc, &docSvcErr{badReq, fmt.Sprintf("invalid xml, %s", err)}
	}

	xmlDoc, err := libxml2.ParseString(doc)
	if err != nil {
		return sHA, doc, &docSvcErr{badReq, fmt.Sprintf("invalid xml, %s", err)}
	}

	if err := schema.Validate(xmlDoc); err != nil {
		return sHA, doc, &docSvcErr{
			badReq, fmt.Sprintf("does not comply with schema, %s", err)}
	}

	return sHA, doc, nil
}
