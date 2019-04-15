// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: ntwrkCmd.proto

#include "ntwrkCmd.pb.h"

#include <algorithm>

#include <google/protobuf/stubs/common.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/extension_set.h>
#include <google/protobuf/wire_format_lite_inl.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/reflection_ops.h>
#include <google/protobuf/wire_format.h>
// @@protoc_insertion_point(includes)
#include <google/protobuf/port_def.inc>

class preGameCmdDefaultTypeInternal {
 public:
  ::google::protobuf::internal::ExplicitlyConstructed<preGameCmd> _instance;
} _preGameCmd_default_instance_;
class inGameCmdDefaultTypeInternal {
 public:
  ::google::protobuf::internal::ExplicitlyConstructed<inGameCmd> _instance;
} _inGameCmd_default_instance_;
static void InitDefaultspreGameCmd_ntwrkCmd_2eproto() {
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  {
    void* ptr = &::_preGameCmd_default_instance_;
    new (ptr) ::preGameCmd();
    ::google::protobuf::internal::OnShutdownDestroyMessage(ptr);
  }
  ::preGameCmd::InitAsDefaultInstance();
}

::google::protobuf::internal::SCCInfo<0> scc_info_preGameCmd_ntwrkCmd_2eproto =
    {{ATOMIC_VAR_INIT(::google::protobuf::internal::SCCInfoBase::kUninitialized), 0, InitDefaultspreGameCmd_ntwrkCmd_2eproto}, {}};

static void InitDefaultsinGameCmd_ntwrkCmd_2eproto() {
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  {
    void* ptr = &::_inGameCmd_default_instance_;
    new (ptr) ::inGameCmd();
    ::google::protobuf::internal::OnShutdownDestroyMessage(ptr);
  }
  ::inGameCmd::InitAsDefaultInstance();
}

::google::protobuf::internal::SCCInfo<0> scc_info_inGameCmd_ntwrkCmd_2eproto =
    {{ATOMIC_VAR_INIT(::google::protobuf::internal::SCCInfoBase::kUninitialized), 0, InitDefaultsinGameCmd_ntwrkCmd_2eproto}, {}};

void InitDefaults_ntwrkCmd_2eproto() {
  ::google::protobuf::internal::InitSCC(&scc_info_preGameCmd_ntwrkCmd_2eproto.base);
  ::google::protobuf::internal::InitSCC(&scc_info_inGameCmd_ntwrkCmd_2eproto.base);
}

::google::protobuf::Metadata file_level_metadata_ntwrkCmd_2eproto[2];
const ::google::protobuf::EnumDescriptor* file_level_enum_descriptors_ntwrkCmd_2eproto[2];
constexpr ::google::protobuf::ServiceDescriptor const** file_level_service_descriptors_ntwrkCmd_2eproto = nullptr;

const ::google::protobuf::uint32 TableStruct_ntwrkCmd_2eproto::offsets[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
  PROTOBUF_FIELD_OFFSET(::preGameCmd, _has_bits_),
  PROTOBUF_FIELD_OFFSET(::preGameCmd, _internal_metadata_),
  ~0u,  // no _extensions_
  ~0u,  // no _oneof_case_
  ~0u,  // no _weak_field_map_
  PROTOBUF_FIELD_OFFSET(::preGameCmd, type_),
  PROTOBUF_FIELD_OFFSET(::preGameCmd, name_),
  PROTOBUF_FIELD_OFFSET(::preGameCmd, indexnum_),
  1,
  0,
  2,
  PROTOBUF_FIELD_OFFSET(::inGameCmd, _has_bits_),
  PROTOBUF_FIELD_OFFSET(::inGameCmd, _internal_metadata_),
  ~0u,  // no _extensions_
  ~0u,  // no _oneof_case_
  ~0u,  // no _weak_field_map_
  PROTOBUF_FIELD_OFFSET(::inGameCmd, type_),
  PROTOBUF_FIELD_OFFSET(::inGameCmd, args_),
  0,
  ~0u,
};
static const ::google::protobuf::internal::MigrationSchema schemas[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
  { 0, 8, sizeof(::preGameCmd)},
  { 11, 18, sizeof(::inGameCmd)},
};

static ::google::protobuf::Message const * const file_default_instances[] = {
  reinterpret_cast<const ::google::protobuf::Message*>(&::_preGameCmd_default_instance_),
  reinterpret_cast<const ::google::protobuf::Message*>(&::_inGameCmd_default_instance_),
};

::google::protobuf::internal::AssignDescriptorsTable assign_descriptors_table_ntwrkCmd_2eproto = {
  {}, AddDescriptors_ntwrkCmd_2eproto, "ntwrkCmd.proto", schemas,
  file_default_instances, TableStruct_ntwrkCmd_2eproto::offsets,
  file_level_metadata_ntwrkCmd_2eproto, 2, file_level_enum_descriptors_ntwrkCmd_2eproto, file_level_service_descriptors_ntwrkCmd_2eproto,
};

const char descriptor_table_protodef_ntwrkCmd_2eproto[] =
  "\n\016ntwrkCmd.proto\"\367\001\n\npreGameCmd\022\017\n\004type\030"
  "\001 \002(\007:\0010\022\014\n\004name\030\002 \001(\t\022\020\n\010indexNum\030\003 \001(\007"
  "\"\267\001\n\017preGameCmdTypes\022\027\n\023PREGAME_NW_CMD_N"
  "ONE\020\000\022#\n\037PREGAME_NW_CMD_PLAYER_CONNECTED"
  "\020\001\022&\n\"PREGAME_NW_CMD_PLAYER_DISCONNECTED"
  "\020\002\022\037\n\033PREGAME_NW_CMD_PLAYER_READY\020\003\022\035\n\031P"
  "REGAME_NW_CMD_START_GAME\020\004\"\270\002\n\tinGameCmd"
  "\022\017\n\004type\030\001 \002(\007:\0010\022\014\n\004args\030\002 \003(\007\"\213\002\n\016inGa"
  "meCmdTypes\022\026\n\022INGAME_NW_CMD_NONE\020\000\022$\n IN"
  "GAME_NW_CMD_PLAYER_RECONNECTED\020\001\022%\n!INGA"
  "ME_NW_CMD_PLAYER_DISCONNECTED\020\002\022\033\n\027INGAM"
  "E_NW_CMD_NEXT_TURN\020\003\022\033\n\027INGAME_NW_CMD_MO"
  "VE_UNIT\020\004\022\035\n\031INGAME_NW_CMD_ATTACK_UNIT\020\005"
  "\022\034\n\030INGAME_NW_CMD_PLACE_UNIT\020\006\022\035\n\031INGAME"
  "_NW_CMD_REMOVE_UNIT\020\007"
  ;
::google::protobuf::internal::DescriptorTable descriptor_table_ntwrkCmd_2eproto = {
  false, InitDefaults_ntwrkCmd_2eproto, 
  descriptor_table_protodef_ntwrkCmd_2eproto,
  "ntwrkCmd.proto", &assign_descriptors_table_ntwrkCmd_2eproto, 581,
};

void AddDescriptors_ntwrkCmd_2eproto() {
  static constexpr ::google::protobuf::internal::InitFunc deps[1] =
  {
  };
 ::google::protobuf::internal::AddDescriptors(&descriptor_table_ntwrkCmd_2eproto, deps, 0);
}

// Force running AddDescriptors() at dynamic initialization time.
static bool dynamic_init_dummy_ntwrkCmd_2eproto = []() { AddDescriptors_ntwrkCmd_2eproto(); return true; }();
const ::google::protobuf::EnumDescriptor* preGameCmd_preGameCmdTypes_descriptor() {
  ::google::protobuf::internal::AssignDescriptors(&assign_descriptors_table_ntwrkCmd_2eproto);
  return file_level_enum_descriptors_ntwrkCmd_2eproto[0];
}
bool preGameCmd_preGameCmdTypes_IsValid(int value) {
  switch (value) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
      return true;
    default:
      return false;
  }
}

#if !defined(_MSC_VER) || _MSC_VER >= 1900
const preGameCmd_preGameCmdTypes preGameCmd::PREGAME_NW_CMD_NONE;
const preGameCmd_preGameCmdTypes preGameCmd::PREGAME_NW_CMD_PLAYER_CONNECTED;
const preGameCmd_preGameCmdTypes preGameCmd::PREGAME_NW_CMD_PLAYER_DISCONNECTED;
const preGameCmd_preGameCmdTypes preGameCmd::PREGAME_NW_CMD_PLAYER_READY;
const preGameCmd_preGameCmdTypes preGameCmd::PREGAME_NW_CMD_START_GAME;
const preGameCmd_preGameCmdTypes preGameCmd::preGameCmdTypes_MIN;
const preGameCmd_preGameCmdTypes preGameCmd::preGameCmdTypes_MAX;
const int preGameCmd::preGameCmdTypes_ARRAYSIZE;
#endif  // !defined(_MSC_VER) || _MSC_VER >= 1900
const ::google::protobuf::EnumDescriptor* inGameCmd_inGameCmdTypes_descriptor() {
  ::google::protobuf::internal::AssignDescriptors(&assign_descriptors_table_ntwrkCmd_2eproto);
  return file_level_enum_descriptors_ntwrkCmd_2eproto[1];
}
bool inGameCmd_inGameCmdTypes_IsValid(int value) {
  switch (value) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
      return true;
    default:
      return false;
  }
}

#if !defined(_MSC_VER) || _MSC_VER >= 1900
const inGameCmd_inGameCmdTypes inGameCmd::INGAME_NW_CMD_NONE;
const inGameCmd_inGameCmdTypes inGameCmd::INGAME_NW_CMD_PLAYER_RECONNECTED;
const inGameCmd_inGameCmdTypes inGameCmd::INGAME_NW_CMD_PLAYER_DISCONNECTED;
const inGameCmd_inGameCmdTypes inGameCmd::INGAME_NW_CMD_NEXT_TURN;
const inGameCmd_inGameCmdTypes inGameCmd::INGAME_NW_CMD_MOVE_UNIT;
const inGameCmd_inGameCmdTypes inGameCmd::INGAME_NW_CMD_ATTACK_UNIT;
const inGameCmd_inGameCmdTypes inGameCmd::INGAME_NW_CMD_PLACE_UNIT;
const inGameCmd_inGameCmdTypes inGameCmd::INGAME_NW_CMD_REMOVE_UNIT;
const inGameCmd_inGameCmdTypes inGameCmd::inGameCmdTypes_MIN;
const inGameCmd_inGameCmdTypes inGameCmd::inGameCmdTypes_MAX;
const int inGameCmd::inGameCmdTypes_ARRAYSIZE;
#endif  // !defined(_MSC_VER) || _MSC_VER >= 1900

// ===================================================================

void preGameCmd::InitAsDefaultInstance() {
}
class preGameCmd::HasBitSetters {
 public:
  static void set_has_type(preGameCmd* msg) {
    msg->_has_bits_[0] |= 0x00000002u;
  }
  static void set_has_name(preGameCmd* msg) {
    msg->_has_bits_[0] |= 0x00000001u;
  }
  static void set_has_indexnum(preGameCmd* msg) {
    msg->_has_bits_[0] |= 0x00000004u;
  }
};

#if !defined(_MSC_VER) || _MSC_VER >= 1900
const int preGameCmd::kTypeFieldNumber;
const int preGameCmd::kNameFieldNumber;
const int preGameCmd::kIndexNumFieldNumber;
#endif  // !defined(_MSC_VER) || _MSC_VER >= 1900

preGameCmd::preGameCmd()
  : ::google::protobuf::Message(), _internal_metadata_(nullptr) {
  SharedCtor();
  // @@protoc_insertion_point(constructor:preGameCmd)
}
preGameCmd::preGameCmd(const preGameCmd& from)
  : ::google::protobuf::Message(),
      _internal_metadata_(nullptr),
      _has_bits_(from._has_bits_) {
  _internal_metadata_.MergeFrom(from._internal_metadata_);
  name_.UnsafeSetDefault(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  if (from.has_name()) {
    name_.AssignWithDefault(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), from.name_);
  }
  ::memcpy(&type_, &from.type_,
    static_cast<size_t>(reinterpret_cast<char*>(&indexnum_) -
    reinterpret_cast<char*>(&type_)) + sizeof(indexnum_));
  // @@protoc_insertion_point(copy_constructor:preGameCmd)
}

void preGameCmd::SharedCtor() {
  ::google::protobuf::internal::InitSCC(
      &scc_info_preGameCmd_ntwrkCmd_2eproto.base);
  name_.UnsafeSetDefault(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  ::memset(&type_, 0, static_cast<size_t>(
      reinterpret_cast<char*>(&indexnum_) -
      reinterpret_cast<char*>(&type_)) + sizeof(indexnum_));
}

preGameCmd::~preGameCmd() {
  // @@protoc_insertion_point(destructor:preGameCmd)
  SharedDtor();
}

void preGameCmd::SharedDtor() {
  name_.DestroyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}

void preGameCmd::SetCachedSize(int size) const {
  _cached_size_.Set(size);
}
const preGameCmd& preGameCmd::default_instance() {
  ::google::protobuf::internal::InitSCC(&::scc_info_preGameCmd_ntwrkCmd_2eproto.base);
  return *internal_default_instance();
}


void preGameCmd::Clear() {
// @@protoc_insertion_point(message_clear_start:preGameCmd)
  ::google::protobuf::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  cached_has_bits = _has_bits_[0];
  if (cached_has_bits & 0x00000001u) {
    name_.ClearNonDefaultToEmptyNoArena();
  }
  if (cached_has_bits & 0x00000006u) {
    ::memset(&type_, 0, static_cast<size_t>(
        reinterpret_cast<char*>(&indexnum_) -
        reinterpret_cast<char*>(&type_)) + sizeof(indexnum_));
  }
  _has_bits_.Clear();
  _internal_metadata_.Clear();
}

#if GOOGLE_PROTOBUF_ENABLE_EXPERIMENTAL_PARSER
const char* preGameCmd::_InternalParse(const char* begin, const char* end, void* object,
                  ::google::protobuf::internal::ParseContext* ctx) {
  auto msg = static_cast<preGameCmd*>(object);
  ::google::protobuf::int32 size; (void)size;
  int depth; (void)depth;
  ::google::protobuf::uint32 tag;
  ::google::protobuf::internal::ParseFunc parser_till_end; (void)parser_till_end;
  auto ptr = begin;
  while (ptr < end) {
    ptr = ::google::protobuf::io::Parse32(ptr, &tag);
    GOOGLE_PROTOBUF_PARSER_ASSERT(ptr);
    switch (tag >> 3) {
      // required fixed32 type = 1 [default = 0];
      case 1: {
        if (static_cast<::google::protobuf::uint8>(tag) != 13) goto handle_unusual;
        msg->set_type(::google::protobuf::io::UnalignedLoad<::google::protobuf::uint32>(ptr));
        ptr += sizeof(::google::protobuf::uint32);
        break;
      }
      // optional string name = 2;
      case 2: {
        if (static_cast<::google::protobuf::uint8>(tag) != 18) goto handle_unusual;
        ptr = ::google::protobuf::io::ReadSize(ptr, &size);
        GOOGLE_PROTOBUF_PARSER_ASSERT(ptr);
        ctx->extra_parse_data().SetFieldName("preGameCmd.name");
        object = msg->mutable_name();
        if (size > end - ptr + ::google::protobuf::internal::ParseContext::kSlopBytes) {
          parser_till_end = ::google::protobuf::internal::GreedyStringParserUTF8Verify;
          goto string_till_end;
        }
        GOOGLE_PROTOBUF_PARSER_ASSERT(::google::protobuf::internal::StringCheckUTF8Verify(ptr, size, ctx));
        ::google::protobuf::internal::InlineGreedyStringParser(object, ptr, size, ctx);
        ptr += size;
        break;
      }
      // optional fixed32 indexNum = 3;
      case 3: {
        if (static_cast<::google::protobuf::uint8>(tag) != 29) goto handle_unusual;
        msg->set_indexnum(::google::protobuf::io::UnalignedLoad<::google::protobuf::uint32>(ptr));
        ptr += sizeof(::google::protobuf::uint32);
        break;
      }
      default: {
      handle_unusual:
        if ((tag & 7) == 4 || tag == 0) {
          ctx->EndGroup(tag);
          return ptr;
        }
        auto res = UnknownFieldParse(tag, {_InternalParse, msg},
          ptr, end, msg->_internal_metadata_.mutable_unknown_fields(), ctx);
        ptr = res.first;
        GOOGLE_PROTOBUF_PARSER_ASSERT(ptr != nullptr);
        if (res.second) return ptr;
      }
    }  // switch
  }  // while
  return ptr;
string_till_end:
  static_cast<::std::string*>(object)->clear();
  static_cast<::std::string*>(object)->reserve(size);
  goto len_delim_till_end;
len_delim_till_end:
  return ctx->StoreAndTailCall(ptr, end, {_InternalParse, msg},
                               {parser_till_end, object}, size);
}
#else  // GOOGLE_PROTOBUF_ENABLE_EXPERIMENTAL_PARSER
bool preGameCmd::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!PROTOBUF_PREDICT_TRUE(EXPRESSION)) goto failure
  ::google::protobuf::uint32 tag;
  // @@protoc_insertion_point(parse_start:preGameCmd)
  for (;;) {
    ::std::pair<::google::protobuf::uint32, bool> p = input->ReadTagWithCutoffNoLastTag(127u);
    tag = p.first;
    if (!p.second) goto handle_unusual;
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // required fixed32 type = 1 [default = 0];
      case 1: {
        if (static_cast< ::google::protobuf::uint8>(tag) == (13 & 0xFF)) {
          HasBitSetters::set_has_type(this);
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::uint32, ::google::protobuf::internal::WireFormatLite::TYPE_FIXED32>(
                 input, &type_)));
        } else {
          goto handle_unusual;
        }
        break;
      }

      // optional string name = 2;
      case 2: {
        if (static_cast< ::google::protobuf::uint8>(tag) == (18 & 0xFF)) {
          DO_(::google::protobuf::internal::WireFormatLite::ReadString(
                input, this->mutable_name()));
          ::google::protobuf::internal::WireFormat::VerifyUTF8StringNamedField(
            this->name().data(), static_cast<int>(this->name().length()),
            ::google::protobuf::internal::WireFormat::PARSE,
            "preGameCmd.name");
        } else {
          goto handle_unusual;
        }
        break;
      }

      // optional fixed32 indexNum = 3;
      case 3: {
        if (static_cast< ::google::protobuf::uint8>(tag) == (29 & 0xFF)) {
          HasBitSetters::set_has_indexnum(this);
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::uint32, ::google::protobuf::internal::WireFormatLite::TYPE_FIXED32>(
                 input, &indexnum_)));
        } else {
          goto handle_unusual;
        }
        break;
      }

      default: {
      handle_unusual:
        if (tag == 0) {
          goto success;
        }
        DO_(::google::protobuf::internal::WireFormat::SkipField(
              input, tag, _internal_metadata_.mutable_unknown_fields()));
        break;
      }
    }
  }
success:
  // @@protoc_insertion_point(parse_success:preGameCmd)
  return true;
failure:
  // @@protoc_insertion_point(parse_failure:preGameCmd)
  return false;
#undef DO_
}
#endif  // GOOGLE_PROTOBUF_ENABLE_EXPERIMENTAL_PARSER

void preGameCmd::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // @@protoc_insertion_point(serialize_start:preGameCmd)
  ::google::protobuf::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  cached_has_bits = _has_bits_[0];
  // required fixed32 type = 1 [default = 0];
  if (cached_has_bits & 0x00000002u) {
    ::google::protobuf::internal::WireFormatLite::WriteFixed32(1, this->type(), output);
  }

  // optional string name = 2;
  if (cached_has_bits & 0x00000001u) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8StringNamedField(
      this->name().data(), static_cast<int>(this->name().length()),
      ::google::protobuf::internal::WireFormat::SERIALIZE,
      "preGameCmd.name");
    ::google::protobuf::internal::WireFormatLite::WriteStringMaybeAliased(
      2, this->name(), output);
  }

  // optional fixed32 indexNum = 3;
  if (cached_has_bits & 0x00000004u) {
    ::google::protobuf::internal::WireFormatLite::WriteFixed32(3, this->indexnum(), output);
  }

  if (_internal_metadata_.have_unknown_fields()) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        _internal_metadata_.unknown_fields(), output);
  }
  // @@protoc_insertion_point(serialize_end:preGameCmd)
}

::google::protobuf::uint8* preGameCmd::InternalSerializeWithCachedSizesToArray(
    ::google::protobuf::uint8* target) const {
  // @@protoc_insertion_point(serialize_to_array_start:preGameCmd)
  ::google::protobuf::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  cached_has_bits = _has_bits_[0];
  // required fixed32 type = 1 [default = 0];
  if (cached_has_bits & 0x00000002u) {
    target = ::google::protobuf::internal::WireFormatLite::WriteFixed32ToArray(1, this->type(), target);
  }

  // optional string name = 2;
  if (cached_has_bits & 0x00000001u) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8StringNamedField(
      this->name().data(), static_cast<int>(this->name().length()),
      ::google::protobuf::internal::WireFormat::SERIALIZE,
      "preGameCmd.name");
    target =
      ::google::protobuf::internal::WireFormatLite::WriteStringToArray(
        2, this->name(), target);
  }

  // optional fixed32 indexNum = 3;
  if (cached_has_bits & 0x00000004u) {
    target = ::google::protobuf::internal::WireFormatLite::WriteFixed32ToArray(3, this->indexnum(), target);
  }

  if (_internal_metadata_.have_unknown_fields()) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        _internal_metadata_.unknown_fields(), target);
  }
  // @@protoc_insertion_point(serialize_to_array_end:preGameCmd)
  return target;
}

size_t preGameCmd::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:preGameCmd)
  size_t total_size = 0;

  if (_internal_metadata_.have_unknown_fields()) {
    total_size +=
      ::google::protobuf::internal::WireFormat::ComputeUnknownFieldsSize(
        _internal_metadata_.unknown_fields());
  }
  // required fixed32 type = 1 [default = 0];
  if (has_type()) {
    total_size += 1 + 4;
  }
  ::google::protobuf::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  // optional string name = 2;
  cached_has_bits = _has_bits_[0];
  if (cached_has_bits & 0x00000001u) {
    total_size += 1 +
      ::google::protobuf::internal::WireFormatLite::StringSize(
        this->name());
  }

  // optional fixed32 indexNum = 3;
  if (cached_has_bits & 0x00000004u) {
    total_size += 1 + 4;
  }

  int cached_size = ::google::protobuf::internal::ToCachedSize(total_size);
  SetCachedSize(cached_size);
  return total_size;
}

void preGameCmd::MergeFrom(const ::google::protobuf::Message& from) {
// @@protoc_insertion_point(generalized_merge_from_start:preGameCmd)
  GOOGLE_DCHECK_NE(&from, this);
  const preGameCmd* source =
      ::google::protobuf::DynamicCastToGenerated<preGameCmd>(
          &from);
  if (source == nullptr) {
  // @@protoc_insertion_point(generalized_merge_from_cast_fail:preGameCmd)
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
  // @@protoc_insertion_point(generalized_merge_from_cast_success:preGameCmd)
    MergeFrom(*source);
  }
}

void preGameCmd::MergeFrom(const preGameCmd& from) {
// @@protoc_insertion_point(class_specific_merge_from_start:preGameCmd)
  GOOGLE_DCHECK_NE(&from, this);
  _internal_metadata_.MergeFrom(from._internal_metadata_);
  ::google::protobuf::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  cached_has_bits = from._has_bits_[0];
  if (cached_has_bits & 0x00000007u) {
    if (cached_has_bits & 0x00000001u) {
      _has_bits_[0] |= 0x00000001u;
      name_.AssignWithDefault(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), from.name_);
    }
    if (cached_has_bits & 0x00000002u) {
      type_ = from.type_;
    }
    if (cached_has_bits & 0x00000004u) {
      indexnum_ = from.indexnum_;
    }
    _has_bits_[0] |= cached_has_bits;
  }
}

void preGameCmd::CopyFrom(const ::google::protobuf::Message& from) {
// @@protoc_insertion_point(generalized_copy_from_start:preGameCmd)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void preGameCmd::CopyFrom(const preGameCmd& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:preGameCmd)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool preGameCmd::IsInitialized() const {
  if ((_has_bits_[0] & 0x00000002) != 0x00000002) return false;
  return true;
}

void preGameCmd::Swap(preGameCmd* other) {
  if (other == this) return;
  InternalSwap(other);
}
void preGameCmd::InternalSwap(preGameCmd* other) {
  using std::swap;
  _internal_metadata_.Swap(&other->_internal_metadata_);
  swap(_has_bits_[0], other->_has_bits_[0]);
  name_.Swap(&other->name_, &::google::protobuf::internal::GetEmptyStringAlreadyInited(),
    GetArenaNoVirtual());
  swap(type_, other->type_);
  swap(indexnum_, other->indexnum_);
}

::google::protobuf::Metadata preGameCmd::GetMetadata() const {
  ::google::protobuf::internal::AssignDescriptors(&::assign_descriptors_table_ntwrkCmd_2eproto);
  return ::file_level_metadata_ntwrkCmd_2eproto[kIndexInFileMessages];
}


// ===================================================================

void inGameCmd::InitAsDefaultInstance() {
}
class inGameCmd::HasBitSetters {
 public:
  static void set_has_type(inGameCmd* msg) {
    msg->_has_bits_[0] |= 0x00000001u;
  }
};

#if !defined(_MSC_VER) || _MSC_VER >= 1900
const int inGameCmd::kTypeFieldNumber;
const int inGameCmd::kArgsFieldNumber;
#endif  // !defined(_MSC_VER) || _MSC_VER >= 1900

inGameCmd::inGameCmd()
  : ::google::protobuf::Message(), _internal_metadata_(nullptr) {
  SharedCtor();
  // @@protoc_insertion_point(constructor:inGameCmd)
}
inGameCmd::inGameCmd(const inGameCmd& from)
  : ::google::protobuf::Message(),
      _internal_metadata_(nullptr),
      _has_bits_(from._has_bits_),
      args_(from.args_) {
  _internal_metadata_.MergeFrom(from._internal_metadata_);
  type_ = from.type_;
  // @@protoc_insertion_point(copy_constructor:inGameCmd)
}

void inGameCmd::SharedCtor() {
  type_ = 0u;
}

inGameCmd::~inGameCmd() {
  // @@protoc_insertion_point(destructor:inGameCmd)
  SharedDtor();
}

void inGameCmd::SharedDtor() {
}

void inGameCmd::SetCachedSize(int size) const {
  _cached_size_.Set(size);
}
const inGameCmd& inGameCmd::default_instance() {
  ::google::protobuf::internal::InitSCC(&::scc_info_inGameCmd_ntwrkCmd_2eproto.base);
  return *internal_default_instance();
}


void inGameCmd::Clear() {
// @@protoc_insertion_point(message_clear_start:inGameCmd)
  ::google::protobuf::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  args_.Clear();
  type_ = 0u;
  _has_bits_.Clear();
  _internal_metadata_.Clear();
}

#if GOOGLE_PROTOBUF_ENABLE_EXPERIMENTAL_PARSER
const char* inGameCmd::_InternalParse(const char* begin, const char* end, void* object,
                  ::google::protobuf::internal::ParseContext* ctx) {
  auto msg = static_cast<inGameCmd*>(object);
  ::google::protobuf::int32 size; (void)size;
  int depth; (void)depth;
  ::google::protobuf::uint32 tag;
  ::google::protobuf::internal::ParseFunc parser_till_end; (void)parser_till_end;
  auto ptr = begin;
  while (ptr < end) {
    ptr = ::google::protobuf::io::Parse32(ptr, &tag);
    GOOGLE_PROTOBUF_PARSER_ASSERT(ptr);
    switch (tag >> 3) {
      // required fixed32 type = 1 [default = 0];
      case 1: {
        if (static_cast<::google::protobuf::uint8>(tag) != 13) goto handle_unusual;
        msg->set_type(::google::protobuf::io::UnalignedLoad<::google::protobuf::uint32>(ptr));
        ptr += sizeof(::google::protobuf::uint32);
        break;
      }
      // repeated fixed32 args = 2;
      case 2: {
        if (static_cast<::google::protobuf::uint8>(tag) == 21) {
          do {
            msg->add_args(::google::protobuf::io::UnalignedLoad<::google::protobuf::uint32>(ptr));
            ptr += sizeof(::google::protobuf::uint32);
            if (ptr >= end) break;
          } while ((::google::protobuf::io::UnalignedLoad<::google::protobuf::uint64>(ptr) & 255) == 21 && (ptr += 1));
          break;
        } else if (static_cast<::google::protobuf::uint8>(tag) != 18) goto handle_unusual;
        ptr = ::google::protobuf::io::ReadSize(ptr, &size);
        GOOGLE_PROTOBUF_PARSER_ASSERT(ptr);
        parser_till_end = ::google::protobuf::internal::PackedFixed32Parser;
        object = msg->mutable_args();
        if (size > end - ptr) goto len_delim_till_end;
        auto newend = ptr + size;
        if (size) ptr = parser_till_end(ptr, newend, object, ctx);
        GOOGLE_PROTOBUF_PARSER_ASSERT(ptr == newend);
        break;
      }
      default: {
      handle_unusual:
        if ((tag & 7) == 4 || tag == 0) {
          ctx->EndGroup(tag);
          return ptr;
        }
        auto res = UnknownFieldParse(tag, {_InternalParse, msg},
          ptr, end, msg->_internal_metadata_.mutable_unknown_fields(), ctx);
        ptr = res.first;
        GOOGLE_PROTOBUF_PARSER_ASSERT(ptr != nullptr);
        if (res.second) return ptr;
      }
    }  // switch
  }  // while
  return ptr;
len_delim_till_end:
  return ctx->StoreAndTailCall(ptr, end, {_InternalParse, msg},
                               {parser_till_end, object}, size);
}
#else  // GOOGLE_PROTOBUF_ENABLE_EXPERIMENTAL_PARSER
bool inGameCmd::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!PROTOBUF_PREDICT_TRUE(EXPRESSION)) goto failure
  ::google::protobuf::uint32 tag;
  // @@protoc_insertion_point(parse_start:inGameCmd)
  for (;;) {
    ::std::pair<::google::protobuf::uint32, bool> p = input->ReadTagWithCutoffNoLastTag(127u);
    tag = p.first;
    if (!p.second) goto handle_unusual;
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // required fixed32 type = 1 [default = 0];
      case 1: {
        if (static_cast< ::google::protobuf::uint8>(tag) == (13 & 0xFF)) {
          HasBitSetters::set_has_type(this);
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::uint32, ::google::protobuf::internal::WireFormatLite::TYPE_FIXED32>(
                 input, &type_)));
        } else {
          goto handle_unusual;
        }
        break;
      }

      // repeated fixed32 args = 2;
      case 2: {
        if (static_cast< ::google::protobuf::uint8>(tag) == (21 & 0xFF)) {
          DO_((::google::protobuf::internal::WireFormatLite::ReadRepeatedPrimitive<
                   ::google::protobuf::uint32, ::google::protobuf::internal::WireFormatLite::TYPE_FIXED32>(
                 1, 21u, input, this->mutable_args())));
        } else if (static_cast< ::google::protobuf::uint8>(tag) == (18 & 0xFF)) {
          DO_((::google::protobuf::internal::WireFormatLite::ReadPackedPrimitiveNoInline<
                   ::google::protobuf::uint32, ::google::protobuf::internal::WireFormatLite::TYPE_FIXED32>(
                 input, this->mutable_args())));
        } else {
          goto handle_unusual;
        }
        break;
      }

      default: {
      handle_unusual:
        if (tag == 0) {
          goto success;
        }
        DO_(::google::protobuf::internal::WireFormat::SkipField(
              input, tag, _internal_metadata_.mutable_unknown_fields()));
        break;
      }
    }
  }
success:
  // @@protoc_insertion_point(parse_success:inGameCmd)
  return true;
failure:
  // @@protoc_insertion_point(parse_failure:inGameCmd)
  return false;
#undef DO_
}
#endif  // GOOGLE_PROTOBUF_ENABLE_EXPERIMENTAL_PARSER

void inGameCmd::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // @@protoc_insertion_point(serialize_start:inGameCmd)
  ::google::protobuf::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  cached_has_bits = _has_bits_[0];
  // required fixed32 type = 1 [default = 0];
  if (cached_has_bits & 0x00000001u) {
    ::google::protobuf::internal::WireFormatLite::WriteFixed32(1, this->type(), output);
  }

  // repeated fixed32 args = 2;
  for (int i = 0, n = this->args_size(); i < n; i++) {
    ::google::protobuf::internal::WireFormatLite::WriteFixed32(
      2, this->args(i), output);
  }

  if (_internal_metadata_.have_unknown_fields()) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        _internal_metadata_.unknown_fields(), output);
  }
  // @@protoc_insertion_point(serialize_end:inGameCmd)
}

::google::protobuf::uint8* inGameCmd::InternalSerializeWithCachedSizesToArray(
    ::google::protobuf::uint8* target) const {
  // @@protoc_insertion_point(serialize_to_array_start:inGameCmd)
  ::google::protobuf::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  cached_has_bits = _has_bits_[0];
  // required fixed32 type = 1 [default = 0];
  if (cached_has_bits & 0x00000001u) {
    target = ::google::protobuf::internal::WireFormatLite::WriteFixed32ToArray(1, this->type(), target);
  }

  // repeated fixed32 args = 2;
  target = ::google::protobuf::internal::WireFormatLite::
    WriteFixed32ToArray(2, this->args_, target);

  if (_internal_metadata_.have_unknown_fields()) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        _internal_metadata_.unknown_fields(), target);
  }
  // @@protoc_insertion_point(serialize_to_array_end:inGameCmd)
  return target;
}

size_t inGameCmd::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:inGameCmd)
  size_t total_size = 0;

  if (_internal_metadata_.have_unknown_fields()) {
    total_size +=
      ::google::protobuf::internal::WireFormat::ComputeUnknownFieldsSize(
        _internal_metadata_.unknown_fields());
  }
  // required fixed32 type = 1 [default = 0];
  if (has_type()) {
    total_size += 1 + 4;
  }
  ::google::protobuf::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  // repeated fixed32 args = 2;
  {
    unsigned int count = static_cast<unsigned int>(this->args_size());
    size_t data_size = 4UL * count;
    total_size += 1 *
                  ::google::protobuf::internal::FromIntSize(this->args_size());
    total_size += data_size;
  }

  int cached_size = ::google::protobuf::internal::ToCachedSize(total_size);
  SetCachedSize(cached_size);
  return total_size;
}

void inGameCmd::MergeFrom(const ::google::protobuf::Message& from) {
// @@protoc_insertion_point(generalized_merge_from_start:inGameCmd)
  GOOGLE_DCHECK_NE(&from, this);
  const inGameCmd* source =
      ::google::protobuf::DynamicCastToGenerated<inGameCmd>(
          &from);
  if (source == nullptr) {
  // @@protoc_insertion_point(generalized_merge_from_cast_fail:inGameCmd)
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
  // @@protoc_insertion_point(generalized_merge_from_cast_success:inGameCmd)
    MergeFrom(*source);
  }
}

void inGameCmd::MergeFrom(const inGameCmd& from) {
// @@protoc_insertion_point(class_specific_merge_from_start:inGameCmd)
  GOOGLE_DCHECK_NE(&from, this);
  _internal_metadata_.MergeFrom(from._internal_metadata_);
  ::google::protobuf::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  args_.MergeFrom(from.args_);
  if (from.has_type()) {
    set_type(from.type());
  }
}

void inGameCmd::CopyFrom(const ::google::protobuf::Message& from) {
// @@protoc_insertion_point(generalized_copy_from_start:inGameCmd)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void inGameCmd::CopyFrom(const inGameCmd& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:inGameCmd)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool inGameCmd::IsInitialized() const {
  if ((_has_bits_[0] & 0x00000001) != 0x00000001) return false;
  return true;
}

void inGameCmd::Swap(inGameCmd* other) {
  if (other == this) return;
  InternalSwap(other);
}
void inGameCmd::InternalSwap(inGameCmd* other) {
  using std::swap;
  _internal_metadata_.Swap(&other->_internal_metadata_);
  swap(_has_bits_[0], other->_has_bits_[0]);
  args_.InternalSwap(&other->args_);
  swap(type_, other->type_);
}

::google::protobuf::Metadata inGameCmd::GetMetadata() const {
  ::google::protobuf::internal::AssignDescriptors(&::assign_descriptors_table_ntwrkCmd_2eproto);
  return ::file_level_metadata_ntwrkCmd_2eproto[kIndexInFileMessages];
}


// @@protoc_insertion_point(namespace_scope)
namespace google {
namespace protobuf {
template<> PROTOBUF_NOINLINE ::preGameCmd* Arena::CreateMaybeMessage< ::preGameCmd >(Arena* arena) {
  return Arena::CreateInternal< ::preGameCmd >(arena);
}
template<> PROTOBUF_NOINLINE ::inGameCmd* Arena::CreateMaybeMessage< ::inGameCmd >(Arena* arena) {
  return Arena::CreateInternal< ::inGameCmd >(arena);
}
}  // namespace protobuf
}  // namespace google

// @@protoc_insertion_point(global_scope)
#include <google/protobuf/port_undef.inc>
