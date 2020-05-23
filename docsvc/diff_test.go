package docsvc

import (
	"fmt"
	"testing"
)

var origHTML string

func init() {
	_, origHTML, _ = processHTML(`<article>
		<section id="uid0">
		  <p>foo</p>
		</section>
		<section id="uid1">
		  <p>bar</p>
		</section>
	  </article>
	`)
}

func modifiedSectionsAreEqual(a []string, b []string) bool {
	if (a == nil) != (b == nil) {
		return false
	}
	if len(a) != len(b) {
		return false
	}

	for i := range a {
		if a[i] != b[i] {
			return false
		}
	}
	return true
}

func TestDiffModifySection(t *testing.T) {
	_, newHTML, _ := processHTML(`<article>
		<section id="uid0">
		  <p>foo</p>
		</section>
		<section id="uid1">
		  <p>pants</p>
		</section>
	  </article>
	`)

	modifiedSections, err := DiffDocs([]byte(origHTML), []byte(newHTML))
	if err != nil {
		t.Error(err)
		return
	}

	expectedSections := []string{"uid1"}
	if !modifiedSectionsAreEqual(modifiedSections, expectedSections) {
		t.Error(fmt.Sprintf("%s should have equaled %s", modifiedSections, expectedSections))
	}
}

func TestDiffDeleteSection(t *testing.T) {
	_, newHTML, _ := processHTML(`<article>
		<section id="uid1">
		  <p>bar</p>
		</section>
	  </article>
	`)

	modifiedSections, err := DiffDocs([]byte(origHTML), []byte(newHTML))
	if err != nil {
		t.Error(err)
		return
	}

	expectedSections := []string{"uid0"}
	if !modifiedSectionsAreEqual(modifiedSections, expectedSections) {
		t.Error(fmt.Sprintf("%s should have equaled %s", modifiedSections, expectedSections))
	}
}

func TestDiffAddSection(t *testing.T) {
	_, newHTML, _ := processHTML(`<article>
		<section id="uid0">
		  <p>foo</p>
		</section>
		<section id="uid2">
		  <p>I'm a new section</p>
		</section>
		<section id="uid1">
		  <p>bar</p>
		</section>
	  </article>
	`)

	modifiedSections, err := DiffDocs([]byte(origHTML), []byte(newHTML))
	if err != nil {
		t.Error(err)
		return
	}

	expectedSections := []string{"uid2"}
	if !modifiedSectionsAreEqual(modifiedSections, expectedSections) {
		t.Error(fmt.Sprintf("%s should have equaled %s", modifiedSections, expectedSections))
	}
}
