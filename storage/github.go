package storage

import (
	"database/sql"

	"github.com/ddouglascarr/rooset/messages"
	"github.com/pkg/errors"
	"golang.org/x/crypto/bcrypt"
)

// GithubProfile is the data sent from the Github user endpoint
type GithubProfile struct {
	ID        string `json:"node_id"` // the GraphQL node id, (e.g. "MDQ6VXNlcjU4MzIzMQ==")
	Login     string `json:"login"`   // username (e.g. "ddouglascarr")
	Name      string `json:"name"`    // display name (e.g. "Daniel Carr")
	AvatarURL string `json:"avatar_url"`
}

// GithubRepoInstallation represents a single github repo that has Rooset installed on it
type GithubRepoInstallation struct {
	GithubID       string
	UnitID         string
	InstallationID int64
	Owner          string
	Name           string
}

// FetchOrCreateGithubUser gets a Rooset user for the given Github profile.
// - If that user hasn't been created before, it is done now.
// - TODO: The Rooset User is updated to reflect the Github profile (e.g. name, email)
func FetchOrCreateGithubUser(cmdDB *sql.DB, githubProfile GithubProfile) (User, error) {
	user := User{
		GithubID: githubProfile.ID,
	}

	tx, err := cmdDB.Begin()
	if err != nil {
		return user, errors.Wrap(err, "rooset: failed to create new Github user")
	}
	defer tx.Rollback()

	stmt, err := tx.Prepare(`
		SELECT id, username
		FROM users
		WHERE github_id = $1;
	`)
	if err != nil {
		return user, errors.Wrap(err, "rooset: failed to create new Github user")
	}
	defer stmt.Close()

	err = stmt.QueryRow(user.GithubID).Scan(&user.ID, &user.Username)
	if err != nil {
		if err == sql.ErrNoRows {
			stmt.Close()
			tx.Rollback()
			return persistNewGithubUser(cmdDB, githubProfile)
		}
		return user, errors.Wrap(err, "rooset: failed to create new Github user")
	}

	return user, nil
}

func persistNewGithubUser(cmdDB *sql.DB, githubProfile GithubProfile) (User, error) {

	user := User{
		ID:       messages.GenID(),
		Username: githubProfile.Login,
		GithubID: githubProfile.ID,
	}

	// Create a random hashed password. If the users wants to use this, they'll have to reset
	hashedPwd, err := bcrypt.GenerateFromPassword([]byte(messages.GenID()), 12)
	if err != nil {
		return user, errors.Wrap(err, "rooset: failed to create new Github user")
	}
	user.Password = string(hashedPwd)

	if err := PersistNewUser(cmdDB, user); err != nil {
		return user, errors.Wrap(err, "rooset: failed to create new Github user")
	}

	return user, nil
}

// PersistGithubRepoInstallation yep
func PersistGithubRepoInstallation(cmdDB *sql.DB, installation GithubRepoInstallation) error {
	tx, err := cmdDB.Begin()
	if err != nil {
		return errors.Wrap(err, "rooset: failed to create new github repo installation")
	}
	defer tx.Rollback()

	stmt, err := tx.Prepare(`
		INSERT INTO github_repo_unit
		(github_id, unit_id, installation_id, owner, name) VALUES (
			$1, $2, $3, $4, $5
		);
	`)
	if err != nil {
		return errors.Wrap(err, "rooset: failed to create new github repo installation")
	}
	defer stmt.Close()

	_, err = stmt.Exec(
		installation.GithubID,
		installation.UnitID,
		installation.InstallationID,
		installation.Owner,
		installation.Name,
	)
	if err != nil {
		return errors.Wrap(err, "rooset: failed to create new github repo installation")
	}

	return nil
}

// FetchGithubRepoInstallation yep
func FetchGithubRepoInstallation(
	cmdDB *sql.DB,
	githubID string,
) (GithubRepoInstallation, error) {
	installation := GithubRepoInstallation{}
	tx, err := cmdDB.Begin()
	if err != nil {
		return installation, errors.Wrap(
			err,
			"rooset: failed to create new github repo installation",
		)
	}
	defer tx.Rollback()

	stmt, err := tx.Prepare(`
		SELECT github_id, unit_id, installation_id, owner, name
		FROM github_repo_unit
		WHERE github_id = $1;
	`)
	if err != nil {
		return installation, errors.Wrap(
			err,
			"rooset: failed to create new github repo installation",
		)
	}
	defer stmt.Close()

	err = stmt.QueryRow(githubID).Scan(
		&installation.GithubID,
		&installation.UnitID,
		&installation.InstallationID,
		&installation.Owner,
		&installation.Name,
	)
	if err != nil {
		return installation, errors.Wrap(
			err,
			"rooset: failed to create new github repo installation",
		)
	}

	return installation, nil
}