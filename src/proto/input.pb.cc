// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: input.proto

#define INTERNAL_SUPPRESS_PROTOBUF_FIELD_DEPRECATION
#include "input.pb.h"

#include <algorithm>

#include <google/protobuf/stubs/common.h>
#include <google/protobuf/stubs/once.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/wire_format_lite_inl.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/reflection_ops.h>
#include <google/protobuf/wire_format.h>
// @@protoc_insertion_point(includes)

namespace {

const ::google::protobuf::Descriptor* Vector2d_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  Vector2d_reflection_ = NULL;
const ::google::protobuf::Descriptor* Input_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  Input_reflection_ = NULL;
const ::google::protobuf::EnumDescriptor* InputType_descriptor_ = NULL;

}  // namespace


void protobuf_AssignDesc_input_2eproto() {
  protobuf_AddDesc_input_2eproto();
  const ::google::protobuf::FileDescriptor* file =
    ::google::protobuf::DescriptorPool::generated_pool()->FindFileByName(
      "input.proto");
  GOOGLE_CHECK(file != NULL);
  Vector2d_descriptor_ = file->message_type(0);
  static const int Vector2d_offsets_[2] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Vector2d, x_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Vector2d, y_),
  };
  Vector2d_reflection_ =
    new ::google::protobuf::internal::GeneratedMessageReflection(
      Vector2d_descriptor_,
      Vector2d::default_instance_,
      Vector2d_offsets_,
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Vector2d, _has_bits_[0]),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Vector2d, _unknown_fields_),
      -1,
      ::google::protobuf::DescriptorPool::generated_pool(),
      ::google::protobuf::MessageFactory::generated_factory(),
      sizeof(Vector2d));
  Input_descriptor_ = file->message_type(1);
  static const int Input_offsets_[5] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Input, pid_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Input, type_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Input, pos_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Input, key_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Input, msg_),
  };
  Input_reflection_ =
    new ::google::protobuf::internal::GeneratedMessageReflection(
      Input_descriptor_,
      Input::default_instance_,
      Input_offsets_,
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Input, _has_bits_[0]),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Input, _unknown_fields_),
      -1,
      ::google::protobuf::DescriptorPool::generated_pool(),
      ::google::protobuf::MessageFactory::generated_factory(),
      sizeof(Input));
  InputType_descriptor_ = file->enum_type(0);
}

namespace {

GOOGLE_PROTOBUF_DECLARE_ONCE(protobuf_AssignDescriptors_once_);
inline void protobuf_AssignDescriptorsOnce() {
  ::google::protobuf::GoogleOnceInit(&protobuf_AssignDescriptors_once_,
                 &protobuf_AssignDesc_input_2eproto);
}

void protobuf_RegisterTypes(const ::std::string&) {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
    Vector2d_descriptor_, &Vector2d::default_instance());
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
    Input_descriptor_, &Input::default_instance());
}

}  // namespace

void protobuf_ShutdownFile_input_2eproto() {
  delete Vector2d::default_instance_;
  delete Vector2d_reflection_;
  delete Input::default_instance_;
  delete Input_reflection_;
}

void protobuf_AddDesc_input_2eproto() {
  static bool already_here = false;
  if (already_here) return;
  already_here = true;
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  ::google::protobuf::DescriptorPool::InternalAddGeneratedFile(
    "\n\013input.proto\" \n\010Vector2d\022\t\n\001x\030\001 \002(\005\022\t\n\001"
    "y\030\002 \002(\005\"`\n\005Input\022\013\n\003pid\030\001 \002(\r\022\030\n\004type\030\002 "
    "\002(\0162\n.InputType\022\026\n\003pos\030\003 \001(\0132\t.Vector2d\022"
    "\013\n\003key\030\004 \001(\005\022\013\n\003msg\030\005 \001(\014*\311\001\n\tInputType\022"
    "\031\n\025INPUT_LEFT_MOUSE_DOWN\020\000\022\027\n\023INPUT_LEFT"
    "_MOUSE_UP\020\001\022\032\n\026INPUT_RIGHT_MOUSE_DOWN\020\002\022"
    "\030\n\024INPUT_RIGHT_MOUSE_UP\020\003\022\024\n\020INPUT_MOUSE"
    "_MOVE\020\004\022\022\n\016INPUT_KEY_DOWN\020\005\022\020\n\014INPUT_KEY"
    "_UP\020\006\022\026\n\022INPUT_CHAT_MESSAGE\020\007", 349);
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedFile(
    "input.proto", &protobuf_RegisterTypes);
  Vector2d::default_instance_ = new Vector2d();
  Input::default_instance_ = new Input();
  Vector2d::default_instance_->InitAsDefaultInstance();
  Input::default_instance_->InitAsDefaultInstance();
  ::google::protobuf::internal::OnShutdown(&protobuf_ShutdownFile_input_2eproto);
}

// Force AddDescriptors() to be called at static initialization time.
struct StaticDescriptorInitializer_input_2eproto {
  StaticDescriptorInitializer_input_2eproto() {
    protobuf_AddDesc_input_2eproto();
  }
} static_descriptor_initializer_input_2eproto_;
const ::google::protobuf::EnumDescriptor* InputType_descriptor() {
  protobuf_AssignDescriptorsOnce();
  return InputType_descriptor_;
}
bool InputType_IsValid(int value) {
  switch(value) {
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


// ===================================================================

#ifndef _MSC_VER
const int Vector2d::kXFieldNumber;
const int Vector2d::kYFieldNumber;
#endif  // !_MSC_VER

Vector2d::Vector2d()
  : ::google::protobuf::Message() {
  SharedCtor();
}

void Vector2d::InitAsDefaultInstance() {
}

Vector2d::Vector2d(const Vector2d& from)
  : ::google::protobuf::Message() {
  SharedCtor();
  MergeFrom(from);
}

void Vector2d::SharedCtor() {
  _cached_size_ = 0;
  x_ = 0;
  y_ = 0;
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

Vector2d::~Vector2d() {
  SharedDtor();
}

void Vector2d::SharedDtor() {
  if (this != default_instance_) {
  }
}

void Vector2d::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* Vector2d::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return Vector2d_descriptor_;
}

const Vector2d& Vector2d::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_input_2eproto();
  return *default_instance_;
}

Vector2d* Vector2d::default_instance_ = NULL;

Vector2d* Vector2d::New() const {
  return new Vector2d;
}

void Vector2d::Clear() {
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    x_ = 0;
    y_ = 0;
  }
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  mutable_unknown_fields()->Clear();
}

bool Vector2d::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) return false
  ::google::protobuf::uint32 tag;
  while ((tag = input->ReadTag()) != 0) {
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // required int32 x = 1;
      case 1: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int32, ::google::protobuf::internal::WireFormatLite::TYPE_INT32>(
                 input, &x_)));
          set_has_x();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(16)) goto parse_y;
        break;
      }

      // required int32 y = 2;
      case 2: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_y:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int32, ::google::protobuf::internal::WireFormatLite::TYPE_INT32>(
                 input, &y_)));
          set_has_y();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectAtEnd()) return true;
        break;
      }

      default: {
      handle_uninterpreted:
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_END_GROUP) {
          return true;
        }
        DO_(::google::protobuf::internal::WireFormat::SkipField(
              input, tag, mutable_unknown_fields()));
        break;
      }
    }
  }
  return true;
#undef DO_
}

void Vector2d::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // required int32 x = 1;
  if (has_x()) {
    ::google::protobuf::internal::WireFormatLite::WriteInt32(1, this->x(), output);
  }

  // required int32 y = 2;
  if (has_y()) {
    ::google::protobuf::internal::WireFormatLite::WriteInt32(2, this->y(), output);
  }

  if (!unknown_fields().empty()) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        unknown_fields(), output);
  }
}

::google::protobuf::uint8* Vector2d::SerializeWithCachedSizesToArray(
    ::google::protobuf::uint8* target) const {
  // required int32 x = 1;
  if (has_x()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteInt32ToArray(1, this->x(), target);
  }

  // required int32 y = 2;
  if (has_y()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteInt32ToArray(2, this->y(), target);
  }

  if (!unknown_fields().empty()) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        unknown_fields(), target);
  }
  return target;
}

int Vector2d::ByteSize() const {
  int total_size = 0;

  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    // required int32 x = 1;
    if (has_x()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::Int32Size(
          this->x());
    }

    // required int32 y = 2;
    if (has_y()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::Int32Size(
          this->y());
    }

  }
  if (!unknown_fields().empty()) {
    total_size +=
      ::google::protobuf::internal::WireFormat::ComputeUnknownFieldsSize(
        unknown_fields());
  }
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = total_size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
  return total_size;
}

void Vector2d::MergeFrom(const ::google::protobuf::Message& from) {
  GOOGLE_CHECK_NE(&from, this);
  const Vector2d* source =
    ::google::protobuf::internal::dynamic_cast_if_available<const Vector2d*>(
      &from);
  if (source == NULL) {
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
    MergeFrom(*source);
  }
}

void Vector2d::MergeFrom(const Vector2d& from) {
  GOOGLE_CHECK_NE(&from, this);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from.has_x()) {
      set_x(from.x());
    }
    if (from.has_y()) {
      set_y(from.y());
    }
  }
  mutable_unknown_fields()->MergeFrom(from.unknown_fields());
}

void Vector2d::CopyFrom(const ::google::protobuf::Message& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void Vector2d::CopyFrom(const Vector2d& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool Vector2d::IsInitialized() const {
  if ((_has_bits_[0] & 0x00000003) != 0x00000003) return false;

  return true;
}

void Vector2d::Swap(Vector2d* other) {
  if (other != this) {
    std::swap(x_, other->x_);
    std::swap(y_, other->y_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    _unknown_fields_.Swap(&other->_unknown_fields_);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::google::protobuf::Metadata Vector2d::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = Vector2d_descriptor_;
  metadata.reflection = Vector2d_reflection_;
  return metadata;
}


// ===================================================================

#ifndef _MSC_VER
const int Input::kPidFieldNumber;
const int Input::kTypeFieldNumber;
const int Input::kPosFieldNumber;
const int Input::kKeyFieldNumber;
const int Input::kMsgFieldNumber;
#endif  // !_MSC_VER

Input::Input()
  : ::google::protobuf::Message() {
  SharedCtor();
}

void Input::InitAsDefaultInstance() {
  pos_ = const_cast< ::Vector2d*>(&::Vector2d::default_instance());
}

Input::Input(const Input& from)
  : ::google::protobuf::Message() {
  SharedCtor();
  MergeFrom(from);
}

void Input::SharedCtor() {
  _cached_size_ = 0;
  pid_ = 0u;
  type_ = 0;
  pos_ = NULL;
  key_ = 0;
  msg_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

Input::~Input() {
  SharedDtor();
}

void Input::SharedDtor() {
  if (msg_ != &::google::protobuf::internal::kEmptyString) {
    delete msg_;
  }
  if (this != default_instance_) {
    delete pos_;
  }
}

void Input::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* Input::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return Input_descriptor_;
}

const Input& Input::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_input_2eproto();
  return *default_instance_;
}

Input* Input::default_instance_ = NULL;

Input* Input::New() const {
  return new Input;
}

void Input::Clear() {
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    pid_ = 0u;
    type_ = 0;
    if (has_pos()) {
      if (pos_ != NULL) pos_->::Vector2d::Clear();
    }
    key_ = 0;
    if (has_msg()) {
      if (msg_ != &::google::protobuf::internal::kEmptyString) {
        msg_->clear();
      }
    }
  }
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  mutable_unknown_fields()->Clear();
}

bool Input::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) return false
  ::google::protobuf::uint32 tag;
  while ((tag = input->ReadTag()) != 0) {
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // required uint32 pid = 1;
      case 1: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::uint32, ::google::protobuf::internal::WireFormatLite::TYPE_UINT32>(
                 input, &pid_)));
          set_has_pid();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(16)) goto parse_type;
        break;
      }

      // required .InputType type = 2;
      case 2: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_type:
          int value;
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   int, ::google::protobuf::internal::WireFormatLite::TYPE_ENUM>(
                 input, &value)));
          if (::InputType_IsValid(value)) {
            set_type(static_cast< ::InputType >(value));
          } else {
            mutable_unknown_fields()->AddVarint(2, value);
          }
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(26)) goto parse_pos;
        break;
      }

      // optional .Vector2d pos = 3;
      case 3: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
         parse_pos:
          DO_(::google::protobuf::internal::WireFormatLite::ReadMessageNoVirtual(
               input, mutable_pos()));
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(32)) goto parse_key;
        break;
      }

      // optional int32 key = 4;
      case 4: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_key:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int32, ::google::protobuf::internal::WireFormatLite::TYPE_INT32>(
                 input, &key_)));
          set_has_key();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(42)) goto parse_msg;
        break;
      }

      // optional bytes msg = 5;
      case 5: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
         parse_msg:
          DO_(::google::protobuf::internal::WireFormatLite::ReadBytes(
                input, this->mutable_msg()));
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectAtEnd()) return true;
        break;
      }

      default: {
      handle_uninterpreted:
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_END_GROUP) {
          return true;
        }
        DO_(::google::protobuf::internal::WireFormat::SkipField(
              input, tag, mutable_unknown_fields()));
        break;
      }
    }
  }
  return true;
#undef DO_
}

void Input::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // required uint32 pid = 1;
  if (has_pid()) {
    ::google::protobuf::internal::WireFormatLite::WriteUInt32(1, this->pid(), output);
  }

  // required .InputType type = 2;
  if (has_type()) {
    ::google::protobuf::internal::WireFormatLite::WriteEnum(
      2, this->type(), output);
  }

  // optional .Vector2d pos = 3;
  if (has_pos()) {
    ::google::protobuf::internal::WireFormatLite::WriteMessageMaybeToArray(
      3, this->pos(), output);
  }

  // optional int32 key = 4;
  if (has_key()) {
    ::google::protobuf::internal::WireFormatLite::WriteInt32(4, this->key(), output);
  }

  // optional bytes msg = 5;
  if (has_msg()) {
    ::google::protobuf::internal::WireFormatLite::WriteBytes(
      5, this->msg(), output);
  }

  if (!unknown_fields().empty()) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        unknown_fields(), output);
  }
}

::google::protobuf::uint8* Input::SerializeWithCachedSizesToArray(
    ::google::protobuf::uint8* target) const {
  // required uint32 pid = 1;
  if (has_pid()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteUInt32ToArray(1, this->pid(), target);
  }

  // required .InputType type = 2;
  if (has_type()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteEnumToArray(
      2, this->type(), target);
  }

  // optional .Vector2d pos = 3;
  if (has_pos()) {
    target = ::google::protobuf::internal::WireFormatLite::
      WriteMessageNoVirtualToArray(
        3, this->pos(), target);
  }

  // optional int32 key = 4;
  if (has_key()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteInt32ToArray(4, this->key(), target);
  }

  // optional bytes msg = 5;
  if (has_msg()) {
    target =
      ::google::protobuf::internal::WireFormatLite::WriteBytesToArray(
        5, this->msg(), target);
  }

  if (!unknown_fields().empty()) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        unknown_fields(), target);
  }
  return target;
}

int Input::ByteSize() const {
  int total_size = 0;

  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    // required uint32 pid = 1;
    if (has_pid()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::UInt32Size(
          this->pid());
    }

    // required .InputType type = 2;
    if (has_type()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::EnumSize(this->type());
    }

    // optional .Vector2d pos = 3;
    if (has_pos()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::MessageSizeNoVirtual(
          this->pos());
    }

    // optional int32 key = 4;
    if (has_key()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::Int32Size(
          this->key());
    }

    // optional bytes msg = 5;
    if (has_msg()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::BytesSize(
          this->msg());
    }

  }
  if (!unknown_fields().empty()) {
    total_size +=
      ::google::protobuf::internal::WireFormat::ComputeUnknownFieldsSize(
        unknown_fields());
  }
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = total_size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
  return total_size;
}

void Input::MergeFrom(const ::google::protobuf::Message& from) {
  GOOGLE_CHECK_NE(&from, this);
  const Input* source =
    ::google::protobuf::internal::dynamic_cast_if_available<const Input*>(
      &from);
  if (source == NULL) {
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
    MergeFrom(*source);
  }
}

void Input::MergeFrom(const Input& from) {
  GOOGLE_CHECK_NE(&from, this);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from.has_pid()) {
      set_pid(from.pid());
    }
    if (from.has_type()) {
      set_type(from.type());
    }
    if (from.has_pos()) {
      mutable_pos()->::Vector2d::MergeFrom(from.pos());
    }
    if (from.has_key()) {
      set_key(from.key());
    }
    if (from.has_msg()) {
      set_msg(from.msg());
    }
  }
  mutable_unknown_fields()->MergeFrom(from.unknown_fields());
}

void Input::CopyFrom(const ::google::protobuf::Message& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void Input::CopyFrom(const Input& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool Input::IsInitialized() const {
  if ((_has_bits_[0] & 0x00000003) != 0x00000003) return false;

  if (has_pos()) {
    if (!this->pos().IsInitialized()) return false;
  }
  return true;
}

void Input::Swap(Input* other) {
  if (other != this) {
    std::swap(pid_, other->pid_);
    std::swap(type_, other->type_);
    std::swap(pos_, other->pos_);
    std::swap(key_, other->key_);
    std::swap(msg_, other->msg_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    _unknown_fields_.Swap(&other->_unknown_fields_);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::google::protobuf::Metadata Input::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = Input_descriptor_;
  metadata.reflection = Input_reflection_;
  return metadata;
}


// @@protoc_insertion_point(namespace_scope)

// @@protoc_insertion_point(global_scope)
