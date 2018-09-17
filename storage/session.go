package storage

import (
	"database/sql"

	"github.com/ddouglascarr/rooset/messages"
	"github.com/golang/protobuf/proto"
	"github.com/gomodule/redigo/redis"
	"github.com/pkg/errors"
)

// User model, password is always hashed
type User struct {
	ID       string
	Email    string
	Username string
	Password string
}

// PersistNewUser persists a new user to the databse table. Password must already be hashed
func PersistNewUser(cmdDB *sql.DB, user User) error {
	tx, err := cmdDB.Begin()
	if err != nil {
		return err
	}
	defer tx.Rollback()

	stmt, err := tx.Prepare(`
		INSERT INTO users (
			id, email, username, password
		) VALUES ($1, $2, $3, $4);
	`)
	if err != nil {
		return err
	}
	defer stmt.Close()

	if _, err := stmt.Exec(user.ID, user.Email, user.Username, user.Password); err != nil {
		return err
	}

	return tx.Commit()
}

// FetchUser retrieves user from the db. Password will be hashed
func FetchUser(cmdDB *sql.DB, email string) (User, error) {
	user := User{}

	tx, err := cmdDB.Begin()
	if err != nil {
		return user, err
	}
	defer tx.Rollback()

	stmt, err := tx.Prepare(`
		SELECT id, email, username, password
		FROM users
		WHERE email = $1
	`)
	if err != nil {
		return user, err
	}
	defer stmt.Close()

	err = stmt.QueryRow(email).Scan(&user.ID, &user.Email, &user.Username, &user.Password)
	if err != nil {
		return user, err
	}

	return user, nil
}

// SessionTimeout is the timeout a user session, in seconds.
const SessionTimeout string = "600"

// PersistSession persists cookies to the cache
func PersistSession(cacheDB redis.Conn, sessionTk string, session *messages.Session) error {
	bSession, err := proto.Marshal(session)
	if err != nil {
		return errors.Wrap(err, "rooset: failed to persist session")
	}

	_, err = cacheDB.Do("SETEX", sessionTk, SessionTimeout, bSession)
	if err != nil {
		return errors.Wrap(err, "rooset: failed to persist session")
	}

	return nil
}

// FetchSession fetches user sessions from the cache
// returns:
// - A Session object, with a newly generated CSRF token, for authentication and GET requests
// - error
func FetchSession(
	cacheDB redis.Conn,
	sessionTk string,
) (session *messages.Session, err error) {
	session = &messages.Session{}
	resp, err := cacheDB.Do("GET", sessionTk)
	if err != nil {
		return nil, errors.Wrap(err, "rooset: session fetch failed")
	}
	if resp == nil {
		return nil, nil
	}

	bSession, ok := resp.([]byte)
	if !ok {
		return nil, errors.Wrap(err, "rooset: invalid session")
	}
	err = proto.Unmarshal(bSession, session)
	if err != nil {
		return nil, errors.Wrap(err, "rooset: invalid session")
	}

	return session, nil
}
