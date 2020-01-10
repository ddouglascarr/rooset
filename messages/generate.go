package messages

// requires https://github.com/golang/protobuf to be installed

//go:generate protoc -I . --go_out . git.proto
//go:generate protoc -I . --go_out . services.proto
