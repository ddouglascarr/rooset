package messages

// requires https://github.com/golang/protobuf to be installed

//go:generate protoc -I ../ --go_out ../ messages/custom_fields.proto
//go:generate protoc -I ../ --go_out ../ messages/unit_aggregate.proto
//go:generate protoc -I ../ --go_out ../ messages/unit_projections.proto
//go:generate protoc -I ../ --go_out ../ messages/app.proto
