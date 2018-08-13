// Code generated by protoc-gen-go. DO NOT EDIT.
// source: messages/projections.proto

package messages

import proto "github.com/golang/protobuf/proto"
import fmt "fmt"
import math "math"

// Reference imports to suppress errors if they are not otherwise used.
var _ = proto.Marshal
var _ = fmt.Errorf
var _ = math.Inf

// This is a compile-time assertion to ensure that this generated file
// is compatible with the proto package it is being compiled against.
// A compilation error at this line likely means your copy of the
// proto package needs to be updated.
const _ = proto.ProtoPackageIsVersion2 // please upgrade the proto package

type UnitProjection struct {
	UnitID               string   `protobuf:"bytes,1,opt,name=UnitID,proto3" json:"UnitID,omitempty"`
	Name                 string   `protobuf:"bytes,2,opt,name=Name,proto3" json:"Name,omitempty"`
	Description          string   `protobuf:"bytes,3,opt,name=Description,proto3" json:"Description,omitempty"`
	URLParameterName     string   `protobuf:"bytes,4,opt,name=URLParameterName,proto3" json:"URLParameterName,omitempty"`
	MemberCount          uint32   `protobuf:"varint,5,opt,name=MemberCount,proto3" json:"MemberCount,omitempty"`
	TotalWeight          uint32   `protobuf:"varint,6,opt,name=TotalWeight,proto3" json:"TotalWeight,omitempty"`
	XXX_NoUnkeyedLiteral struct{} `json:"-"`
	XXX_unrecognized     []byte   `json:"-"`
	XXX_sizecache        int32    `json:"-"`
}

func (m *UnitProjection) Reset()         { *m = UnitProjection{} }
func (m *UnitProjection) String() string { return proto.CompactTextString(m) }
func (*UnitProjection) ProtoMessage()    {}
func (*UnitProjection) Descriptor() ([]byte, []int) {
	return fileDescriptor_projections_ec880357f650ca97, []int{0}
}
func (m *UnitProjection) XXX_Unmarshal(b []byte) error {
	return xxx_messageInfo_UnitProjection.Unmarshal(m, b)
}
func (m *UnitProjection) XXX_Marshal(b []byte, deterministic bool) ([]byte, error) {
	return xxx_messageInfo_UnitProjection.Marshal(b, m, deterministic)
}
func (dst *UnitProjection) XXX_Merge(src proto.Message) {
	xxx_messageInfo_UnitProjection.Merge(dst, src)
}
func (m *UnitProjection) XXX_Size() int {
	return xxx_messageInfo_UnitProjection.Size(m)
}
func (m *UnitProjection) XXX_DiscardUnknown() {
	xxx_messageInfo_UnitProjection.DiscardUnknown(m)
}

var xxx_messageInfo_UnitProjection proto.InternalMessageInfo

func (m *UnitProjection) GetUnitID() string {
	if m != nil {
		return m.UnitID
	}
	return ""
}

func (m *UnitProjection) GetName() string {
	if m != nil {
		return m.Name
	}
	return ""
}

func (m *UnitProjection) GetDescription() string {
	if m != nil {
		return m.Description
	}
	return ""
}

func (m *UnitProjection) GetURLParameterName() string {
	if m != nil {
		return m.URLParameterName
	}
	return ""
}

func (m *UnitProjection) GetMemberCount() uint32 {
	if m != nil {
		return m.MemberCount
	}
	return 0
}

func (m *UnitProjection) GetTotalWeight() uint32 {
	if m != nil {
		return m.TotalWeight
	}
	return 0
}

func init() {
	proto.RegisterType((*UnitProjection)(nil), "messages.UnitProjection")
}

func init() {
	proto.RegisterFile("messages/projections.proto", fileDescriptor_projections_ec880357f650ca97)
}

var fileDescriptor_projections_ec880357f650ca97 = []byte{
	// 212 bytes of a gzipped FileDescriptorProto
	0x1f, 0x8b, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0xff, 0xe2, 0x92, 0xca, 0x4d, 0x2d, 0x2e,
	0x4e, 0x4c, 0x4f, 0x2d, 0xd6, 0x2f, 0x28, 0xca, 0xcf, 0x4a, 0x4d, 0x2e, 0xc9, 0xcc, 0xcf, 0x2b,
	0xd6, 0x2b, 0x28, 0xca, 0x2f, 0xc9, 0x17, 0xe2, 0x80, 0xc9, 0x49, 0xc9, 0xc0, 0x55, 0x25, 0x97,
	0x16, 0x97, 0xe4, 0xe7, 0xc6, 0xa7, 0x65, 0xa6, 0xe6, 0xa4, 0x40, 0xd5, 0x29, 0xdd, 0x62, 0xe4,
	0xe2, 0x0b, 0xcd, 0xcb, 0x2c, 0x09, 0x80, 0x9b, 0x20, 0x24, 0xc6, 0xc5, 0x06, 0x12, 0xf1, 0x74,
	0x91, 0x60, 0x54, 0x60, 0xd4, 0xe0, 0x0c, 0x82, 0xf2, 0x84, 0x84, 0xb8, 0x58, 0xfc, 0x12, 0x73,
	0x53, 0x25, 0x98, 0xc0, 0xa2, 0x60, 0xb6, 0x90, 0x02, 0x17, 0xb7, 0x4b, 0x6a, 0x71, 0x72, 0x51,
	0x66, 0x01, 0x48, 0xab, 0x04, 0x33, 0x58, 0x0a, 0x59, 0x48, 0x48, 0x8b, 0x4b, 0x20, 0x34, 0xc8,
	0x27, 0x20, 0xb1, 0x28, 0x31, 0x37, 0xb5, 0x24, 0xb5, 0x08, 0x6c, 0x02, 0x0b, 0x58, 0x19, 0x86,
	0x38, 0xc8, 0x34, 0xdf, 0xd4, 0xdc, 0xa4, 0xd4, 0x22, 0xe7, 0xfc, 0xd2, 0xbc, 0x12, 0x09, 0x56,
	0x05, 0x46, 0x0d, 0xde, 0x20, 0x64, 0x21, 0x90, 0x8a, 0x90, 0xfc, 0x92, 0xc4, 0x9c, 0xf0, 0xd4,
	0xcc, 0xf4, 0x8c, 0x12, 0x09, 0x36, 0x88, 0x0a, 0x24, 0x21, 0x2b, 0xae, 0x59, 0x4d, 0x92, 0x50,
	0x17, 0x27, 0xb1, 0x81, 0xfd, 0x68, 0x0c, 0x08, 0x00, 0x00, 0xff, 0xff, 0x60, 0x36, 0x7e, 0xeb,
	0x29, 0x01, 0x00, 0x00,
}
