package gitservice

import (
	"io"
	"net/http"
)

type docHandler struct{}

func (h *docHandler) ServeHTTP(res http.ResponseWriter, req *http.Request) {
	if req.URL.Path != "/doc" {
		res.WriteHeader(http.StatusNotFound)
		io.WriteString(res, "unknown path")
		return
	}
	switch req.Method {
	case "POST":
		h.helloWorld(res, req)
	default:
		res.WriteHeader(http.StatusMethodNotAllowed)
		io.WriteString(res, "POST only")
	}
}

func (h *docHandler) helloWorld(res http.ResponseWriter, req *http.Request) {
	res.Header().Set("Content-Type", "application/json")
	res.WriteHeader(http.StatusOK)
	io.WriteString(res, "{\"hello\":\"world\"}")
}

//Run starts a server on 8080
func Run() {
	d := &docHandler{}
	http.ListenAndServe(":8080", d)
}
