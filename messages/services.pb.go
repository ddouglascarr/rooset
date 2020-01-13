// Code generated by protoc-gen-go. DO NOT EDIT.
// source: services.proto

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

type NewInitiativeReq struct {
	RepositoryName       string   `protobuf:"bytes,1,opt,name=RepositoryName,proto3" json:"RepositoryName,omitempty"`
	AreaID               string   `protobuf:"bytes,2,opt,name=AreaID,proto3" json:"AreaID,omitempty"`
	Content              string   `protobuf:"bytes,3,opt,name=Content,proto3" json:"Content,omitempty"`
	XXX_NoUnkeyedLiteral struct{} `json:"-"`
	XXX_unrecognized     []byte   `json:"-"`
	XXX_sizecache        int32    `json:"-"`
}

func (m *NewInitiativeReq) Reset()         { *m = NewInitiativeReq{} }
func (m *NewInitiativeReq) String() string { return proto.CompactTextString(m) }
func (*NewInitiativeReq) ProtoMessage()    {}
func (*NewInitiativeReq) Descriptor() ([]byte, []int) {
	return fileDescriptor_services_338fd1f2c19aa7f3, []int{0}
}
func (m *NewInitiativeReq) XXX_Unmarshal(b []byte) error {
	return xxx_messageInfo_NewInitiativeReq.Unmarshal(m, b)
}
func (m *NewInitiativeReq) XXX_Marshal(b []byte, deterministic bool) ([]byte, error) {
	return xxx_messageInfo_NewInitiativeReq.Marshal(b, m, deterministic)
}
func (dst *NewInitiativeReq) XXX_Merge(src proto.Message) {
	xxx_messageInfo_NewInitiativeReq.Merge(dst, src)
}
func (m *NewInitiativeReq) XXX_Size() int {
	return xxx_messageInfo_NewInitiativeReq.Size(m)
}
func (m *NewInitiativeReq) XXX_DiscardUnknown() {
	xxx_messageInfo_NewInitiativeReq.DiscardUnknown(m)
}

var xxx_messageInfo_NewInitiativeReq proto.InternalMessageInfo

func (m *NewInitiativeReq) GetRepositoryName() string {
	if m != nil {
		return m.RepositoryName
	}
	return ""
}

func (m *NewInitiativeReq) GetAreaID() string {
	if m != nil {
		return m.AreaID
	}
	return ""
}

func (m *NewInitiativeReq) GetContent() string {
	if m != nil {
		return m.Content
	}
	return ""
}

type NewInitiativeResp struct {
	CommitRecord         *CommitRecord `protobuf:"bytes,1,opt,name=CommitRecord,proto3" json:"CommitRecord,omitempty"`
	Tk                   string        `protobuf:"bytes,2,opt,name=Tk,proto3" json:"Tk,omitempty"`
	XXX_NoUnkeyedLiteral struct{}      `json:"-"`
	XXX_unrecognized     []byte        `json:"-"`
	XXX_sizecache        int32         `json:"-"`
}

func (m *NewInitiativeResp) Reset()         { *m = NewInitiativeResp{} }
func (m *NewInitiativeResp) String() string { return proto.CompactTextString(m) }
func (*NewInitiativeResp) ProtoMessage()    {}
func (*NewInitiativeResp) Descriptor() ([]byte, []int) {
	return fileDescriptor_services_338fd1f2c19aa7f3, []int{1}
}
func (m *NewInitiativeResp) XXX_Unmarshal(b []byte) error {
	return xxx_messageInfo_NewInitiativeResp.Unmarshal(m, b)
}
func (m *NewInitiativeResp) XXX_Marshal(b []byte, deterministic bool) ([]byte, error) {
	return xxx_messageInfo_NewInitiativeResp.Marshal(b, m, deterministic)
}
func (dst *NewInitiativeResp) XXX_Merge(src proto.Message) {
	xxx_messageInfo_NewInitiativeResp.Merge(dst, src)
}
func (m *NewInitiativeResp) XXX_Size() int {
	return xxx_messageInfo_NewInitiativeResp.Size(m)
}
func (m *NewInitiativeResp) XXX_DiscardUnknown() {
	xxx_messageInfo_NewInitiativeResp.DiscardUnknown(m)
}

var xxx_messageInfo_NewInitiativeResp proto.InternalMessageInfo

func (m *NewInitiativeResp) GetCommitRecord() *CommitRecord {
	if m != nil {
		return m.CommitRecord
	}
	return nil
}

func (m *NewInitiativeResp) GetTk() string {
	if m != nil {
		return m.Tk
	}
	return ""
}

type GetDocReq struct {
	RepositoryName       string   `protobuf:"bytes,1,opt,name=RepositoryName,proto3" json:"RepositoryName,omitempty"`
	AreaID               string   `protobuf:"bytes,2,opt,name=AreaID,proto3" json:"AreaID,omitempty"`
	GitRef               string   `protobuf:"bytes,3,opt,name=GitRef,proto3" json:"GitRef,omitempty"`
	XXX_NoUnkeyedLiteral struct{} `json:"-"`
	XXX_unrecognized     []byte   `json:"-"`
	XXX_sizecache        int32    `json:"-"`
}

func (m *GetDocReq) Reset()         { *m = GetDocReq{} }
func (m *GetDocReq) String() string { return proto.CompactTextString(m) }
func (*GetDocReq) ProtoMessage()    {}
func (*GetDocReq) Descriptor() ([]byte, []int) {
	return fileDescriptor_services_338fd1f2c19aa7f3, []int{2}
}
func (m *GetDocReq) XXX_Unmarshal(b []byte) error {
	return xxx_messageInfo_GetDocReq.Unmarshal(m, b)
}
func (m *GetDocReq) XXX_Marshal(b []byte, deterministic bool) ([]byte, error) {
	return xxx_messageInfo_GetDocReq.Marshal(b, m, deterministic)
}
func (dst *GetDocReq) XXX_Merge(src proto.Message) {
	xxx_messageInfo_GetDocReq.Merge(dst, src)
}
func (m *GetDocReq) XXX_Size() int {
	return xxx_messageInfo_GetDocReq.Size(m)
}
func (m *GetDocReq) XXX_DiscardUnknown() {
	xxx_messageInfo_GetDocReq.DiscardUnknown(m)
}

var xxx_messageInfo_GetDocReq proto.InternalMessageInfo

func (m *GetDocReq) GetRepositoryName() string {
	if m != nil {
		return m.RepositoryName
	}
	return ""
}

func (m *GetDocReq) GetAreaID() string {
	if m != nil {
		return m.AreaID
	}
	return ""
}

func (m *GetDocReq) GetGitRef() string {
	if m != nil {
		return m.GitRef
	}
	return ""
}

type GetDocResp struct {
	Blob                 *Blob    `protobuf:"bytes,1,opt,name=Blob,proto3" json:"Blob,omitempty"`
	XXX_NoUnkeyedLiteral struct{} `json:"-"`
	XXX_unrecognized     []byte   `json:"-"`
	XXX_sizecache        int32    `json:"-"`
}

func (m *GetDocResp) Reset()         { *m = GetDocResp{} }
func (m *GetDocResp) String() string { return proto.CompactTextString(m) }
func (*GetDocResp) ProtoMessage()    {}
func (*GetDocResp) Descriptor() ([]byte, []int) {
	return fileDescriptor_services_338fd1f2c19aa7f3, []int{3}
}
func (m *GetDocResp) XXX_Unmarshal(b []byte) error {
	return xxx_messageInfo_GetDocResp.Unmarshal(m, b)
}
func (m *GetDocResp) XXX_Marshal(b []byte, deterministic bool) ([]byte, error) {
	return xxx_messageInfo_GetDocResp.Marshal(b, m, deterministic)
}
func (dst *GetDocResp) XXX_Merge(src proto.Message) {
	xxx_messageInfo_GetDocResp.Merge(dst, src)
}
func (m *GetDocResp) XXX_Size() int {
	return xxx_messageInfo_GetDocResp.Size(m)
}
func (m *GetDocResp) XXX_DiscardUnknown() {
	xxx_messageInfo_GetDocResp.DiscardUnknown(m)
}

var xxx_messageInfo_GetDocResp proto.InternalMessageInfo

func (m *GetDocResp) GetBlob() *Blob {
	if m != nil {
		return m.Blob
	}
	return nil
}

func init() {
	proto.RegisterType((*NewInitiativeReq)(nil), "messages.NewInitiativeReq")
	proto.RegisterType((*NewInitiativeResp)(nil), "messages.NewInitiativeResp")
	proto.RegisterType((*GetDocReq)(nil), "messages.GetDocReq")
	proto.RegisterType((*GetDocResp)(nil), "messages.GetDocResp")
}

func init() { proto.RegisterFile("services.proto", fileDescriptor_services_338fd1f2c19aa7f3) }

var fileDescriptor_services_338fd1f2c19aa7f3 = []byte{
	// 255 bytes of a gzipped FileDescriptorProto
	0x1f, 0x8b, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0xff, 0xac, 0x91, 0xc1, 0x4a, 0xc3, 0x40,
	0x10, 0x86, 0x49, 0x2c, 0xd1, 0x8c, 0x1a, 0x74, 0x0f, 0x25, 0x14, 0x05, 0xd9, 0x93, 0x07, 0x09,
	0xa2, 0x37, 0x6f, 0xb6, 0x85, 0xd2, 0x4b, 0x0f, 0x4b, 0xef, 0x92, 0xc6, 0xb1, 0x2c, 0x35, 0x3b,
	0xeb, 0xee, 0x50, 0xf5, 0x2d, 0x7c, 0x20, 0x1f, 0x4e, 0x92, 0x6e, 0xb0, 0xe6, 0xec, 0x71, 0xfe,
	0xff, 0x87, 0x6f, 0x3f, 0x16, 0x32, 0x8f, 0x6e, 0xab, 0x2b, 0xf4, 0x85, 0x75, 0xc4, 0x24, 0x8e,
	0x6a, 0xf4, 0xbe, 0x5c, 0xa3, 0x1f, 0xa5, 0x6b, 0xcd, 0xbb, 0x70, 0x74, 0x4a, 0x96, 0x35, 0x99,
	0xb0, 0x91, 0x1f, 0x70, 0xb6, 0xc0, 0xf7, 0xb9, 0xd1, 0xac, 0x4b, 0xd6, 0x5b, 0x54, 0xf8, 0x26,
	0x6e, 0x20, 0x53, 0x68, 0xc9, 0x6b, 0x26, 0xf7, 0xb9, 0x28, 0x6b, 0xcc, 0xa3, 0xab, 0xe8, 0x3a,
	0x1d, 0x0f, 0xbe, 0xbe, 0x2f, 0x23, 0xd5, 0xeb, 0xc4, 0x05, 0x24, 0x8f, 0x0e, 0xcb, 0xf9, 0x34,
	0x8f, 0xf7, 0x56, 0x21, 0x13, 0x39, 0x1c, 0x4e, 0xc8, 0x30, 0x1a, 0xce, 0x0f, 0x9a, 0x5a, 0x75,
	0xa7, 0x7c, 0x82, 0xf3, 0x1e, 0xd9, 0x5b, 0xf1, 0x00, 0x27, 0x13, 0xaa, 0x6b, 0xcd, 0x0a, 0x2b,
	0x72, 0xcf, 0x2d, 0xf8, 0xf8, 0x6e, 0x58, 0x74, 0x26, 0xc5, 0x7e, 0xab, 0xfe, 0x6c, 0x45, 0x06,
	0xf1, 0x72, 0xb3, 0x7b, 0x84, 0x8a, 0x97, 0x1b, 0x49, 0x90, 0xce, 0x90, 0xa7, 0x54, 0xfd, 0xb7,
	0xd3, 0x10, 0x92, 0x59, 0x43, 0x7d, 0x09, 0x4a, 0xe1, 0x92, 0xb7, 0x00, 0x1d, 0xd0, 0x5b, 0x21,
	0x61, 0x30, 0x7e, 0xa5, 0x55, 0x50, 0xc8, 0x7e, 0x15, 0x9a, 0x54, 0xb5, 0xdd, 0x2a, 0x69, 0x3f,
	0xe1, 0xfe, 0x27, 0x00, 0x00, 0xff, 0xff, 0xba, 0xbc, 0xa0, 0x90, 0xba, 0x01, 0x00, 0x00,
}
