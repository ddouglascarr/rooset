package gitservice

import (
	"bytes"
	"crypto/rand"
	"encoding/base32"
	"strings"
)

//genID generates a random 128 bit base32 string
func genID() string {
	b := make([]byte, 16)
	rand.Read(b)
	buf := new(bytes.Buffer)
	encoder := base32.NewEncoder(base32.StdEncoding, buf)
	encoder.Write(b)
	return strings.ToLower(buf.String())
}
