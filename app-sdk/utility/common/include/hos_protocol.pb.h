// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: hos_protocol.proto

#ifndef PROTOBUF_hos_5fprotocol_2eproto__INCLUDED
#define PROTOBUF_hos_5fprotocol_2eproto__INCLUDED

#include <string>

#include <google/protobuf/stubs/common.h>

#if GOOGLE_PROTOBUF_VERSION < 3000000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please update
#error your headers.
#endif
#if 3000000 < GOOGLE_PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/arena.h>
#include <google/protobuf/arenastring.h>
#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/metadata.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>
#include <google/protobuf/extension_set.h>
#include <google/protobuf/generated_enum_reflection.h>
#include <google/protobuf/unknown_field_set.h>
// @@protoc_insertion_point(includes)

// Internal implementation detail -- do not call these.
void protobuf_AddDesc_hos_5fprotocol_2eproto();
void protobuf_AssignDesc_hos_5fprotocol_2eproto();
void protobuf_ShutdownFile_hos_5fprotocol_2eproto();

class ServerMessage;
class ClientMessage;

enum MessageType {
  Ping = 0,
  Pong = 1,
  Init = 2
};
bool MessageType_IsValid(int value);
const MessageType MessageType_MIN = Ping;
const MessageType MessageType_MAX = Init;
const int MessageType_ARRAYSIZE = MessageType_MAX + 1;

const ::google::protobuf::EnumDescriptor* MessageType_descriptor();
inline const ::std::string& MessageType_Name(MessageType value) {
  return ::google::protobuf::internal::NameOfEnum(
    MessageType_descriptor(), value);
}
inline bool MessageType_Parse(
    const ::std::string& name, MessageType* value) {
  return ::google::protobuf::internal::ParseNamedEnum<MessageType>(
    MessageType_descriptor(), name, value);
}
// ===================================================================

class ServerMessage : public ::google::protobuf::Message {
 public:
  ServerMessage();
  virtual ~ServerMessage();

  ServerMessage(const ServerMessage& from);

  inline ServerMessage& operator=(const ServerMessage& from) {
    CopyFrom(from);
    return *this;
  }

  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _internal_metadata_.unknown_fields();
  }

  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return _internal_metadata_.mutable_unknown_fields();
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const ServerMessage& default_instance();

  void Swap(ServerMessage* other);

  // implements Message ----------------------------------------------

  inline ServerMessage* New() const { return New(NULL); }

  ServerMessage* New(::google::protobuf::Arena* arena) const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const ServerMessage& from);
  void MergeFrom(const ServerMessage& from);
  void Clear();
  bool IsInitialized() const;

  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  void InternalSwap(ServerMessage* other);
  private:
  inline ::google::protobuf::Arena* GetArenaNoVirtual() const {
    return _internal_metadata_.arena();
  }
  inline void* MaybeArenaPtr() const {
    return _internal_metadata_.raw_arena_ptr();
  }
  public:

  ::google::protobuf::Metadata GetMetadata() const;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // required .MessageType type = 1;
  inline bool has_type() const;
  inline void clear_type();
  static const int kTypeFieldNumber = 1;
  inline ::MessageType type() const;
  inline void set_type(::MessageType value);

  // @@protoc_insertion_point(class_scope:ServerMessage)
 private:
  inline void set_has_type();
  inline void clear_has_type();

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  ::google::protobuf::uint32 _has_bits_[1];
  mutable int _cached_size_;
  int type_;
  friend void  protobuf_AddDesc_hos_5fprotocol_2eproto();
  friend void protobuf_AssignDesc_hos_5fprotocol_2eproto();
  friend void protobuf_ShutdownFile_hos_5fprotocol_2eproto();

  void InitAsDefaultInstance();
  static ServerMessage* default_instance_;
};
// -------------------------------------------------------------------

class ClientMessage : public ::google::protobuf::Message {
 public:
  ClientMessage();
  virtual ~ClientMessage();

  ClientMessage(const ClientMessage& from);

  inline ClientMessage& operator=(const ClientMessage& from) {
    CopyFrom(from);
    return *this;
  }

  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _internal_metadata_.unknown_fields();
  }

  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return _internal_metadata_.mutable_unknown_fields();
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const ClientMessage& default_instance();

  void Swap(ClientMessage* other);

  // implements Message ----------------------------------------------

  inline ClientMessage* New() const { return New(NULL); }

  ClientMessage* New(::google::protobuf::Arena* arena) const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const ClientMessage& from);
  void MergeFrom(const ClientMessage& from);
  void Clear();
  bool IsInitialized() const;

  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  void InternalSwap(ClientMessage* other);
  private:
  inline ::google::protobuf::Arena* GetArenaNoVirtual() const {
    return _internal_metadata_.arena();
  }
  inline void* MaybeArenaPtr() const {
    return _internal_metadata_.raw_arena_ptr();
  }
  public:

  ::google::protobuf::Metadata GetMetadata() const;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // required .MessageType type = 1;
  inline bool has_type() const;
  inline void clear_type();
  static const int kTypeFieldNumber = 1;
  inline ::MessageType type() const;
  inline void set_type(::MessageType value);

  // @@protoc_insertion_point(class_scope:ClientMessage)
 private:
  inline void set_has_type();
  inline void clear_has_type();

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  ::google::protobuf::uint32 _has_bits_[1];
  mutable int _cached_size_;
  int type_;
  friend void  protobuf_AddDesc_hos_5fprotocol_2eproto();
  friend void protobuf_AssignDesc_hos_5fprotocol_2eproto();
  friend void protobuf_ShutdownFile_hos_5fprotocol_2eproto();

  void InitAsDefaultInstance();
  static ClientMessage* default_instance_;
};
// ===================================================================


// ===================================================================

// ServerMessage

// required .MessageType type = 1;
inline bool ServerMessage::has_type() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void ServerMessage::set_has_type() {
  _has_bits_[0] |= 0x00000001u;
}
inline void ServerMessage::clear_has_type() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void ServerMessage::clear_type() {
  type_ = 0;
  clear_has_type();
}
inline ::MessageType ServerMessage::type() const {
  // @@protoc_insertion_point(field_get:ServerMessage.type)
  return static_cast< ::MessageType >(type_);
}
inline void ServerMessage::set_type(::MessageType value) {
  assert(::MessageType_IsValid(value));
  set_has_type();
  type_ = value;
  // @@protoc_insertion_point(field_set:ServerMessage.type)
}

// -------------------------------------------------------------------

// ClientMessage

// required .MessageType type = 1;
inline bool ClientMessage::has_type() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void ClientMessage::set_has_type() {
  _has_bits_[0] |= 0x00000001u;
}
inline void ClientMessage::clear_has_type() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void ClientMessage::clear_type() {
  type_ = 0;
  clear_has_type();
}
inline ::MessageType ClientMessage::type() const {
  // @@protoc_insertion_point(field_get:ClientMessage.type)
  return static_cast< ::MessageType >(type_);
}
inline void ClientMessage::set_type(::MessageType value) {
  assert(::MessageType_IsValid(value));
  set_has_type();
  type_ = value;
  // @@protoc_insertion_point(field_set:ClientMessage.type)
}


// @@protoc_insertion_point(namespace_scope)

#ifndef SWIG
namespace google {
namespace protobuf {

template <> struct is_proto_enum< ::MessageType> : ::google::protobuf::internal::true_type {};
template <>
inline const EnumDescriptor* GetEnumDescriptor< ::MessageType>() {
  return ::MessageType_descriptor();
}

}  // namespace protobuf
}  // namespace google
#endif  // SWIG

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_hos_5fprotocol_2eproto__INCLUDED
