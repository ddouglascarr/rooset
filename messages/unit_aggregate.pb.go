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
	return fileDescriptor_unit_aggregate_c6afba69f3d5e3c8, []int{0}
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
	return fileDescriptor_unit_aggregate_c6afba69f3d5e3c8, []int{1}
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
	return fileDescriptor_unit_aggregate_c6afba69f3d5e3c8, []int{2}
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
	return fileDescriptor_unit_aggregate_c6afba69f3d5e3c8, []int{3}
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
	return fileDescriptor_unit_aggregate_c6afba69f3d5e3c8, []int{4}
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
	return fileDescriptor_unit_aggregate_c6afba69f3d5e3c8, []int{5}
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
	return fileDescriptor_unit_aggregate_c6afba69f3d5e3c8, []int{6}
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
	return fileDescriptor_unit_aggregate_c6afba69f3d5e3c8, []int{7}
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

type CreatePolicyCommand struct {
	UnitID      string `protobuf:"bytes,1,opt,name=UnitID,proto3" json:"UnitID,omitempty"`
	RequesterID string `protobuf:"bytes,2,opt,name=RequesterID,proto3" json:"RequesterID,omitempty"`
	PolicyID    string `protobuf:"bytes,3,opt,name=PolicyID,proto3" json:"PolicyID,omitempty"`
	Description string `protobuf:"bytes,4,opt,name=Description,proto3" json:"Description,omitempty"`
	// Max / min amount of time before an issue is passed the Admision phase
	MinAdmissionDuration int32 `protobuf:"varint,5,opt,name=MinAdmissionDuration,proto3" json:"MinAdmissionDuration,omitempty"`
	MaxAdmissionDuration int32 `protobuf:"varint,6,opt,name=MaxAdmissionDuration,proto3" json:"MaxAdmissionDuration,omitempty"`
	// phase durations
	DiscussionDuration   int32 `protobuf:"varint,7,opt,name=DiscussionDuration,proto3" json:"DiscussionDuration,omitempty"`
	VerificationDuration int32 `protobuf:"varint,8,opt,name=VerificationDuration,proto3" json:"VerificationDuration,omitempty"`
	VotingDuration       int32 `protobuf:"varint,9,opt,name=VotingDuration,proto3" json:"VotingDuration,omitempty"`
	// quorums required (numerator / denominator)
	IssueQuorumNum       int32    `protobuf:"varint,10,opt,name=IssueQuorumNum,proto3" json:"IssueQuorumNum,omitempty"`
	IssueQuroumDen       int32    `protobuf:"varint,11,opt,name=IssueQuroumDen,proto3" json:"IssueQuroumDen,omitempty"`
	InitiativeQuorumNum  int32    `protobuf:"varint,12,opt,name=InitiativeQuorumNum,proto3" json:"InitiativeQuorumNum,omitempty"`
	InitiativeQuorumDen  int32    `protobuf:"varint,13,opt,name=InitiativeQuorumDen,proto3" json:"InitiativeQuorumDen,omitempty"`
	XXX_NoUnkeyedLiteral struct{} `json:"-"`
	XXX_unrecognized     []byte   `json:"-"`
	XXX_sizecache        int32    `json:"-"`
}

func (m *CreatePolicyCommand) Reset()         { *m = CreatePolicyCommand{} }
func (m *CreatePolicyCommand) String() string { return proto.CompactTextString(m) }
func (*CreatePolicyCommand) ProtoMessage()    {}
func (*CreatePolicyCommand) Descriptor() ([]byte, []int) {
	return fileDescriptor_unit_aggregate_c6afba69f3d5e3c8, []int{8}
}
func (m *CreatePolicyCommand) XXX_Unmarshal(b []byte) error {
	return xxx_messageInfo_CreatePolicyCommand.Unmarshal(m, b)
}
func (m *CreatePolicyCommand) XXX_Marshal(b []byte, deterministic bool) ([]byte, error) {
	return xxx_messageInfo_CreatePolicyCommand.Marshal(b, m, deterministic)
}
func (dst *CreatePolicyCommand) XXX_Merge(src proto.Message) {
	xxx_messageInfo_CreatePolicyCommand.Merge(dst, src)
}
func (m *CreatePolicyCommand) XXX_Size() int {
	return xxx_messageInfo_CreatePolicyCommand.Size(m)
}
func (m *CreatePolicyCommand) XXX_DiscardUnknown() {
	xxx_messageInfo_CreatePolicyCommand.DiscardUnknown(m)
}

var xxx_messageInfo_CreatePolicyCommand proto.InternalMessageInfo

func (m *CreatePolicyCommand) GetUnitID() string {
	if m != nil {
		return m.UnitID
	}
	return ""
}

func (m *CreatePolicyCommand) GetRequesterID() string {
	if m != nil {
		return m.RequesterID
	}
	return ""
}

func (m *CreatePolicyCommand) GetPolicyID() string {
	if m != nil {
		return m.PolicyID
	}
	return ""
}

func (m *CreatePolicyCommand) GetDescription() string {
	if m != nil {
		return m.Description
	}
	return ""
}

func (m *CreatePolicyCommand) GetMinAdmissionDuration() int32 {
	if m != nil {
		return m.MinAdmissionDuration
	}
	return 0
}

func (m *CreatePolicyCommand) GetMaxAdmissionDuration() int32 {
	if m != nil {
		return m.MaxAdmissionDuration
	}
	return 0
}

func (m *CreatePolicyCommand) GetDiscussionDuration() int32 {
	if m != nil {
		return m.DiscussionDuration
	}
	return 0
}

func (m *CreatePolicyCommand) GetVerificationDuration() int32 {
	if m != nil {
		return m.VerificationDuration
	}
	return 0
}

func (m *CreatePolicyCommand) GetVotingDuration() int32 {
	if m != nil {
		return m.VotingDuration
	}
	return 0
}

func (m *CreatePolicyCommand) GetIssueQuorumNum() int32 {
	if m != nil {
		return m.IssueQuorumNum
	}
	return 0
}

func (m *CreatePolicyCommand) GetIssueQuroumDen() int32 {
	if m != nil {
		return m.IssueQuroumDen
	}
	return 0
}

func (m *CreatePolicyCommand) GetInitiativeQuorumNum() int32 {
	if m != nil {
		return m.InitiativeQuorumNum
	}
	return 0
}

func (m *CreatePolicyCommand) GetInitiativeQuorumDen() int32 {
	if m != nil {
		return m.InitiativeQuorumDen
	}
	return 0
}

type PolicyCreatedEvent struct {
	UnitID      string `protobuf:"bytes,1,opt,name=UnitID,proto3" json:"UnitID,omitempty"`
	RequesterID string `protobuf:"bytes,2,opt,name=RequesterID,proto3" json:"RequesterID,omitempty"`
	PolicyID    string `protobuf:"bytes,3,opt,name=PolicyID,proto3" json:"PolicyID,omitempty"`
	Description string `protobuf:"bytes,4,opt,name=Description,proto3" json:"Description,omitempty"`
	// Max / min amount of time before an issue is passed the Admision phase
	MinAdmissionDuration int32 `protobuf:"varint,5,opt,name=MinAdmissionDuration,proto3" json:"MinAdmissionDuration,omitempty"`
	MaxAdmissionDuration int32 `protobuf:"varint,6,opt,name=MaxAdmissionDuration,proto3" json:"MaxAdmissionDuration,omitempty"`
	// phase durations
	DiscussionDuration   int32 `protobuf:"varint,7,opt,name=DiscussionDuration,proto3" json:"DiscussionDuration,omitempty"`
	VerificationDuration int32 `protobuf:"varint,8,opt,name=VerificationDuration,proto3" json:"VerificationDuration,omitempty"`
	VotingDuration       int32 `protobuf:"varint,9,opt,name=VotingDuration,proto3" json:"VotingDuration,omitempty"`
	// quorums required (numerator / denominator)
	IssueQuorumNum       int32    `protobuf:"varint,10,opt,name=IssueQuorumNum,proto3" json:"IssueQuorumNum,omitempty"`
	IssueQuroumDen       int32    `protobuf:"varint,11,opt,name=IssueQuroumDen,proto3" json:"IssueQuroumDen,omitempty"`
	InitiativeQuorumNum  int32    `protobuf:"varint,12,opt,name=InitiativeQuorumNum,proto3" json:"InitiativeQuorumNum,omitempty"`
	InitiativeQuorumDen  int32    `protobuf:"varint,13,opt,name=InitiativeQuorumDen,proto3" json:"InitiativeQuorumDen,omitempty"`
	XXX_NoUnkeyedLiteral struct{} `json:"-"`
	XXX_unrecognized     []byte   `json:"-"`
	XXX_sizecache        int32    `json:"-"`
}

func (m *PolicyCreatedEvent) Reset()         { *m = PolicyCreatedEvent{} }
func (m *PolicyCreatedEvent) String() string { return proto.CompactTextString(m) }
func (*PolicyCreatedEvent) ProtoMessage()    {}
func (*PolicyCreatedEvent) Descriptor() ([]byte, []int) {
	return fileDescriptor_unit_aggregate_c6afba69f3d5e3c8, []int{9}
}
func (m *PolicyCreatedEvent) XXX_Unmarshal(b []byte) error {
	return xxx_messageInfo_PolicyCreatedEvent.Unmarshal(m, b)
}
func (m *PolicyCreatedEvent) XXX_Marshal(b []byte, deterministic bool) ([]byte, error) {
	return xxx_messageInfo_PolicyCreatedEvent.Marshal(b, m, deterministic)
}
func (dst *PolicyCreatedEvent) XXX_Merge(src proto.Message) {
	xxx_messageInfo_PolicyCreatedEvent.Merge(dst, src)
}
func (m *PolicyCreatedEvent) XXX_Size() int {
	return xxx_messageInfo_PolicyCreatedEvent.Size(m)
}
func (m *PolicyCreatedEvent) XXX_DiscardUnknown() {
	xxx_messageInfo_PolicyCreatedEvent.DiscardUnknown(m)
}

var xxx_messageInfo_PolicyCreatedEvent proto.InternalMessageInfo

func (m *PolicyCreatedEvent) GetUnitID() string {
	if m != nil {
		return m.UnitID
	}
	return ""
}

func (m *PolicyCreatedEvent) GetRequesterID() string {
	if m != nil {
		return m.RequesterID
	}
	return ""
}

func (m *PolicyCreatedEvent) GetPolicyID() string {
	if m != nil {
		return m.PolicyID
	}
	return ""
}

func (m *PolicyCreatedEvent) GetDescription() string {
	if m != nil {
		return m.Description
	}
	return ""
}

func (m *PolicyCreatedEvent) GetMinAdmissionDuration() int32 {
	if m != nil {
		return m.MinAdmissionDuration
	}
	return 0
}

func (m *PolicyCreatedEvent) GetMaxAdmissionDuration() int32 {
	if m != nil {
		return m.MaxAdmissionDuration
	}
	return 0
}

func (m *PolicyCreatedEvent) GetDiscussionDuration() int32 {
	if m != nil {
		return m.DiscussionDuration
	}
	return 0
}

func (m *PolicyCreatedEvent) GetVerificationDuration() int32 {
	if m != nil {
		return m.VerificationDuration
	}
	return 0
}

func (m *PolicyCreatedEvent) GetVotingDuration() int32 {
	if m != nil {
		return m.VotingDuration
	}
	return 0
}

func (m *PolicyCreatedEvent) GetIssueQuorumNum() int32 {
	if m != nil {
		return m.IssueQuorumNum
	}
	return 0
}

func (m *PolicyCreatedEvent) GetIssueQuroumDen() int32 {
	if m != nil {
		return m.IssueQuroumDen
	}
	return 0
}

func (m *PolicyCreatedEvent) GetInitiativeQuorumNum() int32 {
	if m != nil {
		return m.InitiativeQuorumNum
	}
	return 0
}

func (m *PolicyCreatedEvent) GetInitiativeQuorumDen() int32 {
	if m != nil {
		return m.InitiativeQuorumDen
	}
	return 0
}

type DeactivatePolicyCommand struct {
	UnitID               string   `protobuf:"bytes,1,opt,name=UnitID,proto3" json:"UnitID,omitempty"`
	RequesterID          string   `protobuf:"bytes,2,opt,name=RequesterID,proto3" json:"RequesterID,omitempty"`
	PolicyID             string   `protobuf:"bytes,3,opt,name=PolicyID,proto3" json:"PolicyID,omitempty"`
	XXX_NoUnkeyedLiteral struct{} `json:"-"`
	XXX_unrecognized     []byte   `json:"-"`
	XXX_sizecache        int32    `json:"-"`
}

func (m *DeactivatePolicyCommand) Reset()         { *m = DeactivatePolicyCommand{} }
func (m *DeactivatePolicyCommand) String() string { return proto.CompactTextString(m) }
func (*DeactivatePolicyCommand) ProtoMessage()    {}
func (*DeactivatePolicyCommand) Descriptor() ([]byte, []int) {
	return fileDescriptor_unit_aggregate_c6afba69f3d5e3c8, []int{10}
}
func (m *DeactivatePolicyCommand) XXX_Unmarshal(b []byte) error {
	return xxx_messageInfo_DeactivatePolicyCommand.Unmarshal(m, b)
}
func (m *DeactivatePolicyCommand) XXX_Marshal(b []byte, deterministic bool) ([]byte, error) {
	return xxx_messageInfo_DeactivatePolicyCommand.Marshal(b, m, deterministic)
}
func (dst *DeactivatePolicyCommand) XXX_Merge(src proto.Message) {
	xxx_messageInfo_DeactivatePolicyCommand.Merge(dst, src)
}
func (m *DeactivatePolicyCommand) XXX_Size() int {
	return xxx_messageInfo_DeactivatePolicyCommand.Size(m)
}
func (m *DeactivatePolicyCommand) XXX_DiscardUnknown() {
	xxx_messageInfo_DeactivatePolicyCommand.DiscardUnknown(m)
}

var xxx_messageInfo_DeactivatePolicyCommand proto.InternalMessageInfo

func (m *DeactivatePolicyCommand) GetUnitID() string {
	if m != nil {
		return m.UnitID
	}
	return ""
}

func (m *DeactivatePolicyCommand) GetRequesterID() string {
	if m != nil {
		return m.RequesterID
	}
	return ""
}

func (m *DeactivatePolicyCommand) GetPolicyID() string {
	if m != nil {
		return m.PolicyID
	}
	return ""
}

type PolicyDeactivatedEvent struct {
	UnitID               string   `protobuf:"bytes,1,opt,name=UnitID,proto3" json:"UnitID,omitempty"`
	RequesterID          string   `protobuf:"bytes,2,opt,name=RequesterID,proto3" json:"RequesterID,omitempty"`
	PolicyID             string   `protobuf:"bytes,3,opt,name=PolicyID,proto3" json:"PolicyID,omitempty"`
	XXX_NoUnkeyedLiteral struct{} `json:"-"`
	XXX_unrecognized     []byte   `json:"-"`
	XXX_sizecache        int32    `json:"-"`
}

func (m *PolicyDeactivatedEvent) Reset()         { *m = PolicyDeactivatedEvent{} }
func (m *PolicyDeactivatedEvent) String() string { return proto.CompactTextString(m) }
func (*PolicyDeactivatedEvent) ProtoMessage()    {}
func (*PolicyDeactivatedEvent) Descriptor() ([]byte, []int) {
	return fileDescriptor_unit_aggregate_c6afba69f3d5e3c8, []int{11}
}
func (m *PolicyDeactivatedEvent) XXX_Unmarshal(b []byte) error {
	return xxx_messageInfo_PolicyDeactivatedEvent.Unmarshal(m, b)
}
func (m *PolicyDeactivatedEvent) XXX_Marshal(b []byte, deterministic bool) ([]byte, error) {
	return xxx_messageInfo_PolicyDeactivatedEvent.Marshal(b, m, deterministic)
}
func (dst *PolicyDeactivatedEvent) XXX_Merge(src proto.Message) {
	xxx_messageInfo_PolicyDeactivatedEvent.Merge(dst, src)
}
func (m *PolicyDeactivatedEvent) XXX_Size() int {
	return xxx_messageInfo_PolicyDeactivatedEvent.Size(m)
}
func (m *PolicyDeactivatedEvent) XXX_DiscardUnknown() {
	xxx_messageInfo_PolicyDeactivatedEvent.DiscardUnknown(m)
}

var xxx_messageInfo_PolicyDeactivatedEvent proto.InternalMessageInfo

func (m *PolicyDeactivatedEvent) GetUnitID() string {
	if m != nil {
		return m.UnitID
	}
	return ""
}

func (m *PolicyDeactivatedEvent) GetRequesterID() string {
	if m != nil {
		return m.RequesterID
	}
	return ""
}

func (m *PolicyDeactivatedEvent) GetPolicyID() string {
	if m != nil {
		return m.PolicyID
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
	proto.RegisterType((*CreatePolicyCommand)(nil), "messages.CreatePolicyCommand")
	proto.RegisterType((*PolicyCreatedEvent)(nil), "messages.PolicyCreatedEvent")
	proto.RegisterType((*DeactivatePolicyCommand)(nil), "messages.DeactivatePolicyCommand")
	proto.RegisterType((*PolicyDeactivatedEvent)(nil), "messages.PolicyDeactivatedEvent")
}

func init() {
	proto.RegisterFile("messages/unit_aggregate.proto", fileDescriptor_unit_aggregate_c6afba69f3d5e3c8)
}

var fileDescriptor_unit_aggregate_c6afba69f3d5e3c8 = []byte{
	// 593 bytes of a gzipped FileDescriptorProto
	0x1f, 0x8b, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0xff, 0xec, 0x56, 0xcf, 0x6f, 0xd3, 0x30,
	0x14, 0x56, 0x46, 0xd6, 0x75, 0x6f, 0x0c, 0x86, 0x07, 0x25, 0x4c, 0x20, 0x55, 0x3b, 0xa0, 0x8a,
	0xc3, 0x40, 0x70, 0xe3, 0x36, 0x2d, 0x08, 0x55, 0xa2, 0x53, 0x89, 0xb4, 0x71, 0x9c, 0xbc, 0xf4,
	0x2d, 0x58, 0xd4, 0xf6, 0xb0, 0x9d, 0x8a, 0x1f, 0x37, 0x2e, 0x9c, 0xb9, 0xf2, 0x07, 0xc0, 0xff,
	0x00, 0x17, 0xf8, 0xcf, 0x50, 0xec, 0x2c, 0x4d, 0xda, 0x4c, 0x42, 0x6a, 0x3b, 0x69, 0x82, 0x5b,
	0xdf, 0xf7, 0xbe, 0xcf, 0xfe, 0xfc, 0xfc, 0xfc, 0x1a, 0xb8, 0xc7, 0x51, 0x6b, 0x9a, 0xa0, 0x7e,
	0x98, 0x0a, 0x66, 0x8e, 0x68, 0x92, 0x28, 0x4c, 0xa8, 0xc1, 0x9d, 0x53, 0x25, 0x8d, 0x24, 0xcd,
	0xb3, 0xf4, 0xd6, 0xdd, 0x82, 0x18, 0xa7, 0xda, 0x48, 0x7e, 0x74, 0xc2, 0x70, 0x38, 0xd0, 0x8e,
	0xb7, 0xfd, 0xd3, 0x83, 0x1b, 0x7b, 0x0a, 0xa9, 0xc1, 0x03, 0xc1, 0xcc, 0x9e, 0xe4, 0x9c, 0x8a,
	0x01, 0x69, 0x41, 0x23, 0x0b, 0xbb, 0x61, 0xe0, 0xb5, 0xbd, 0xce, 0x6a, 0x94, 0x47, 0xa4, 0x0d,
	0x6b, 0x11, 0xbe, 0x4d, 0x51, 0x1b, 0x54, 0xdd, 0x30, 0x58, 0xb2, 0xc9, 0x32, 0x44, 0x08, 0xf8,
	0xfb, 0x94, 0x63, 0x70, 0xc5, 0xa6, 0xec, 0xef, 0x4c, 0x15, 0xa2, 0x8e, 0x15, 0x3b, 0x35, 0x4c,
	0x8a, 0xc0, 0x77, 0xaa, 0x12, 0x44, 0x1e, 0xc0, 0xc6, 0x41, 0xf4, 0xa2, 0x4f, 0x15, 0xe5, 0x68,
	0x50, 0xd9, 0x15, 0x96, 0x2d, 0x6d, 0x0a, 0x7f, 0x0a, 0x5f, 0x3f, 0xdd, 0xc9, 0xfd, 0x6c, 0xff,
	0xf0, 0x60, 0xc3, 0xfa, 0xb6, 0x27, 0x18, 0x3c, 0x1b, 0xa1, 0x30, 0x97, 0xc6, 0xfc, 0xe7, 0x25,
	0xb8, 0xf5, 0x5c, 0x51, 0x61, 0xfa, 0x8a, 0x8d, 0xd8, 0x10, 0x13, 0x9c, 0xbd, 0xfc, 0x5b, 0xd0,
	0xec, 0x21, 0x3f, 0xb6, 0x69, 0x77, 0x8a, 0x22, 0xce, 0xd4, 0x87, 0xd2, 0x30, 0x91, 0x44, 0x2c,
	0x79, 0x6d, 0xac, 0xc5, 0x66, 0x54, 0x86, 0x48, 0x07, 0xae, 0x77, 0x05, 0x33, 0x8c, 0x1a, 0x36,
	0x42, 0xc7, 0x6a, 0x58, 0xd6, 0x24, 0x9c, 0x31, 0x7b, 0x54, 0xd0, 0x04, 0x39, 0x0a, 0xe3, 0x98,
	0x2b, 0x8e, 0x39, 0x01, 0x67, 0x67, 0x79, 0x85, 0x96, 0xd0, 0x6c, 0x7b, 0x9d, 0xf5, 0x28, 0x8f,
	0xa6, 0x2a, 0x51, 0x14, 0xc1, 0x96, 0x64, 0xf6, 0xbb, 0xbc, 0x9c, 0x95, 0xf8, 0x00, 0xad, 0x08,
	0x47, 0xf2, 0x0d, 0x5e, 0x4c, 0x4f, 0x54, 0xf6, 0xfe, 0xe2, 0x95, 0x6e, 0xc1, 0xb9, 0x58, 0xe8,
	0x2d, 0x8c, 0xeb, 0xe1, 0x9f, 0x5b, 0x8f, 0xef, 0xc5, 0x78, 0xda, 0x55, 0x48, 0x67, 0xaf, 0x45,
	0x0b, 0x1a, 0xd9, 0x42, 0x85, 0x9b, 0x3c, 0x2a, 0x5e, 0xbe, 0x7f, 0xfe, 0xcb, 0x5f, 0x9e, 0x7a,
	0xf9, 0x15, 0xa7, 0xdf, 0x3c, 0xd8, 0xb0, 0x1e, 0xe7, 0x33, 0x8a, 0x16, 0x67, 0xf4, 0xb7, 0x0f,
	0x9b, 0xce, 0x64, 0x5f, 0x0e, 0x59, 0xfc, 0x7e, 0x2e, 0x0d, 0xe6, 0x96, 0x1a, 0x5f, 0xf2, 0x59,
	0xfc, 0x17, 0xe3, 0xf3, 0x31, 0xdc, 0xec, 0x31, 0xb1, 0x3b, 0xe0, 0x4c, 0x6b, 0x26, 0x45, 0x98,
	0x2a, 0x5a, 0x1c, 0x63, 0x39, 0xaa, 0xcd, 0x59, 0x0d, 0x7d, 0x37, 0xad, 0x69, 0xe4, 0x9a, 0x9a,
	0x1c, 0xd9, 0x01, 0x12, 0x32, 0x1d, 0xa7, 0x55, 0xc5, 0x8a, 0x55, 0xd4, 0x64, 0xb2, 0x3d, 0x0e,
	0x51, 0xb1, 0x13, 0x16, 0xdb, 0xb8, 0x50, 0x34, 0xdd, 0x1e, 0x75, 0x39, 0x72, 0x1f, 0xae, 0xb9,
	0x29, 0x52, 0xb0, 0x57, 0x2d, 0x7b, 0x02, 0xcd, 0x78, 0x5d, 0xad, 0x53, 0x7c, 0x99, 0x4a, 0x95,
	0xf2, 0xfd, 0x94, 0x07, 0xe0, 0x78, 0x55, 0xb4, 0xc4, 0x53, 0x32, 0xe5, 0x21, 0x8a, 0x60, 0xad,
	0xc2, 0xcb, 0x51, 0xf2, 0x08, 0x36, 0xc7, 0x73, 0x69, 0xbc, 0xe8, 0x55, 0x4b, 0xae, 0x4b, 0xd5,
	0x29, 0xb2, 0xe5, 0xd7, 0xeb, 0x15, 0x21, 0x56, 0x7b, 0xe8, 0x97, 0x0f, 0x24, 0xef, 0x9e, 0xf9,
	0xb4, 0xfb, 0xff, 0x16, 0xfa, 0xd7, 0x5a, 0xe8, 0x23, 0xdc, 0x0e, 0x91, 0xc6, 0x86, 0x8d, 0x2e,
	0x66, 0x12, 0x4d, 0xfe, 0xcd, 0x3a, 0x7c, 0x6c, 0x61, 0x91, 0x2d, 0x5c, 0xde, 0xfb, 0xb8, 0x61,
	0x3f, 0xbc, 0x9f, 0xfc, 0x09, 0x00, 0x00, 0xff, 0xff, 0x77, 0x14, 0x9a, 0x32, 0xc1, 0x0b, 0x00,
	0x00,
}