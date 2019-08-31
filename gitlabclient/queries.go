package gitlabclient

import (
	"log"

	"github.com/xanzy/go-gitlab"
)

// Ping checks if it works
func Ping() {
	git := gitlab.NewClient(nil, "y816jUs-hhV8sokfR_Fe")
	git.SetBaseURL("http://localhost:9080/api/v4")

	// Create new project
	p := &gitlab.CreateProjectOptions{
		Name:                 gitlab.String("My Other Project"),
		Description:          gitlab.String("Just a test project to play with"),
		MergeRequestsEnabled: gitlab.Bool(true),
		SnippetsEnabled:      gitlab.Bool(true),
		Visibility:           gitlab.Visibility(gitlab.PublicVisibility),
	}
	project, _, err := git.Projects.CreateProject(p)
	if err != nil {
		log.Fatal(err)
	}

	log.Println(project.ID)
}
