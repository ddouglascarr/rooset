package docsvc

import (
	"fmt"
	"testing"
)

func TestProcessHTMLHashIgnoresWhitespace(t *testing.T) {
	userHTML := `
	<article>
	  <section>
		<p>foo</p>
	  </section>
	</article>
  `
	sysHTML := `<article><section><p>foo</p></section></article>`

	SHA0, resultHTML0, err := processHTML(userHTML)
	if err != nil {
		t.Error(err)
	}
	SHA1, resultHTML1, err := processHTML(sysHTML)
	if err != nil {
		t.Error(err)
	}

	if resultHTML0 != resultHTML1 {
		t.Error(fmt.Sprintf(`
%s
== did not equal ==
%s`,
			resultHTML0, resultHTML1))
	}

	if resultHTML1 != sysHTML {
		t.Error(fmt.Sprintf(`
Unexpected transform of html
===================
%s
== did not equal ==
%s`,
			resultHTML1, sysHTML))
	}

	if SHA0 != SHA1 {
		t.Error("SHA did not match")
	}
}
