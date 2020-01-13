package gitlab

import (
	"fmt"

	"github.com/ddouglascarr/rooset/messages"
)

//CreateInitiative creates a new branch based against master with updated content
// in the area doc
func CreateInitiative(req *messages.NewInitiativeReq) (*messages.CommitRecord, error) {
	return createGitlabCommit(
		req.RepositoryName,
		"master",
		genID(),
		[]gitlabAction{
			gitlabAction{
				Action:   "update",
				FilePath: fmt.Sprintf("areas/%s/%s", req.AreaID, "doc.json"),
				Content:  req.Content,
			},
		},
	)

}
