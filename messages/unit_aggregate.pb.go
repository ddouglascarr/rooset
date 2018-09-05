// Code generated by protoc-gen-go. DO NOT EDIT.
// source: messages/unit_aggregate.proto

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

type CreateUnitCommand struct {
	UnitID               string   `protobuf:"bytes,1,opt,name=UnitID,proto3" json:"UnitID,omitempty"`
	RequesterID          string   `protobuf:"bytes,2,opt,name=RequesterID,proto3" json:"RequesterID,omitempty"`
	Name                 string   `protobuf:"bytes,3,opt,name=Name,proto3" json:"Name,omitempty"`
	Description          string   `protobuf:"bytes,4,opt,name=Description,proto3" json:"Description,omitempty"`
	URLParameterName     string   `protobuf:"bytes,5,opt,name=URLParameterName,proto3" json:"URLParameterName,omitempty"`
	XXX_NoUnkeyedLiteral struct{} `json:"-"`
	XXX_unrecognized     []byte   `json:"-"`
	XXX_sizecache        int32    `json:"-"`
}

func (m *CreateUnitCommand) Reset()         { *m = CreateUnitCommand{} }
func (m *CreateUnitCommand) String() string { return proto.CompactTextString(m) }
func (*CreateUnitCommand) ProtoMessage()    {}
func (*CreateUnitCommand) Descriptor() ([]byte, []int) {
	return fileDescriptor_unit_aggregate_82b8f812f07e36ec, []int{0}
}
func (m *CreateUnitCommand) XXX_Unmarshal(b []byte) error {
	return xxx_messageInfo_CreateUnitCommand.Unmarshal(m, b)
}
func (m *CreateUnitCommand) XXX_Marshal(b []byte, deterministic bool) ([]byte, error) {
	return xxx_messageInfo_CreateUnitCommand.Marshal(b, m, deterministic)
}
func (dst *CreateUnitCommand) XXX_Merge(src proto.Message) {
	xxx_messageInfo_CreateUnitCommand.Merge(dst, src)
}
func (m *CreateUnitCommand) XXX_Size() int {
	return xxx_messageInfo_CreateUnitCommand.Size(m)
}
func (m *CreateUnitCommand) XXX_DiscardUnknown() {
	xxx_messageInfo_CreateUnitCommand.DiscardUnknown(m)
}

var xxx_messageInfo_CreateUnitCommand proto.InternalMessageInfo

func (m *CreateUnitCommand) GetUnitID() string {
	if m != nil {
		return m.UnitID
	}
	return ""
}

func (m *CreateUnitCommand) GetRequesterID() string {
	if m != nil {
		return m.RequesterID
	}
	return ""
}

func (m *CreateUnitCommand) GetName() string {
	if m != nil {
		return m.Name
	}
	return ""
}

func (m *CreateUnitCommand) GetDescription() string {
	if m != nil {
		return m.Description
	}
	return ""
}

func (m *CreateUnitCommand) GetURLParameterName() string {
	if m != nil {
		return m.URLParameterName
	}
	return ""
}

type UnitCreatedEvent struct {
	UnitID               string   `protobuf:"bytes,1,opt,name=UnitID,proto3" json:"UnitID,omitempty"`
	RequesterID          string   `protobuf:"bytes,2,opt,name=RequesterID,proto3" json:"RequesterID,omitempty"`
	Name                 string   `protobuf:"bytes,3,opt,name=Name,proto3" json:"Name,omitempty"`
	Description          string   `protobuf:"bytes,4,opt,name=Description,proto3" json:"Description,omitempty"`
	URLParameterName     string   `protobuf:"bytes,5,opt,name=URLParameterName,proto3" json:"URLParameterName,omitempty"`
	XXX_NoUnkeyedLiteral struct{} `json:"-"`
	XXX_unrecognized     []byte   `json:"-"`
	XXX_sizecache        int32    `json:"-"`
}

func (m *UnitCreatedEvent) Reset()         { *m = UnitCreatedEvent{} }
func (m *UnitCreatedEvent) String() string { return proto.CompactTextString(m) }
func (*UnitCreatedEvent) ProtoMessage()    {}
func (*UnitCreatedEvent) Descriptor() ([]byte, []int) {
	return fileDescriptor_unit_aggregate_82b8f812f07e36ec, []int{1}
}
func (m *UnitCreatedEvent) XXX_Unmarshal(b []byte) error {
	return xxx_messageInfo_UnitCreatedEvent.Unmarshal(m, b)
}
func (m *UnitCreatedEvent) XXX_Marshal(b []byte, deterministic bool) ([]byte, error) {
	return xxx_messageInfo_UnitCreatedEvent.Marshal(b, m, deterministic)
}
func (dst *UnitCreatedEvent) XXX_Merge(src proto.Message) {
	xxx_messageInfo_UnitCreatedEvent.Merge(dst, src)
}
func (m *UnitCreatedEvent) XXX_Size() int {
	return xxx_messageInfo_UnitCreatedEvent.Size(m)
}
func (m *UnitCreatedEvent) XXX_DiscardUnknown() {
	xxx_messageInfo_UnitCreatedEvent.DiscardUnknown(m)
}

var xxx_messageInfo_UnitCreatedEvent proto.InternalMessageInfo

func (m *UnitCreatedEvent) GetUnitID() string {
	if m != nil {
		return m.UnitID
	}
	return ""
}

func (m *UnitCreatedEvent) GetRequesterID() string {
	if m != nil {
		return m.RequesterID
	}
	return ""
}

func (m *UnitCreatedEvent) GetName() string {
	if m != nil {
		return m.Name
	}
	return ""
}

func (m *UnitCreatedEvent) GetDescription() string {
	if m != nil {
		return m.Description
	}
	return ""
}

func (m *UnitCreatedEvent) GetURLParameterName() string {
	if m != nil {
		return m.URLParameterName
	}
	return ""
}

type GrantPrivilegeCommand struct {
	UnitID               string   `protobuf:"bytes,1,opt,name=UnitID,proto3" json:"UnitID,omitempty"`
	RequesterID          string   `protobuf:"bytes,2,opt,name=RequesterID,proto3" json:"RequesterID,omitempty"`
	MemberID             string   `protobuf:"bytes,3,opt,name=MemberID,proto3" json:"MemberID,omitempty"`
	VotingRight          bool     `protobuf:"varint,5,opt,name=VotingRight,proto3" json:"VotingRight,omitempty"`
	InitiativeRight      bool     `protobuf:"varint,6,opt,name=InitiativeRight,proto3" json:"InitiativeRight,omitempty"`
	ManagementRight      bool     `protobuf:"varint,7,opt,name=ManagementRight,proto3" json:"ManagementRight,omitempty"`
	Weight               uint32   `protobuf:"varint,8,opt,name=Weight,proto3" json:"Weight,omitempty"`
	XXX_NoUnkeyedLiteral struct{} `json:"-"`
	XXX_unrecognized     []byte   `json:"-"`
	XXX_sizecache        int32    `json:"-"`
}

func (m *GrantPrivilegeCommand) Reset()         { *m = GrantPrivilegeCommand{} }
func (m *GrantPrivilegeCommand) String() string { return proto.CompactTextString(m) }
func (*GrantPrivilegeCommand) ProtoMessage()    {}
func (*GrantPrivilegeCommand) Descriptor() ([]byte, []int) {
	return fileDescriptor_unit_aggregate_82b8f812f07e36ec, []int{2}
}
func (m *GrantPrivilegeCommand) XXX_Unmarshal(b []byte) error {
	return xxx_messageInfo_GrantPrivilegeCommand.Unmarshal(m, b)
}
func (m *GrantPrivilegeCommand) XXX_Marshal(b []byte, deterministic bool) ([]byte, error) {
	return xxx_messageInfo_GrantPrivilegeCommand.Marshal(b, m, deterministic)
}
func (dst *GrantPrivilegeCommand) XXX_Merge(src proto.Message) {
	xxx_messageInfo_GrantPrivilegeCommand.Merge(dst, src)
}
func (m *GrantPrivilegeCommand) XXX_Size() int {
	return xxx_messageInfo_GrantPrivilegeCommand.Size(m)
}
func (m *GrantPrivilegeCommand) XXX_DiscardUnknown() {
	xxx_messageInfo_GrantPrivilegeCommand.DiscardUnknown(m)
}

var xxx_messageInfo_GrantPrivilegeCommand proto.InternalMessageInfo

func (m *GrantPrivilegeCommand) GetUnitID() string {
	if m != nil {
		return m.UnitID
	}
	return ""
}

func (m *GrantPrivilegeCommand) GetRequesterID() string {
	if m != nil {
		return m.RequesterID
	}
	return ""
}

func (m *GrantPrivilegeCommand) GetMemberID() string {
	if m != nil {
		return m.MemberID
	}
	return ""
}

func (m *GrantPrivilegeCommand) GetVotingRight() bool {
	if m != nil {
		return m.VotingRight
	}
	return false
}

func (m *GrantPrivilegeCommand) GetInitiativeRight() bool {
	if m != nil {
		return m.InitiativeRight
	}
	return false
}

func (m *GrantPrivilegeCommand) GetManagementRight() bool {
	if m != nil {
		return m.ManagementRight
	}
	return false
}

func (m *GrantPrivilegeCommand) GetWeight() uint32 {
	if m != nil {
		return m.Weight
	}
	return 0
}

type PrivilegeGrantedEvent struct {
	UnitID               string   `protobuf:"bytes,1,opt,name=UnitID,proto3" json:"UnitID,omitempty"`
	RequesterID          string   `protobuf:"bytes,2,opt,name=RequesterID,proto3" json:"RequesterID,omitempty"`
	MemberID             string   `protobuf:"bytes,3,opt,name=MemberID,proto3" json:"MemberID,omitempty"`
	VotingRight          bool     `protobuf:"varint,5,opt,name=VotingRight,proto3" json:"VotingRight,omitempty"`
	InitiativeRight      bool     `protobuf:"varint,6,opt,name=InitiativeRight,proto3" json:"InitiativeRight,omitempty"`
	ManagementRight      bool     `protobuf:"varint,7,opt,name=ManagementRight,proto3" json:"ManagementRight,omitempty"`
	Weight               uint32   `protobuf:"varint,8,opt,name=Weight,proto3" json:"Weight,omitempty"`
	XXX_NoUnkeyedLiteral struct{} `json:"-"`
	XXX_unrecognized     []byte   `json:"-"`
	XXX_sizecache        int32    `json:"-"`
}

func (m *PrivilegeGrantedEvent) Reset()         { *m = PrivilegeGrantedEvent{} }
func (m *PrivilegeGrantedEvent) String() string { return proto.CompactTextString(m) }
func (*PrivilegeGrantedEvent) ProtoMessage()    {}
func (*PrivilegeGrantedEvent) Descriptor() ([]byte, []int) {
	return fileDescriptor_unit_aggregate_82b8f812f07e36ec, []int{3}
}
func (m *PrivilegeGrantedEvent) XXX_Unmarshal(b []byte) error {
	return xxx_messageInfo_PrivilegeGrantedEvent.Unmarshal(m, b)
}
func (m *PrivilegeGrantedEvent) XXX_Marshal(b []byte, deterministic bool) ([]byte, error) {
	return xxx_messageInfo_PrivilegeGrantedEvent.Marshal(b, m, deterministic)
}
func (dst *PrivilegeGrantedEvent) XXX_Merge(src proto.Message) {
	xxx_messageInfo_PrivilegeGrantedEvent.Merge(dst, src)
}
func (m *PrivilegeGrantedEvent) XXX_Size() int {
	return xxx_messageInfo_PrivilegeGrantedEvent.Size(m)
}
func (m *PrivilegeGrantedEvent) XXX_DiscardUnknown() {
	xxx_messageInfo_PrivilegeGrantedEvent.DiscardUnknown(m)
}

var xxx_messageInfo_PrivilegeGrantedEvent proto.InternalMessageInfo

func (m *PrivilegeGrantedEvent) GetUnitID() string {
	if m != nil {
		return m.UnitID
	}
	return ""
}

func (m *PrivilegeGrantedEvent) GetRequesterID() string {
	if m != nil {
		return m.RequesterID
	}
	return ""
}

func (m *PrivilegeGrantedEvent) GetMemberID() string {
	if m != nil {
		return m.MemberID
	}
	return ""
}

func (m *PrivilegeGrantedEvent) GetVotingRight() bool {
	if m != nil {
		return m.VotingRight
	}
	return false
}

func (m *PrivilegeGrantedEvent) GetInitiativeRight() bool {
	if m != nil {
		return m.InitiativeRight
	}
	return false
}

func (m *PrivilegeGrantedEvent) GetManagementRight() bool {
	if m != nil {
		return m.ManagementRight
	}
	return false
}

func (m *PrivilegeGrantedEvent) GetWeight() uint32 {
	if m != nil {
		return m.Weight
	}
	return 0
}

type RevokePrivilegeCommand struct {
	UnitID               string   `protobuf:"bytes,1,opt,name=UnitID,proto3" json:"UnitID,omitempty"`
	RequesterID          string   `protobuf:"bytes,2,opt,name=RequesterID,proto3" json:"RequesterID,omitempty"`
	MemberID             string   `protobuf:"bytes,3,opt,name=MemberID,proto3" json:"MemberID,omitempty"`
	XXX_NoUnkeyedLiteral struct{} `json:"-"`
	XXX_unrecognized     []byte   `json:"-"`
	XXX_sizecache        int32    `json:"-"`
}

func (m *RevokePrivilegeCommand) Reset()         { *m = RevokePrivilegeCommand{} }
func (m *RevokePrivilegeCommand) String() string { return proto.CompactTextString(m) }
func (*RevokePrivilegeCommand) ProtoMessage()    {}
func (*RevokePrivilegeCommand) Descriptor() ([]byte, []int) {
	return fileDescriptor_unit_aggregate_82b8f812f07e36ec, []int{4}
}
func (m *RevokePrivilegeCommand) XXX_Unmarshal(b []byte) error {
	return xxx_messageInfo_RevokePrivilegeCommand.Unmarshal(m, b)
}
func (m *RevokePrivilegeCommand) XXX_Marshal(b []byte, deterministic bool) ([]byte, error) {
	return xxx_messageInfo_RevokePrivilegeCommand.Marshal(b, m, deterministic)
}
func (dst *RevokePrivilegeCommand) XXX_Merge(src proto.Message) {
	xxx_messageInfo_RevokePrivilegeCommand.Merge(dst, src)
}
func (m *RevokePrivilegeCommand) XXX_Size() int {
	return xxx_messageInfo_RevokePrivilegeCommand.Size(m)
}
func (m *RevokePrivilegeCommand) XXX_DiscardUnknown() {
	xxx_messageInfo_RevokePrivilegeCommand.DiscardUnknown(m)
}

var xxx_messageInfo_RevokePrivilegeCommand proto.InternalMessageInfo

func (m *RevokePrivilegeCommand) GetUnitID() string {
	if m != nil {
		return m.UnitID
	}
	return ""
}

func (m *RevokePrivilegeCommand) GetRequesterID() string {
	if m != nil {
		return m.RequesterID
	}
	return ""
}

func (m *RevokePrivilegeCommand) GetMemberID() string {
	if m != nil {
		return m.MemberID
	}
	return ""
}

type PrivilegeRevokedEvent struct {
	UnitID               string   `protobuf:"bytes,1,opt,name=UnitID,proto3" json:"UnitID,omitempty"`
	RequesterID          string   `protobuf:"bytes,2,opt,name=RequesterID,proto3" json:"RequesterID,omitempty"`
	MemberID             string   `protobuf:"bytes,3,opt,name=MemberID,proto3" json:"MemberID,omitempty"`
	Weight               uint32   `protobuf:"varint,4,opt,name=Weight,proto3" json:"Weight,omitempty"`
	XXX_NoUnkeyedLiteral struct{} `json:"-"`
	XXX_unrecognized     []byte   `json:"-"`
	XXX_sizecache        int32    `json:"-"`
}

func (m *PrivilegeRevokedEvent) Reset()         { *m = PrivilegeRevokedEvent{} }
func (m *PrivilegeRevokedEvent) String() string { return proto.CompactTextString(m) }
func (*PrivilegeRevokedEvent) ProtoMessage()    {}
func (*PrivilegeRevokedEvent) Descriptor() ([]byte, []int) {
	return fileDescriptor_unit_aggregate_82b8f812f07e36ec, []int{5}
}
func (m *PrivilegeRevokedEvent) XXX_Unmarshal(b []byte) error {
	return xxx_messageInfo_PrivilegeRevokedEvent.Unmarshal(m, b)
}
func (m *PrivilegeRevokedEvent) XXX_Marshal(b []byte, deterministic bool) ([]byte, error) {
	return xxx_messageInfo_PrivilegeRevokedEvent.Marshal(b, m, deterministic)
}
func (dst *PrivilegeRevokedEvent) XXX_Merge(src proto.Message) {
	xxx_messageInfo_PrivilegeRevokedEvent.Merge(dst, src)
}
func (m *PrivilegeRevokedEvent) XXX_Size() int {
	return xxx_messageInfo_PrivilegeRevokedEvent.Size(m)
}
func (m *PrivilegeRevokedEvent) XXX_DiscardUnknown() {
	xxx_messageInfo_PrivilegeRevokedEvent.DiscardUnknown(m)
}

var xxx_messageInfo_PrivilegeRevokedEvent proto.InternalMessageInfo

func (m *PrivilegeRevokedEvent) GetUnitID() string {
	if m != nil {
		return m.UnitID
	}
	return ""
}

func (m *PrivilegeRevokedEvent) GetRequesterID() string {
	if m != nil {
		return m.RequesterID
	}
	return ""
}

func (m *PrivilegeRevokedEvent) GetMemberID() string {
	if m != nil {
		return m.MemberID
	}
	return ""
}

func (m *PrivilegeRevokedEvent) GetWeight() uint32 {
	if m != nil {
		return m.Weight
	}
	return 0
}

type CreateAreaCommand struct {
	UnitID               string   `protobuf:"bytes,1,opt,name=UnitID,proto3" json:"UnitID,omitempty"`
	RequesterID          string   `protobuf:"bytes,2,opt,name=RequesterID,proto3" json:"RequesterID,omitempty"`
	AreaID               string   `protobuf:"bytes,3,opt,name=AreaID,proto3" json:"AreaID,omitempty"`
	Name                 string   `protobuf:"bytes,4,opt,name=Name,proto3" json:"Name,omitempty"`
	Description          string   `protobuf:"bytes,5,opt,name=Description,proto3" json:"Description,omitempty"`
	XXX_NoUnkeyedLiteral struct{} `json:"-"`
	XXX_unrecognized     []byte   `json:"-"`
	XXX_sizecache        int32    `json:"-"`
}

func (m *CreateAreaCommand) Reset()         { *m = CreateAreaCommand{} }
func (m *CreateAreaCommand) String() string { return proto.CompactTextString(m) }
func (*CreateAreaCommand) ProtoMessage()    {}
func (*CreateAreaCommand) Descriptor() ([]byte, []int) {
	return fileDescriptor_unit_aggregate_82b8f812f07e36ec, []int{6}
}
func (m *CreateAreaCommand) XXX_Unmarshal(b []byte) error {
	return xxx_messageInfo_CreateAreaCommand.Unmarshal(m, b)
}
func (m *CreateAreaCommand) XXX_Marshal(b []byte, deterministic bool) ([]byte, error) {
	return xxx_messageInfo_CreateAreaCommand.Marshal(b, m, deterministic)
}
func (dst *CreateAreaCommand) XXX_Merge(src proto.Message) {
	xxx_messageInfo_CreateAreaCommand.Merge(dst, src)
}
func (m *CreateAreaCommand) XXX_Size() int {
	return xxx_messageInfo_CreateAreaCommand.Size(m)
}
func (m *CreateAreaCommand) XXX_DiscardUnknown() {
	xxx_messageInfo_CreateAreaCommand.DiscardUnknown(m)
}

var xxx_messageInfo_CreateAreaCommand proto.InternalMessageInfo

func (m *CreateAreaCommand) GetUnitID() string {
	if m != nil {
		return m.UnitID
	}
	return ""
}

func (m *CreateAreaCommand) GetRequesterID() string {
	if m != nil {
		return m.RequesterID
	}
	return ""
}

func (m *CreateAreaCommand) GetAreaID() string {
	if m != nil {
		return m.AreaID
	}
	return ""
}

func (m *CreateAreaCommand) GetName() string {
	if m != nil {
		return m.Name
	}
	return ""
}

func (m *CreateAreaCommand) GetDescription() string {
	if m != nil {
		return m.Description
	}
	return ""
}

type AreaCreatedEvent struct {
	UnitID               string   `protobuf:"bytes,1,opt,name=UnitID,proto3" json:"UnitID,omitempty"`
	RequesterID          string   `protobuf:"bytes,2,opt,name=RequesterID,proto3" json:"RequesterID,omitempty"`
	AreaID               string   `protobuf:"bytes,3,opt,name=AreaID,proto3" json:"AreaID,omitempty"`
	Name                 string   `protobuf:"bytes,4,opt,name=Name,proto3" json:"Name,omitempty"`
	Description          string   `protobuf:"bytes,5,opt,name=Description,proto3" json:"Description,omitempty"`
	XXX_NoUnkeyedLiteral struct{} `json:"-"`
	XXX_unrecognized     []byte   `json:"-"`
	XXX_sizecache        int32    `json:"-"`
}

func (m *AreaCreatedEvent) Reset()         { *m = AreaCreatedEvent{} }
func (m *AreaCreatedEvent) String() string { return proto.CompactTextString(m) }
func (*AreaCreatedEvent) ProtoMessage()    {}
func (*AreaCreatedEvent) Descriptor() ([]byte, []int) {
	return fileDescriptor_unit_aggregate_82b8f812f07e36ec, []int{7}
}
func (m *AreaCreatedEvent) XXX_Unmarshal(b []byte) error {
	return xxx_messageInfo_AreaCreatedEvent.Unmarshal(m, b)
}
func (m *AreaCreatedEvent) XXX_Marshal(b []byte, deterministic bool) ([]byte, error) {
	return xxx_messageInfo_AreaCreatedEvent.Marshal(b, m, deterministic)
}
func (dst *AreaCreatedEvent) XXX_Merge(src proto.Message) {
	xxx_messageInfo_AreaCreatedEvent.Merge(dst, src)
}
func (m *AreaCreatedEvent) XXX_Size() int {
	return xxx_messageInfo_AreaCreatedEvent.Size(m)
}
func (m *AreaCreatedEvent) XXX_DiscardUnknown() {
	xxx_messageInfo_AreaCreatedEvent.DiscardUnknown(m)
}

var xxx_messageInfo_AreaCreatedEvent proto.InternalMessageInfo

func (m *AreaCreatedEvent) GetUnitID() string {
	if m != nil {
		return m.UnitID
	}
	return ""
}

func (m *AreaCreatedEvent) GetRequesterID() string {
	if m != nil {
		return m.RequesterID
	}
	return ""
}

func (m *AreaCreatedEvent) GetAreaID() string {
	if m != nil {
		return m.AreaID
	}
	return ""
}

func (m *AreaCreatedEvent) GetName() string {
	if m != nil {
		return m.Name
	}
	return ""
}

func (m *AreaCreatedEvent) GetDescription() string {
	if m != nil {
		return m.Description
	}
	return ""
}

func init() {
	proto.RegisterType((*CreateUnitCommand)(nil), "messages.CreateUnitCommand")
	proto.RegisterType((*UnitCreatedEvent)(nil), "messages.UnitCreatedEvent")
	proto.RegisterType((*GrantPrivilegeCommand)(nil), "messages.GrantPrivilegeCommand")
	proto.RegisterType((*PrivilegeGrantedEvent)(nil), "messages.PrivilegeGrantedEvent")
	proto.RegisterType((*RevokePrivilegeCommand)(nil), "messages.RevokePrivilegeCommand")
	proto.RegisterType((*PrivilegeRevokedEvent)(nil), "messages.PrivilegeRevokedEvent")
	proto.RegisterType((*CreateAreaCommand)(nil), "messages.CreateAreaCommand")
	proto.RegisterType((*AreaCreatedEvent)(nil), "messages.AreaCreatedEvent")
}

func init() {
	proto.RegisterFile("messages/unit_aggregate.proto", fileDescriptor_unit_aggregate_82b8f812f07e36ec)
}

var fileDescriptor_unit_aggregate_82b8f812f07e36ec = []byte{
	// 401 bytes of a gzipped FileDescriptorProto
	0x1f, 0x8b, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0xff, 0xd4, 0x54, 0x41, 0x4f, 0xf2, 0x40,
	0x10, 0x4d, 0xf9, 0x4a, 0xbf, 0xba, 0xc6, 0x88, 0x4d, 0x24, 0x95, 0x68, 0x42, 0x38, 0x11, 0x0f,
	0x7a, 0xf0, 0xe6, 0xcd, 0x80, 0x31, 0x24, 0x62, 0x48, 0x13, 0xf4, 0x48, 0x16, 0x18, 0xd7, 0x8d,
	0xec, 0x16, 0xb7, 0x43, 0x0f, 0x1e, 0xbd, 0x78, 0xf6, 0xea, 0x0f, 0xd0, 0xff, 0xa0, 0x7f, 0xce,
	0x74, 0xb7, 0xd6, 0x02, 0xe1, 0x84, 0x98, 0x70, 0x63, 0xde, 0xbc, 0x59, 0xde, 0xbc, 0xe9, 0x0c,
	0x39, 0x10, 0x10, 0x45, 0x94, 0x41, 0x74, 0x3c, 0x91, 0x1c, 0x7b, 0x94, 0x31, 0x05, 0x8c, 0x22,
	0x1c, 0x8d, 0x55, 0x88, 0xa1, 0xe7, 0x7e, 0xa7, 0x2b, 0xfb, 0x19, 0x71, 0x30, 0x89, 0x30, 0x14,
	0xbd, 0x5b, 0x0e, 0xa3, 0x61, 0x64, 0x78, 0xb5, 0x4f, 0x8b, 0xec, 0x34, 0x14, 0x50, 0x84, 0xae,
	0xe4, 0xd8, 0x08, 0x85, 0xa0, 0x72, 0xe8, 0x95, 0x89, 0x93, 0x84, 0xad, 0xa6, 0x6f, 0x55, 0xad,
	0xfa, 0x46, 0x90, 0x46, 0x5e, 0x95, 0x6c, 0x06, 0xf0, 0x30, 0x81, 0x08, 0x41, 0xb5, 0x9a, 0x7e,
	0x41, 0x27, 0xf3, 0x90, 0xe7, 0x11, 0xfb, 0x8a, 0x0a, 0xf0, 0xff, 0xe9, 0x94, 0xfe, 0x9d, 0x54,
	0x35, 0x21, 0x1a, 0x28, 0x3e, 0x46, 0x1e, 0x4a, 0xdf, 0x36, 0x55, 0x39, 0xc8, 0x3b, 0x24, 0xa5,
	0x6e, 0x70, 0xd9, 0xa1, 0x8a, 0x0a, 0x40, 0x50, 0xfa, 0x85, 0xa2, 0xa6, 0xcd, 0xe1, 0xa7, 0xe4,
	0xf5, 0x69, 0x2f, 0xd5, 0x53, 0xfb, 0xb0, 0x48, 0x49, 0xeb, 0xd6, 0x1d, 0x0c, 0xcf, 0x63, 0x90,
	0xb8, 0x36, 0xe2, 0x9f, 0x0b, 0x64, 0xf7, 0x42, 0x51, 0x89, 0x1d, 0xc5, 0x63, 0x3e, 0x02, 0x06,
	0xcb, 0xdb, 0x5f, 0x21, 0x6e, 0x1b, 0x44, 0x5f, 0xa7, 0x4d, 0x17, 0x59, 0x9c, 0x54, 0x5f, 0x87,
	0xc8, 0x25, 0x0b, 0x38, 0xbb, 0x43, 0x2d, 0xd1, 0x0d, 0xf2, 0x90, 0x57, 0x27, 0xdb, 0x2d, 0xc9,
	0x91, 0x53, 0xe4, 0x31, 0x18, 0x96, 0xa3, 0x59, 0xb3, 0x70, 0xc2, 0x6c, 0x53, 0x49, 0x19, 0x08,
	0x90, 0x68, 0x98, 0xff, 0x0d, 0x73, 0x06, 0x4e, 0x7a, 0xb9, 0x01, 0x4d, 0x70, 0xab, 0x56, 0x7d,
	0x2b, 0x48, 0xa3, 0x39, 0x27, 0x32, 0x13, 0xb4, 0x25, 0xcb, 0xcf, 0x72, 0x3d, 0x9d, 0x78, 0x24,
	0xe5, 0x00, 0xe2, 0xf0, 0x1e, 0xfe, 0xe6, 0x9b, 0x98, 0xfa, 0xef, 0x17, 0x2b, 0x37, 0x05, 0xa3,
	0x62, 0xa5, 0x53, 0xf8, 0xf1, 0xc3, 0x5e, 0xe8, 0xc7, 0x7b, 0x76, 0x9e, 0xce, 0x14, 0xd0, 0xe5,
	0xbd, 0x28, 0x13, 0x27, 0x79, 0x28, 0x53, 0x93, 0x46, 0xd9, 0xe6, 0xdb, 0x8b, 0x37, 0xbf, 0x38,
	0xb7, 0xf9, 0x53, 0x4a, 0xdf, 0x2c, 0x52, 0xd2, 0x1a, 0x7f, 0xe7, 0x14, 0xad, 0x4c, 0x68, 0xdf,
	0xd1, 0x87, 0xff, 0xe4, 0x2b, 0x00, 0x00, 0xff, 0xff, 0x2d, 0x82, 0x50, 0xac, 0x41, 0x06, 0x00,
	0x00,
}
