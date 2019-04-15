// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: ntwrkCmd.proto

#ifndef PROTOBUF_INCLUDED_ntwrkCmd_2eproto
#define PROTOBUF_INCLUDED_ntwrkCmd_2eproto

#include <limits>
#include <string>

#include <google/protobuf/port_def.inc>
#if PROTOBUF_VERSION < 3007000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers. Please update
#error your headers.
#endif
#if 3007000 < PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers. Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/port_undef.inc>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/arena.h>
#include <google/protobuf/arenastring.h>
#include <google/protobuf/generated_message_table_driven.h>
#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/inlined_string_field.h>
#include <google/protobuf/metadata.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>  // IWYU pragma: export
#include <google/protobuf/extension_set.h>  // IWYU pragma: export
#include <google/protobuf/generated_enum_reflection.h>
#include <google/protobuf/unknown_field_set.h>
// @@protoc_insertion_point(includes)
#include <google/protobuf/port_def.inc>
#define PROTOBUF_INTERNAL_EXPORT_ntwrkCmd_2eproto

// Internal implementation detail -- do not use these members.
struct TableStruct_ntwrkCmd_2eproto {
  static const ::google::protobuf::internal::ParseTableField entries[]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::google::protobuf::internal::AuxillaryParseTableField aux[]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::google::protobuf::internal::ParseTable schema[2]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::google::protobuf::internal::FieldMetadata field_metadata[];
  static const ::google::protobuf::internal::SerializationTable serialization_table[];
  static const ::google::protobuf::uint32 offsets[];
};
void AddDescriptors_ntwrkCmd_2eproto();
class inGameCmd;
class inGameCmdDefaultTypeInternal;
extern inGameCmdDefaultTypeInternal _inGameCmd_default_instance_;
class preGameCmd;
class preGameCmdDefaultTypeInternal;
extern preGameCmdDefaultTypeInternal _preGameCmd_default_instance_;
namespace google {
namespace protobuf {
template<> ::inGameCmd* Arena::CreateMaybeMessage<::inGameCmd>(Arena*);
template<> ::preGameCmd* Arena::CreateMaybeMessage<::preGameCmd>(Arena*);
}  // namespace protobuf
}  // namespace google

enum preGameCmd_preGameCmdTypes {
  preGameCmd_preGameCmdTypes_PREGAME_NW_CMD_NONE = 0,
  preGameCmd_preGameCmdTypes_PREGAME_NW_CMD_PLAYER_CONNECTED = 1,
  preGameCmd_preGameCmdTypes_PREGAME_NW_CMD_PLAYER_DISCONNECTED = 2,
  preGameCmd_preGameCmdTypes_PREGAME_NW_CMD_PLAYER_READY = 3,
  preGameCmd_preGameCmdTypes_PREGAME_NW_CMD_START_GAME = 4
};
bool preGameCmd_preGameCmdTypes_IsValid(int value);
const preGameCmd_preGameCmdTypes preGameCmd_preGameCmdTypes_preGameCmdTypes_MIN = preGameCmd_preGameCmdTypes_PREGAME_NW_CMD_NONE;
const preGameCmd_preGameCmdTypes preGameCmd_preGameCmdTypes_preGameCmdTypes_MAX = preGameCmd_preGameCmdTypes_PREGAME_NW_CMD_START_GAME;
const int preGameCmd_preGameCmdTypes_preGameCmdTypes_ARRAYSIZE = preGameCmd_preGameCmdTypes_preGameCmdTypes_MAX + 1;

const ::google::protobuf::EnumDescriptor* preGameCmd_preGameCmdTypes_descriptor();
inline const ::std::string& preGameCmd_preGameCmdTypes_Name(preGameCmd_preGameCmdTypes value) {
  return ::google::protobuf::internal::NameOfEnum(
    preGameCmd_preGameCmdTypes_descriptor(), value);
}
inline bool preGameCmd_preGameCmdTypes_Parse(
    const ::std::string& name, preGameCmd_preGameCmdTypes* value) {
  return ::google::protobuf::internal::ParseNamedEnum<preGameCmd_preGameCmdTypes>(
    preGameCmd_preGameCmdTypes_descriptor(), name, value);
}
enum inGameCmd_inGameCmdTypes {
  inGameCmd_inGameCmdTypes_INGAME_NW_CMD_NONE = 0,
  inGameCmd_inGameCmdTypes_INGAME_NW_CMD_PLAYER_RECONNECTED = 1,
  inGameCmd_inGameCmdTypes_INGAME_NW_CMD_PLAYER_DISCONNECTED = 2,
  inGameCmd_inGameCmdTypes_INGAME_NW_CMD_NEXT_TURN = 3,
  inGameCmd_inGameCmdTypes_INGAME_NW_CMD_MOVE_UNIT = 4,
  inGameCmd_inGameCmdTypes_INGAME_NW_CMD_ATTACK_UNIT = 5,
  inGameCmd_inGameCmdTypes_INGAME_NW_CMD_PLACE_UNIT = 6,
  inGameCmd_inGameCmdTypes_INGAME_NW_CMD_REMOVE_UNIT = 7
};
bool inGameCmd_inGameCmdTypes_IsValid(int value);
const inGameCmd_inGameCmdTypes inGameCmd_inGameCmdTypes_inGameCmdTypes_MIN = inGameCmd_inGameCmdTypes_INGAME_NW_CMD_NONE;
const inGameCmd_inGameCmdTypes inGameCmd_inGameCmdTypes_inGameCmdTypes_MAX = inGameCmd_inGameCmdTypes_INGAME_NW_CMD_REMOVE_UNIT;
const int inGameCmd_inGameCmdTypes_inGameCmdTypes_ARRAYSIZE = inGameCmd_inGameCmdTypes_inGameCmdTypes_MAX + 1;

const ::google::protobuf::EnumDescriptor* inGameCmd_inGameCmdTypes_descriptor();
inline const ::std::string& inGameCmd_inGameCmdTypes_Name(inGameCmd_inGameCmdTypes value) {
  return ::google::protobuf::internal::NameOfEnum(
    inGameCmd_inGameCmdTypes_descriptor(), value);
}
inline bool inGameCmd_inGameCmdTypes_Parse(
    const ::std::string& name, inGameCmd_inGameCmdTypes* value) {
  return ::google::protobuf::internal::ParseNamedEnum<inGameCmd_inGameCmdTypes>(
    inGameCmd_inGameCmdTypes_descriptor(), name, value);
}
// ===================================================================

class preGameCmd final :
    public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:preGameCmd) */ {
 public:
  preGameCmd();
  virtual ~preGameCmd();

  preGameCmd(const preGameCmd& from);

  inline preGameCmd& operator=(const preGameCmd& from) {
    CopyFrom(from);
    return *this;
  }
  #if LANG_CXX11
  preGameCmd(preGameCmd&& from) noexcept
    : preGameCmd() {
    *this = ::std::move(from);
  }

  inline preGameCmd& operator=(preGameCmd&& from) noexcept {
    if (GetArenaNoVirtual() == from.GetArenaNoVirtual()) {
      if (this != &from) InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }
  #endif
  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _internal_metadata_.unknown_fields();
  }
  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return _internal_metadata_.mutable_unknown_fields();
  }

  static const ::google::protobuf::Descriptor* descriptor() {
    return default_instance().GetDescriptor();
  }
  static const preGameCmd& default_instance();

  static void InitAsDefaultInstance();  // FOR INTERNAL USE ONLY
  static inline const preGameCmd* internal_default_instance() {
    return reinterpret_cast<const preGameCmd*>(
               &_preGameCmd_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    0;

  void Swap(preGameCmd* other);
  friend void swap(preGameCmd& a, preGameCmd& b) {
    a.Swap(&b);
  }

  // implements Message ----------------------------------------------

  inline preGameCmd* New() const final {
    return CreateMaybeMessage<preGameCmd>(nullptr);
  }

  preGameCmd* New(::google::protobuf::Arena* arena) const final {
    return CreateMaybeMessage<preGameCmd>(arena);
  }
  void CopyFrom(const ::google::protobuf::Message& from) final;
  void MergeFrom(const ::google::protobuf::Message& from) final;
  void CopyFrom(const preGameCmd& from);
  void MergeFrom(const preGameCmd& from);
  PROTOBUF_ATTRIBUTE_REINITIALIZES void Clear() final;
  bool IsInitialized() const final;

  size_t ByteSizeLong() const final;
  #if GOOGLE_PROTOBUF_ENABLE_EXPERIMENTAL_PARSER
  static const char* _InternalParse(const char* begin, const char* end, void* object, ::google::protobuf::internal::ParseContext* ctx);
  ::google::protobuf::internal::ParseFunc _ParseFunc() const final { return _InternalParse; }
  #else
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input) final;
  #endif  // GOOGLE_PROTOBUF_ENABLE_EXPERIMENTAL_PARSER
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const final;
  ::google::protobuf::uint8* InternalSerializeWithCachedSizesToArray(
      ::google::protobuf::uint8* target) const final;
  int GetCachedSize() const final { return _cached_size_.Get(); }

  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const final;
  void InternalSwap(preGameCmd* other);
  private:
  inline ::google::protobuf::Arena* GetArenaNoVirtual() const {
    return nullptr;
  }
  inline void* MaybeArenaPtr() const {
    return nullptr;
  }
  public:

  ::google::protobuf::Metadata GetMetadata() const final;

  // nested types ----------------------------------------------------

  typedef preGameCmd_preGameCmdTypes preGameCmdTypes;
  static const preGameCmdTypes PREGAME_NW_CMD_NONE =
    preGameCmd_preGameCmdTypes_PREGAME_NW_CMD_NONE;
  static const preGameCmdTypes PREGAME_NW_CMD_PLAYER_CONNECTED =
    preGameCmd_preGameCmdTypes_PREGAME_NW_CMD_PLAYER_CONNECTED;
  static const preGameCmdTypes PREGAME_NW_CMD_PLAYER_DISCONNECTED =
    preGameCmd_preGameCmdTypes_PREGAME_NW_CMD_PLAYER_DISCONNECTED;
  static const preGameCmdTypes PREGAME_NW_CMD_PLAYER_READY =
    preGameCmd_preGameCmdTypes_PREGAME_NW_CMD_PLAYER_READY;
  static const preGameCmdTypes PREGAME_NW_CMD_START_GAME =
    preGameCmd_preGameCmdTypes_PREGAME_NW_CMD_START_GAME;
  static inline bool preGameCmdTypes_IsValid(int value) {
    return preGameCmd_preGameCmdTypes_IsValid(value);
  }
  static const preGameCmdTypes preGameCmdTypes_MIN =
    preGameCmd_preGameCmdTypes_preGameCmdTypes_MIN;
  static const preGameCmdTypes preGameCmdTypes_MAX =
    preGameCmd_preGameCmdTypes_preGameCmdTypes_MAX;
  static const int preGameCmdTypes_ARRAYSIZE =
    preGameCmd_preGameCmdTypes_preGameCmdTypes_ARRAYSIZE;
  static inline const ::google::protobuf::EnumDescriptor*
  preGameCmdTypes_descriptor() {
    return preGameCmd_preGameCmdTypes_descriptor();
  }
  static inline const ::std::string& preGameCmdTypes_Name(preGameCmdTypes value) {
    return preGameCmd_preGameCmdTypes_Name(value);
  }
  static inline bool preGameCmdTypes_Parse(const ::std::string& name,
      preGameCmdTypes* value) {
    return preGameCmd_preGameCmdTypes_Parse(name, value);
  }

  // accessors -------------------------------------------------------

  // optional string name = 2;
  bool has_name() const;
  void clear_name();
  static const int kNameFieldNumber = 2;
  const ::std::string& name() const;
  void set_name(const ::std::string& value);
  #if LANG_CXX11
  void set_name(::std::string&& value);
  #endif
  void set_name(const char* value);
  void set_name(const char* value, size_t size);
  ::std::string* mutable_name();
  ::std::string* release_name();
  void set_allocated_name(::std::string* name);

  // required fixed32 type = 1 [default = 0];
  bool has_type() const;
  void clear_type();
  static const int kTypeFieldNumber = 1;
  ::google::protobuf::uint32 type() const;
  void set_type(::google::protobuf::uint32 value);

  // optional fixed32 indexNum = 3;
  bool has_indexnum() const;
  void clear_indexnum();
  static const int kIndexNumFieldNumber = 3;
  ::google::protobuf::uint32 indexnum() const;
  void set_indexnum(::google::protobuf::uint32 value);

  // @@protoc_insertion_point(class_scope:preGameCmd)
 private:
  class HasBitSetters;

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  ::google::protobuf::internal::HasBits<1> _has_bits_;
  mutable ::google::protobuf::internal::CachedSize _cached_size_;
  ::google::protobuf::internal::ArenaStringPtr name_;
  ::google::protobuf::uint32 type_;
  ::google::protobuf::uint32 indexnum_;
  friend struct ::TableStruct_ntwrkCmd_2eproto;
};
// -------------------------------------------------------------------

class inGameCmd final :
    public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:inGameCmd) */ {
 public:
  inGameCmd();
  virtual ~inGameCmd();

  inGameCmd(const inGameCmd& from);

  inline inGameCmd& operator=(const inGameCmd& from) {
    CopyFrom(from);
    return *this;
  }
  #if LANG_CXX11
  inGameCmd(inGameCmd&& from) noexcept
    : inGameCmd() {
    *this = ::std::move(from);
  }

  inline inGameCmd& operator=(inGameCmd&& from) noexcept {
    if (GetArenaNoVirtual() == from.GetArenaNoVirtual()) {
      if (this != &from) InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }
  #endif
  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _internal_metadata_.unknown_fields();
  }
  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return _internal_metadata_.mutable_unknown_fields();
  }

  static const ::google::protobuf::Descriptor* descriptor() {
    return default_instance().GetDescriptor();
  }
  static const inGameCmd& default_instance();

  static void InitAsDefaultInstance();  // FOR INTERNAL USE ONLY
  static inline const inGameCmd* internal_default_instance() {
    return reinterpret_cast<const inGameCmd*>(
               &_inGameCmd_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    1;

  void Swap(inGameCmd* other);
  friend void swap(inGameCmd& a, inGameCmd& b) {
    a.Swap(&b);
  }

  // implements Message ----------------------------------------------

  inline inGameCmd* New() const final {
    return CreateMaybeMessage<inGameCmd>(nullptr);
  }

  inGameCmd* New(::google::protobuf::Arena* arena) const final {
    return CreateMaybeMessage<inGameCmd>(arena);
  }
  void CopyFrom(const ::google::protobuf::Message& from) final;
  void MergeFrom(const ::google::protobuf::Message& from) final;
  void CopyFrom(const inGameCmd& from);
  void MergeFrom(const inGameCmd& from);
  PROTOBUF_ATTRIBUTE_REINITIALIZES void Clear() final;
  bool IsInitialized() const final;

  size_t ByteSizeLong() const final;
  #if GOOGLE_PROTOBUF_ENABLE_EXPERIMENTAL_PARSER
  static const char* _InternalParse(const char* begin, const char* end, void* object, ::google::protobuf::internal::ParseContext* ctx);
  ::google::protobuf::internal::ParseFunc _ParseFunc() const final { return _InternalParse; }
  #else
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input) final;
  #endif  // GOOGLE_PROTOBUF_ENABLE_EXPERIMENTAL_PARSER
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const final;
  ::google::protobuf::uint8* InternalSerializeWithCachedSizesToArray(
      ::google::protobuf::uint8* target) const final;
  int GetCachedSize() const final { return _cached_size_.Get(); }

  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const final;
  void InternalSwap(inGameCmd* other);
  private:
  inline ::google::protobuf::Arena* GetArenaNoVirtual() const {
    return nullptr;
  }
  inline void* MaybeArenaPtr() const {
    return nullptr;
  }
  public:

  ::google::protobuf::Metadata GetMetadata() const final;

  // nested types ----------------------------------------------------

  typedef inGameCmd_inGameCmdTypes inGameCmdTypes;
  static const inGameCmdTypes INGAME_NW_CMD_NONE =
    inGameCmd_inGameCmdTypes_INGAME_NW_CMD_NONE;
  static const inGameCmdTypes INGAME_NW_CMD_PLAYER_RECONNECTED =
    inGameCmd_inGameCmdTypes_INGAME_NW_CMD_PLAYER_RECONNECTED;
  static const inGameCmdTypes INGAME_NW_CMD_PLAYER_DISCONNECTED =
    inGameCmd_inGameCmdTypes_INGAME_NW_CMD_PLAYER_DISCONNECTED;
  static const inGameCmdTypes INGAME_NW_CMD_NEXT_TURN =
    inGameCmd_inGameCmdTypes_INGAME_NW_CMD_NEXT_TURN;
  static const inGameCmdTypes INGAME_NW_CMD_MOVE_UNIT =
    inGameCmd_inGameCmdTypes_INGAME_NW_CMD_MOVE_UNIT;
  static const inGameCmdTypes INGAME_NW_CMD_ATTACK_UNIT =
    inGameCmd_inGameCmdTypes_INGAME_NW_CMD_ATTACK_UNIT;
  static const inGameCmdTypes INGAME_NW_CMD_PLACE_UNIT =
    inGameCmd_inGameCmdTypes_INGAME_NW_CMD_PLACE_UNIT;
  static const inGameCmdTypes INGAME_NW_CMD_REMOVE_UNIT =
    inGameCmd_inGameCmdTypes_INGAME_NW_CMD_REMOVE_UNIT;
  static inline bool inGameCmdTypes_IsValid(int value) {
    return inGameCmd_inGameCmdTypes_IsValid(value);
  }
  static const inGameCmdTypes inGameCmdTypes_MIN =
    inGameCmd_inGameCmdTypes_inGameCmdTypes_MIN;
  static const inGameCmdTypes inGameCmdTypes_MAX =
    inGameCmd_inGameCmdTypes_inGameCmdTypes_MAX;
  static const int inGameCmdTypes_ARRAYSIZE =
    inGameCmd_inGameCmdTypes_inGameCmdTypes_ARRAYSIZE;
  static inline const ::google::protobuf::EnumDescriptor*
  inGameCmdTypes_descriptor() {
    return inGameCmd_inGameCmdTypes_descriptor();
  }
  static inline const ::std::string& inGameCmdTypes_Name(inGameCmdTypes value) {
    return inGameCmd_inGameCmdTypes_Name(value);
  }
  static inline bool inGameCmdTypes_Parse(const ::std::string& name,
      inGameCmdTypes* value) {
    return inGameCmd_inGameCmdTypes_Parse(name, value);
  }

  // accessors -------------------------------------------------------

  // repeated fixed32 args = 2;
  int args_size() const;
  void clear_args();
  static const int kArgsFieldNumber = 2;
  ::google::protobuf::uint32 args(int index) const;
  void set_args(int index, ::google::protobuf::uint32 value);
  void add_args(::google::protobuf::uint32 value);
  const ::google::protobuf::RepeatedField< ::google::protobuf::uint32 >&
      args() const;
  ::google::protobuf::RepeatedField< ::google::protobuf::uint32 >*
      mutable_args();

  // required fixed32 type = 1 [default = 0];
  bool has_type() const;
  void clear_type();
  static const int kTypeFieldNumber = 1;
  ::google::protobuf::uint32 type() const;
  void set_type(::google::protobuf::uint32 value);

  // @@protoc_insertion_point(class_scope:inGameCmd)
 private:
  class HasBitSetters;

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  ::google::protobuf::internal::HasBits<1> _has_bits_;
  mutable ::google::protobuf::internal::CachedSize _cached_size_;
  ::google::protobuf::RepeatedField< ::google::protobuf::uint32 > args_;
  ::google::protobuf::uint32 type_;
  friend struct ::TableStruct_ntwrkCmd_2eproto;
};
// ===================================================================


// ===================================================================

#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
// preGameCmd

// required fixed32 type = 1 [default = 0];
inline bool preGameCmd::has_type() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void preGameCmd::clear_type() {
  type_ = 0u;
  _has_bits_[0] &= ~0x00000002u;
}
inline ::google::protobuf::uint32 preGameCmd::type() const {
  // @@protoc_insertion_point(field_get:preGameCmd.type)
  return type_;
}
inline void preGameCmd::set_type(::google::protobuf::uint32 value) {
  _has_bits_[0] |= 0x00000002u;
  type_ = value;
  // @@protoc_insertion_point(field_set:preGameCmd.type)
}

// optional string name = 2;
inline bool preGameCmd::has_name() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void preGameCmd::clear_name() {
  name_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  _has_bits_[0] &= ~0x00000001u;
}
inline const ::std::string& preGameCmd::name() const {
  // @@protoc_insertion_point(field_get:preGameCmd.name)
  return name_.GetNoArena();
}
inline void preGameCmd::set_name(const ::std::string& value) {
  _has_bits_[0] |= 0x00000001u;
  name_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:preGameCmd.name)
}
#if LANG_CXX11
inline void preGameCmd::set_name(::std::string&& value) {
  _has_bits_[0] |= 0x00000001u;
  name_.SetNoArena(
    &::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::move(value));
  // @@protoc_insertion_point(field_set_rvalue:preGameCmd.name)
}
#endif
inline void preGameCmd::set_name(const char* value) {
  GOOGLE_DCHECK(value != nullptr);
  _has_bits_[0] |= 0x00000001u;
  name_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:preGameCmd.name)
}
inline void preGameCmd::set_name(const char* value, size_t size) {
  _has_bits_[0] |= 0x00000001u;
  name_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:preGameCmd.name)
}
inline ::std::string* preGameCmd::mutable_name() {
  _has_bits_[0] |= 0x00000001u;
  // @@protoc_insertion_point(field_mutable:preGameCmd.name)
  return name_.MutableNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline ::std::string* preGameCmd::release_name() {
  // @@protoc_insertion_point(field_release:preGameCmd.name)
  if (!has_name()) {
    return nullptr;
  }
  _has_bits_[0] &= ~0x00000001u;
  return name_.ReleaseNonDefaultNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void preGameCmd::set_allocated_name(::std::string* name) {
  if (name != nullptr) {
    _has_bits_[0] |= 0x00000001u;
  } else {
    _has_bits_[0] &= ~0x00000001u;
  }
  name_.SetAllocatedNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), name);
  // @@protoc_insertion_point(field_set_allocated:preGameCmd.name)
}

// optional fixed32 indexNum = 3;
inline bool preGameCmd::has_indexnum() const {
  return (_has_bits_[0] & 0x00000004u) != 0;
}
inline void preGameCmd::clear_indexnum() {
  indexnum_ = 0u;
  _has_bits_[0] &= ~0x00000004u;
}
inline ::google::protobuf::uint32 preGameCmd::indexnum() const {
  // @@protoc_insertion_point(field_get:preGameCmd.indexNum)
  return indexnum_;
}
inline void preGameCmd::set_indexnum(::google::protobuf::uint32 value) {
  _has_bits_[0] |= 0x00000004u;
  indexnum_ = value;
  // @@protoc_insertion_point(field_set:preGameCmd.indexNum)
}

// -------------------------------------------------------------------

// inGameCmd

// required fixed32 type = 1 [default = 0];
inline bool inGameCmd::has_type() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void inGameCmd::clear_type() {
  type_ = 0u;
  _has_bits_[0] &= ~0x00000001u;
}
inline ::google::protobuf::uint32 inGameCmd::type() const {
  // @@protoc_insertion_point(field_get:inGameCmd.type)
  return type_;
}
inline void inGameCmd::set_type(::google::protobuf::uint32 value) {
  _has_bits_[0] |= 0x00000001u;
  type_ = value;
  // @@protoc_insertion_point(field_set:inGameCmd.type)
}

// repeated fixed32 args = 2;
inline int inGameCmd::args_size() const {
  return args_.size();
}
inline void inGameCmd::clear_args() {
  args_.Clear();
}
inline ::google::protobuf::uint32 inGameCmd::args(int index) const {
  // @@protoc_insertion_point(field_get:inGameCmd.args)
  return args_.Get(index);
}
inline void inGameCmd::set_args(int index, ::google::protobuf::uint32 value) {
  args_.Set(index, value);
  // @@protoc_insertion_point(field_set:inGameCmd.args)
}
inline void inGameCmd::add_args(::google::protobuf::uint32 value) {
  args_.Add(value);
  // @@protoc_insertion_point(field_add:inGameCmd.args)
}
inline const ::google::protobuf::RepeatedField< ::google::protobuf::uint32 >&
inGameCmd::args() const {
  // @@protoc_insertion_point(field_list:inGameCmd.args)
  return args_;
}
inline ::google::protobuf::RepeatedField< ::google::protobuf::uint32 >*
inGameCmd::mutable_args() {
  // @@protoc_insertion_point(field_mutable_list:inGameCmd.args)
  return &args_;
}

#ifdef __GNUC__
  #pragma GCC diagnostic pop
#endif  // __GNUC__
// -------------------------------------------------------------------


// @@protoc_insertion_point(namespace_scope)


namespace google {
namespace protobuf {

template <> struct is_proto_enum< ::preGameCmd_preGameCmdTypes> : ::std::true_type {};
template <>
inline const EnumDescriptor* GetEnumDescriptor< ::preGameCmd_preGameCmdTypes>() {
  return ::preGameCmd_preGameCmdTypes_descriptor();
}
template <> struct is_proto_enum< ::inGameCmd_inGameCmdTypes> : ::std::true_type {};
template <>
inline const EnumDescriptor* GetEnumDescriptor< ::inGameCmd_inGameCmdTypes>() {
  return ::inGameCmd_inGameCmdTypes_descriptor();
}

}  // namespace protobuf
}  // namespace google

// @@protoc_insertion_point(global_scope)

#include <google/protobuf/port_undef.inc>
#endif  // PROTOBUF_INCLUDED_ntwrkCmd_2eproto