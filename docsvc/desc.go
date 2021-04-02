package docsvc

import (
	"github.com/ddouglascarr/rooset/messages"
	"github.com/lestrrat-go/libxml2/xsd"
)

func createDesc(
	revSchema *xsd.Schema,
	req *messages.CreateDescReq,
) (*messages.CreateDescResp, error) {
	sHA, desc, err := processAndValidate(revSchema, req.Content)
	if err != nil {
		return nil, err
	}

	err = saveBlob(sHA, desc)
	if err != nil {
		return nil, err
	}

	tk, err := buildDescRespTk(sHA)
	if err != nil {
		return nil, err
	}

	return &messages.CreateDescResp{
		Tk: tk,
	}, nil
}
