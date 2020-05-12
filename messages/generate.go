package messages

// requires https://github.com/golang/protobuf to be installed
// requires https://github.com/protobufjs/protobuf.js to be installed

//go:generate protoc -I . --go_out . options.proto
//go:generate protoc -I . --go_out . services.proto
//go:generate pbjs -t static-module --no-create --no-encode --no-decode  -w commonjs -o index.js services.proto
//go:generate pbts -o index.d.ts index.js
