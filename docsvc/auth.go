package docsvc

import (
	"fmt"
	"io"
	"net/http"
	"reflect"

	"github.com/ddouglascarr/rooset/conf"
	"github.com/ddouglascarr/rooset/messages"
	jwt "github.com/dgrijalva/jwt-go"
	"github.com/golang/protobuf/descriptor"
	"github.com/golang/protobuf/jsonpb"
	"github.com/golang/protobuf/proto"
	protocdesc "github.com/golang/protobuf/protoc-gen-go/descriptor"
	"github.com/pkg/errors"
)

//JWTHandlerFunc http handler function that requires the JWT payload.
type JWTHandlerFunc func(
	w http.ResponseWriter,
	r *http.Request,
	m proto.Message,
)

func keyFunc(token *jwt.Token) (interface{}, error) {
	// TODO: handle expiry ?
	if _, ok := token.Method.(*jwt.SigningMethodHMAC); !ok {
		return nil, fmt.Errorf("Unexpected signing method: %v", token.Header["alg"])
	}
	// TODO: get key from config
	return []byte("development"), nil
}

// TODO: tighten this up
func setupResponse(w *http.ResponseWriter, req *http.Request) {
	(*w).Header().Set("Access-Control-Allow-Origin", "*")
	(*w).Header().Set("Access-Control-Allow-Methods", "POST, GET, OPTIONS, PUT, DELETE")
	(*w).Header().Set("Access-Control-Allow-Headers", "Accept, Content-Type, Content-Length, Accept-Encoding, X-CSRF-Token, Authorization")
}

//withAuthenticatedMessage closure for wrapping handlers which rely on a JWT payload
//responsds with error codes if the JWT token is invalid. It authenticates the request body
//as well, and calls the handle function with a unmarshaled proto.Message
// TODO: the authentication part of this needs testing
func withAuthenticatedMessage(messageType string, f JWTHandlerFunc) http.HandlerFunc {
	return func(w http.ResponseWriter, r *http.Request) {
		setupResponse(&w, r)
		if r.Method == "OPTIONS" {
			return
		}
		w.Header().Set("Content-Type", "application/json")

		var claims jwt.MapClaims
		var tokenHeader string
		for i, tk := range r.Header["Authorization"] {
			tokenHeader = tk
			if i == 1 {
				w.WriteHeader(http.StatusUnauthorized)
				io.WriteString(w, `{"errors": ["More than one Authorization header"]}`)
				return
			}
		}

		if tokenHeader == "" {
			w.WriteHeader(http.StatusUnauthorized)
			io.WriteString(w, `{"errors":["No Authorization Header"]}`)
			return
		}

		token, err := jwt.ParseWithClaims(tokenHeader, &claims, keyFunc)
		if err != nil {
			w.WriteHeader(http.StatusUnauthorized)
			io.WriteString(w, errors.Wrap(err, "Invalid auth token").Error())
			return
		}

		if !token.Valid {
			w.WriteHeader(http.StatusUnauthorized)
			io.WriteString(w, `{"errors":["Token not verified"]}`)
			return
		}

		// check claims against body
		pbtype := proto.MessageType(messageType)
		msg := reflect.New(pbtype.Elem()).Interface().(descriptor.Message)
		err = jsonpb.Unmarshal(r.Body, msg)
		if err != nil {
			w.WriteHeader(http.StatusBadRequest)
			io.WriteString(w, fmt.Sprintf(`{"errors": ["%s"]}`,
				errors.Wrap(err, "rooset: invalid request body").Error()))
			return
		}

		isAuth, failureMessage, err := authOpAndSvc(messageType, &claims)
		if err != nil {
			w.WriteHeader(http.StatusInternalServerError)
			io.WriteString(w, fmt.Sprintf(`{"errors": ["%s"]}`,
				errors.Wrap(err, "rooset: auth system error").Error()))
			return
		}
		if !isAuth {
			w.WriteHeader(http.StatusUnauthorized)
			io.WriteString(w, fmt.Sprintf(`{"errors": ["%s"]}`,
				failureMessage))
			return
		}

		isAuth, failureMessage, err = authReqAgainstClaims(msg, &claims)
		if err != nil {
			w.WriteHeader(http.StatusInternalServerError)
			io.WriteString(w, fmt.Sprintf(`{"errors": ["%s"]}`,
				errors.Wrap(err, "rooset: auth system error").Error()))
			return
		}
		if !isAuth {
			w.WriteHeader(http.StatusUnauthorized)
			io.WriteString(w, fmt.Sprintf(`{"errors": ["%s"]}`,
				failureMessage))
			return
		}

		f(w, r, msg)
	}
}

//BuildDocSHATk builds a jwt token to be passed to lffronted detailing a new
// document.
func BuildDocSHATk(sHA string) (string, error) {
	token := jwt.NewWithClaims(jwt.SigningMethodHS256, jwt.MapClaims{
		"SHA": sHA,
	})

	tkStr, err := token.SignedString([]byte(conf.Auth.JWTKey))
	if err != nil {
		return "", errors.Wrap(err, "rooset: failed to sign doc SHA")
	}

	return tkStr, nil
}

func doesFieldNeedValidation(fd *protocdesc.FieldDescriptorProto) (bool, error) {
	ex, err := proto.GetExtension(fd.Options, messages.E_Auth)
	if ex == nil {
		return false, nil
	}
	if err != nil {
		return true, err
	}
	auth, ok := ex.(*messages.FieldAuth)
	if !ok {
		return true, errors.New("rooset: auth option cast failed")
	}
	if auth.String() == messages.FieldAuth_JWTAuthRequired.String() {
		return true, nil
	}
	return false, nil
}

func isStringInSlice(s []interface{}, v string) bool {
	for _, sv := range s {
		strV, ok := sv.(string)
		if !ok {
			return false
		}
		if strV == v {
			return true
		}
	}
	return false
}

func authOpAndSvc(messageType string, claims *jwt.MapClaims) (bool, string, error) {
	opsRaw, ok := (*claims)["Op"]
	if !ok {
		return false, "claims.Op missing", nil
	}

	ops, ok := opsRaw.([]interface{})
	if !ok {
		return false, "claims.Op must be an array", nil
	}

	isAuthed := isStringInSlice(ops, messageType)
	if !isAuthed {
		return false, fmt.Sprintf("claims.Op does not include %s", messageType), nil
	}

	svcRaw, ok := (*claims)["Svc"]
	if !ok {
		return false, "claims.Svc missing", nil
	}

	svc, ok := svcRaw.([]interface{})
	if !ok {
		return false, "claims.Svc must be an array", nil
	}

	isAuthed = isStringInSlice(svc, "gitsvc")
	if !isAuthed {
		return false, fmt.Sprintf("claims.Svc does not include %s", "gitsvc"), nil
	}

	return true, "", nil
}

func authReqAgainstClaims(msg descriptor.Message, claims *jwt.MapClaims) (bool, string, error) {
	_, md := descriptor.ForMessage(msg)
	for _, fd := range md.Field {
		isAuth, err := doesFieldNeedValidation(fd)
		if err != nil {
			return false, "system error", errors.Wrap(err, "rooset: proto error")
		}
		if !isAuth {
			continue
		}
		msgValue := reflect.ValueOf(msg)
		fieldValue := reflect.Indirect(msgValue).FieldByName(*fd.Name)
		fieldStr, ok := fieldValue.Interface().(string)
		if !ok || fieldStr == "" {
			return false, fmt.Sprintf("body.%s must be a non-empty string", *fd.Name), nil
		}
		claimField, ok := (*claims)[*fd.Name]
		if !ok {
			return false, fmt.Sprintf("claims.%s missing", *fd.Name), nil
		}
		claimFieldVals, ok := claimField.([]interface{})
		if !ok {
			return false, fmt.Sprintf("claims.%s should be array", *fd.Name), nil
		}

		isAuthed := isStringInSlice(claimFieldVals, fieldStr)
		if !isAuthed {
			return false, fmt.Sprintf("claims.%s does not include %s", *fd.Name, fieldStr), nil
		}
	}
	return true, "", nil
}
