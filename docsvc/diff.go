package docsvc

import (
	"encoding/xml"
	"fmt"
)

//DiffDocs parses and compares 2 rooset initiative docs, and outputs a slice of
//section ids that have changed. It does not differentiate between create,
//update, delete. It's up to the reader to work those out if required
//does not handle changing order of sections. This is currently unsupported
// TODO: fail on reordered sections
func DiffDocs(baseXML []byte, headXML []byte) ([]string, error) {

	var baseDoc, headDoc doc
	modifiedSections := make([]string, 0) // needs to be empty to marshall to []

	err := xml.Unmarshal(baseXML, &baseDoc)
	if err != nil {
		return modifiedSections, &docSvcErr{
			generalErr, fmt.Sprintf("failed to unmarshal baseXML, %s", err)}
	}

	err = xml.Unmarshal(headXML, &headDoc)
	if err != nil {
		return modifiedSections, &docSvcErr{
			generalErr, fmt.Sprintf("failed to unmarshal headXML, %s", err)}
	}

	for _, baseSection := range baseDoc.Sections {
		ok, headSection := findSectionByID(headDoc.Sections, baseSection.ID)
		if ok && baseSection.Content != headSection.Content {
			// section is there, but it's been modified
			modifiedSections = append(modifiedSections, baseSection.ID)
		}
		if !ok {
			// secion has been removed
			modifiedSections = append(modifiedSections, baseSection.ID)
		}
	}

	for _, headSection := range headDoc.Sections {
		ok, _ := findSectionByID(baseDoc.Sections, headSection.ID)
		if !ok {
			// section has been added
			modifiedSections = append(modifiedSections, headSection.ID)
		}
	}

	return modifiedSections, nil
}

func findSectionByID(sections []section, ID string) (bool, *section) {
	for i := range sections {
		if sections[i].ID == ID {
			return true, &sections[i]
		}
	}
	return false, nil
}

type doc struct {
	XMLName  xml.Name  `xml:"article"`
	Sections []section `xml:"section"`
}

type section struct {
	XMLName xml.Name `xml:"section"`
	ID      string   `xml:"id,attr"`
	Content string   `xml:",innerxml"`
}
