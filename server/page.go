package server

import (
	"fmt"
	"go/build"
	"html/template"
	"net/http"
)

var (
	pageTmpl *template.Template
)

func init() {
	pageTmpl = template.Must(template.ParseFiles(build.Default.GOPATH + "/src/github.com/ddouglascarr/rooset/server/tmpl/page.html"))
}

// pageCtx is the struct to be passed to the generic page tepmlate
// - PageName indicates the typescript Page (e.g. FooPage, would cause a bundle called FooPage.js to load)
// - Props is a struct which will be marshalled into json and passed as props to the page component
//     TODO: in the case of an application/json request, this is the json payload
type pageCtx struct {
	PageName string
	Props    interface{}
}

// renderPage serves a generic html page, or json depending on request
//   - TODO: ssr & conditional javascript
//	 - TODO: json response
func renderPage(
	w http.ResponseWriter,
	r *http.Request,
	pageName string,
	enableClientJS bool, // should javascript be delivered with the page
	props interface{},
) {
	err := pageTmpl.Execute(w, pageCtx{
		PageName: pageName,
		Props:    props,
	})
	if err != nil {
		w.WriteHeader(http.StatusInternalServerError)
		fmt.Fprint(w, err)
	}
}
