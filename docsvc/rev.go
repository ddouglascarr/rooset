package docsvc

import (
	"github.com/ddouglascarr/rooset/messages"
	"github.com/lestrrat-go/libxml2/xsd"
)

func createRev(docSchema *xsd.Schema, req *messages.CreateRevReq) (*messages.CreateRevResp, error) {
	sHA, doc, err := processAndValidate(docSchema, req.Content)
	if err != nil {
		return nil, err
	}

	baseDoc, err := getBlob(req.BaseSHA)
	if err != nil {
		return nil, err
	}

	modifiedSectionIDs, err := DiffDocs(baseDoc, []byte(doc))
	if err != nil {
		return nil, err
	}

	err = saveBlob(sHA, doc)
	if err != nil {
		return nil, err
	}

	tk, err := BuildDocSHATk(sHA, req.BaseSHA, modifiedSectionIDs)
	if err != nil {
		return nil, err
	}

	return &messages.CreateRevResp{
		SHA:                sHA,
		BaseSHA:            req.BaseSHA,
		ModifiedSectionIDs: modifiedSectionIDs,
		Tk:                 tk,
	}, nil
}
