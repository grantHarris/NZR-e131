// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: frame.proto

#include "frame.pb.h"

#include <algorithm>

#include <google/protobuf/stubs/common.h>
#include <google/protobuf/stubs/port.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/wire_format_lite_inl.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/reflection_ops.h>
#include <google/protobuf/wire_format.h>
// This is a temporary google only hack
#ifdef GOOGLE_PROTOBUF_ENFORCE_UNIQUENESS
#include "third_party/protobuf/version.h"
#endif
// @@protoc_insertion_point(includes)

namespace protobuf_frame_2eproto {
extern PROTOBUF_INTERNAL_EXPORT_protobuf_frame_2eproto ::google::protobuf::internal::SCCInfo<0> scc_info_Pixel;
}  // namespace protobuf_frame_2eproto
namespace proto {
class PixelDefaultTypeInternal {
 public:
  ::google::protobuf::internal::ExplicitlyConstructed<Pixel>
      _instance;
} _Pixel_default_instance_;
class FrameDefaultTypeInternal {
 public:
  ::google::protobuf::internal::ExplicitlyConstructed<Frame>
      _instance;
} _Frame_default_instance_;
}  // namespace proto
namespace protobuf_frame_2eproto {
static void InitDefaultsPixel() {
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  {
    void* ptr = &::proto::_Pixel_default_instance_;
    new (ptr) ::proto::Pixel();
    ::google::protobuf::internal::OnShutdownDestroyMessage(ptr);
  }
  ::proto::Pixel::InitAsDefaultInstance();
}

::google::protobuf::internal::SCCInfo<0> scc_info_Pixel =
    {{ATOMIC_VAR_INIT(::google::protobuf::internal::SCCInfoBase::kUninitialized), 0, InitDefaultsPixel}, {}};

static void InitDefaultsFrame() {
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  {
    void* ptr = &::proto::_Frame_default_instance_;
    new (ptr) ::proto::Frame();
    ::google::protobuf::internal::OnShutdownDestroyMessage(ptr);
  }
  ::proto::Frame::InitAsDefaultInstance();
}

::google::protobuf::internal::SCCInfo<1> scc_info_Frame =
    {{ATOMIC_VAR_INIT(::google::protobuf::internal::SCCInfoBase::kUninitialized), 1, InitDefaultsFrame}, {
      &protobuf_frame_2eproto::scc_info_Pixel.base,}};

void InitDefaults() {
  ::google::protobuf::internal::InitSCC(&scc_info_Pixel.base);
  ::google::protobuf::internal::InitSCC(&scc_info_Frame.base);
}

::google::protobuf::Metadata file_level_metadata[2];

const ::google::protobuf::uint32 TableStruct::offsets[] GOOGLE_PROTOBUF_ATTRIBUTE_SECTION_VARIABLE(protodesc_cold) = {
  ~0u,  // no _has_bits_
  GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(::proto::Pixel, _internal_metadata_),
  ~0u,  // no _extensions_
  ~0u,  // no _oneof_case_
  ~0u,  // no _weak_field_map_
  GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(::proto::Pixel, r_),
  GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(::proto::Pixel, g_),
  GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(::proto::Pixel, b_),
  ~0u,  // no _has_bits_
  GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(::proto::Frame, _internal_metadata_),
  ~0u,  // no _extensions_
  ~0u,  // no _oneof_case_
  ~0u,  // no _weak_field_map_
  GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(::proto::Frame, pixels_),
};
static const ::google::protobuf::internal::MigrationSchema schemas[] GOOGLE_PROTOBUF_ATTRIBUTE_SECTION_VARIABLE(protodesc_cold) = {
  { 0, -1, sizeof(::proto::Pixel)},
  { 8, -1, sizeof(::proto::Frame)},
};

static ::google::protobuf::Message const * const file_default_instances[] = {
  reinterpret_cast<const ::google::protobuf::Message*>(&::proto::_Pixel_default_instance_),
  reinterpret_cast<const ::google::protobuf::Message*>(&::proto::_Frame_default_instance_),
};

void protobuf_AssignDescriptors() {
  AddDescriptors();
  AssignDescriptors(
      "frame.proto", schemas, file_default_instances, TableStruct::offsets,
      file_level_metadata, NULL, NULL);
}

void protobuf_AssignDescriptorsOnce() {
  static ::google::protobuf::internal::once_flag once;
  ::google::protobuf::internal::call_once(once, protobuf_AssignDescriptors);
}

void protobuf_RegisterTypes(const ::std::string&) GOOGLE_PROTOBUF_ATTRIBUTE_COLD;
void protobuf_RegisterTypes(const ::std::string&) {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::internal::RegisterAllTypes(file_level_metadata, 2);
}

void AddDescriptorsImpl() {
  InitDefaults();
  static const char descriptor[] GOOGLE_PROTOBUF_ATTRIBUTE_SECTION_VARIABLE(protodesc_cold) = {
      "\n\013frame.proto\022\005proto\"(\n\005Pixel\022\t\n\001r\030\001 \001(\005"
      "\022\t\n\001g\030\002 \001(\005\022\t\n\001b\030\003 \001(\005\"%\n\005Frame\022\034\n\006pixel"
      "s\030\001 \003(\0132\014.proto.Pixelb\006proto3"
  };
  ::google::protobuf::DescriptorPool::InternalAddGeneratedFile(
      descriptor, 109);
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedFile(
    "frame.proto", &protobuf_RegisterTypes);
}

void AddDescriptors() {
  static ::google::protobuf::internal::once_flag once;
  ::google::protobuf::internal::call_once(once, AddDescriptorsImpl);
}
// Force AddDescriptors() to be called at dynamic initialization time.
struct StaticDescriptorInitializer {
  StaticDescriptorInitializer() {
    AddDescriptors();
  }
} static_descriptor_initializer;
}  // namespace protobuf_frame_2eproto
namespace proto {

// ===================================================================

void Pixel::InitAsDefaultInstance() {
}
#if !defined(_MSC_VER) || _MSC_VER >= 1900
const int Pixel::kRFieldNumber;
const int Pixel::kGFieldNumber;
const int Pixel::kBFieldNumber;
#endif  // !defined(_MSC_VER) || _MSC_VER >= 1900

Pixel::Pixel()
  : ::google::protobuf::Message(), _internal_metadata_(NULL) {
  ::google::protobuf::internal::InitSCC(
      &protobuf_frame_2eproto::scc_info_Pixel.base);
  SharedCtor();
  // @@protoc_insertion_point(constructor:proto.Pixel)
}
Pixel::Pixel(const Pixel& from)
  : ::google::protobuf::Message(),
      _internal_metadata_(NULL) {
  _internal_metadata_.MergeFrom(from._internal_metadata_);
  ::memcpy(&r_, &from.r_,
    static_cast<size_t>(reinterpret_cast<char*>(&b_) -
    reinterpret_cast<char*>(&r_)) + sizeof(b_));
  // @@protoc_insertion_point(copy_constructor:proto.Pixel)
}

void Pixel::SharedCtor() {
  ::memset(&r_, 0, static_cast<size_t>(
      reinterpret_cast<char*>(&b_) -
      reinterpret_cast<char*>(&r_)) + sizeof(b_));
}

Pixel::~Pixel() {
  // @@protoc_insertion_point(destructor:proto.Pixel)
  SharedDtor();
}

void Pixel::SharedDtor() {
}

void Pixel::SetCachedSize(int size) const {
  _cached_size_.Set(size);
}
const ::google::protobuf::Descriptor* Pixel::descriptor() {
  ::protobuf_frame_2eproto::protobuf_AssignDescriptorsOnce();
  return ::protobuf_frame_2eproto::file_level_metadata[kIndexInFileMessages].descriptor;
}

const Pixel& Pixel::default_instance() {
  ::google::protobuf::internal::InitSCC(&protobuf_frame_2eproto::scc_info_Pixel.base);
  return *internal_default_instance();
}


void Pixel::Clear() {
// @@protoc_insertion_point(message_clear_start:proto.Pixel)
  ::google::protobuf::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  ::memset(&r_, 0, static_cast<size_t>(
      reinterpret_cast<char*>(&b_) -
      reinterpret_cast<char*>(&r_)) + sizeof(b_));
  _internal_metadata_.Clear();
}

bool Pixel::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!GOOGLE_PREDICT_TRUE(EXPRESSION)) goto failure
  ::google::protobuf::uint32 tag;
  // @@protoc_insertion_point(parse_start:proto.Pixel)
  for (;;) {
    ::std::pair<::google::protobuf::uint32, bool> p = input->ReadTagWithCutoffNoLastTag(127u);
    tag = p.first;
    if (!p.second) goto handle_unusual;
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // int32 r = 1;
      case 1: {
        if (static_cast< ::google::protobuf::uint8>(tag) ==
            static_cast< ::google::protobuf::uint8>(8u /* 8 & 0xFF */)) {

          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int32, ::google::protobuf::internal::WireFormatLite::TYPE_INT32>(
                 input, &r_)));
        } else {
          goto handle_unusual;
        }
        break;
      }

      // int32 g = 2;
      case 2: {
        if (static_cast< ::google::protobuf::uint8>(tag) ==
            static_cast< ::google::protobuf::uint8>(16u /* 16 & 0xFF */)) {

          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int32, ::google::protobuf::internal::WireFormatLite::TYPE_INT32>(
                 input, &g_)));
        } else {
          goto handle_unusual;
        }
        break;
      }

      // int32 b = 3;
      case 3: {
        if (static_cast< ::google::protobuf::uint8>(tag) ==
            static_cast< ::google::protobuf::uint8>(24u /* 24 & 0xFF */)) {

          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int32, ::google::protobuf::internal::WireFormatLite::TYPE_INT32>(
                 input, &b_)));
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
  // @@protoc_insertion_point(parse_success:proto.Pixel)
  return true;
failure:
  // @@protoc_insertion_point(parse_failure:proto.Pixel)
  return false;
#undef DO_
}

void Pixel::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // @@protoc_insertion_point(serialize_start:proto.Pixel)
  ::google::protobuf::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  // int32 r = 1;
  if (this->r() != 0) {
    ::google::protobuf::internal::WireFormatLite::WriteInt32(1, this->r(), output);
  }

  // int32 g = 2;
  if (this->g() != 0) {
    ::google::protobuf::internal::WireFormatLite::WriteInt32(2, this->g(), output);
  }

  // int32 b = 3;
  if (this->b() != 0) {
    ::google::protobuf::internal::WireFormatLite::WriteInt32(3, this->b(), output);
  }

  if ((_internal_metadata_.have_unknown_fields() &&  ::google::protobuf::internal::GetProto3PreserveUnknownsDefault())) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        (::google::protobuf::internal::GetProto3PreserveUnknownsDefault()   ? _internal_metadata_.unknown_fields()   : _internal_metadata_.default_instance()), output);
  }
  // @@protoc_insertion_point(serialize_end:proto.Pixel)
}

::google::protobuf::uint8* Pixel::InternalSerializeWithCachedSizesToArray(
    bool deterministic, ::google::protobuf::uint8* target) const {
  (void)deterministic; // Unused
  // @@protoc_insertion_point(serialize_to_array_start:proto.Pixel)
  ::google::protobuf::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  // int32 r = 1;
  if (this->r() != 0) {
    target = ::google::protobuf::internal::WireFormatLite::WriteInt32ToArray(1, this->r(), target);
  }

  // int32 g = 2;
  if (this->g() != 0) {
    target = ::google::protobuf::internal::WireFormatLite::WriteInt32ToArray(2, this->g(), target);
  }

  // int32 b = 3;
  if (this->b() != 0) {
    target = ::google::protobuf::internal::WireFormatLite::WriteInt32ToArray(3, this->b(), target);
  }

  if ((_internal_metadata_.have_unknown_fields() &&  ::google::protobuf::internal::GetProto3PreserveUnknownsDefault())) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        (::google::protobuf::internal::GetProto3PreserveUnknownsDefault()   ? _internal_metadata_.unknown_fields()   : _internal_metadata_.default_instance()), target);
  }
  // @@protoc_insertion_point(serialize_to_array_end:proto.Pixel)
  return target;
}

size_t Pixel::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:proto.Pixel)
  size_t total_size = 0;

  if ((_internal_metadata_.have_unknown_fields() &&  ::google::protobuf::internal::GetProto3PreserveUnknownsDefault())) {
    total_size +=
      ::google::protobuf::internal::WireFormat::ComputeUnknownFieldsSize(
        (::google::protobuf::internal::GetProto3PreserveUnknownsDefault()   ? _internal_metadata_.unknown_fields()   : _internal_metadata_.default_instance()));
  }
  // int32 r = 1;
  if (this->r() != 0) {
    total_size += 1 +
      ::google::protobuf::internal::WireFormatLite::Int32Size(
        this->r());
  }

  // int32 g = 2;
  if (this->g() != 0) {
    total_size += 1 +
      ::google::protobuf::internal::WireFormatLite::Int32Size(
        this->g());
  }

  // int32 b = 3;
  if (this->b() != 0) {
    total_size += 1 +
      ::google::protobuf::internal::WireFormatLite::Int32Size(
        this->b());
  }

  int cached_size = ::google::protobuf::internal::ToCachedSize(total_size);
  SetCachedSize(cached_size);
  return total_size;
}

void Pixel::MergeFrom(const ::google::protobuf::Message& from) {
// @@protoc_insertion_point(generalized_merge_from_start:proto.Pixel)
  GOOGLE_DCHECK_NE(&from, this);
  const Pixel* source =
      ::google::protobuf::internal::DynamicCastToGenerated<const Pixel>(
          &from);
  if (source == NULL) {
  // @@protoc_insertion_point(generalized_merge_from_cast_fail:proto.Pixel)
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
  // @@protoc_insertion_point(generalized_merge_from_cast_success:proto.Pixel)
    MergeFrom(*source);
  }
}

void Pixel::MergeFrom(const Pixel& from) {
// @@protoc_insertion_point(class_specific_merge_from_start:proto.Pixel)
  GOOGLE_DCHECK_NE(&from, this);
  _internal_metadata_.MergeFrom(from._internal_metadata_);
  ::google::protobuf::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  if (from.r() != 0) {
    set_r(from.r());
  }
  if (from.g() != 0) {
    set_g(from.g());
  }
  if (from.b() != 0) {
    set_b(from.b());
  }
}

void Pixel::CopyFrom(const ::google::protobuf::Message& from) {
// @@protoc_insertion_point(generalized_copy_from_start:proto.Pixel)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void Pixel::CopyFrom(const Pixel& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:proto.Pixel)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool Pixel::IsInitialized() const {
  return true;
}

void Pixel::Swap(Pixel* other) {
  if (other == this) return;
  InternalSwap(other);
}
void Pixel::InternalSwap(Pixel* other) {
  using std::swap;
  swap(r_, other->r_);
  swap(g_, other->g_);
  swap(b_, other->b_);
  _internal_metadata_.Swap(&other->_internal_metadata_);
}

::google::protobuf::Metadata Pixel::GetMetadata() const {
  protobuf_frame_2eproto::protobuf_AssignDescriptorsOnce();
  return ::protobuf_frame_2eproto::file_level_metadata[kIndexInFileMessages];
}


// ===================================================================

void Frame::InitAsDefaultInstance() {
}
#if !defined(_MSC_VER) || _MSC_VER >= 1900
const int Frame::kPixelsFieldNumber;
#endif  // !defined(_MSC_VER) || _MSC_VER >= 1900

Frame::Frame()
  : ::google::protobuf::Message(), _internal_metadata_(NULL) {
  ::google::protobuf::internal::InitSCC(
      &protobuf_frame_2eproto::scc_info_Frame.base);
  SharedCtor();
  // @@protoc_insertion_point(constructor:proto.Frame)
}
Frame::Frame(const Frame& from)
  : ::google::protobuf::Message(),
      _internal_metadata_(NULL),
      pixels_(from.pixels_) {
  _internal_metadata_.MergeFrom(from._internal_metadata_);
  // @@protoc_insertion_point(copy_constructor:proto.Frame)
}

void Frame::SharedCtor() {
}

Frame::~Frame() {
  // @@protoc_insertion_point(destructor:proto.Frame)
  SharedDtor();
}

void Frame::SharedDtor() {
}

void Frame::SetCachedSize(int size) const {
  _cached_size_.Set(size);
}
const ::google::protobuf::Descriptor* Frame::descriptor() {
  ::protobuf_frame_2eproto::protobuf_AssignDescriptorsOnce();
  return ::protobuf_frame_2eproto::file_level_metadata[kIndexInFileMessages].descriptor;
}

const Frame& Frame::default_instance() {
  ::google::protobuf::internal::InitSCC(&protobuf_frame_2eproto::scc_info_Frame.base);
  return *internal_default_instance();
}


void Frame::Clear() {
// @@protoc_insertion_point(message_clear_start:proto.Frame)
  ::google::protobuf::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  pixels_.Clear();
  _internal_metadata_.Clear();
}

bool Frame::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!GOOGLE_PREDICT_TRUE(EXPRESSION)) goto failure
  ::google::protobuf::uint32 tag;
  // @@protoc_insertion_point(parse_start:proto.Frame)
  for (;;) {
    ::std::pair<::google::protobuf::uint32, bool> p = input->ReadTagWithCutoffNoLastTag(127u);
    tag = p.first;
    if (!p.second) goto handle_unusual;
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // repeated .proto.Pixel pixels = 1;
      case 1: {
        if (static_cast< ::google::protobuf::uint8>(tag) ==
            static_cast< ::google::protobuf::uint8>(10u /* 10 & 0xFF */)) {
          DO_(::google::protobuf::internal::WireFormatLite::ReadMessage(
                input, add_pixels()));
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
  // @@protoc_insertion_point(parse_success:proto.Frame)
  return true;
failure:
  // @@protoc_insertion_point(parse_failure:proto.Frame)
  return false;
#undef DO_
}

void Frame::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // @@protoc_insertion_point(serialize_start:proto.Frame)
  ::google::protobuf::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  // repeated .proto.Pixel pixels = 1;
  for (unsigned int i = 0,
      n = static_cast<unsigned int>(this->pixels_size()); i < n; i++) {
    ::google::protobuf::internal::WireFormatLite::WriteMessageMaybeToArray(
      1,
      this->pixels(static_cast<int>(i)),
      output);
  }

  if ((_internal_metadata_.have_unknown_fields() &&  ::google::protobuf::internal::GetProto3PreserveUnknownsDefault())) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        (::google::protobuf::internal::GetProto3PreserveUnknownsDefault()   ? _internal_metadata_.unknown_fields()   : _internal_metadata_.default_instance()), output);
  }
  // @@protoc_insertion_point(serialize_end:proto.Frame)
}

::google::protobuf::uint8* Frame::InternalSerializeWithCachedSizesToArray(
    bool deterministic, ::google::protobuf::uint8* target) const {
  (void)deterministic; // Unused
  // @@protoc_insertion_point(serialize_to_array_start:proto.Frame)
  ::google::protobuf::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  // repeated .proto.Pixel pixels = 1;
  for (unsigned int i = 0,
      n = static_cast<unsigned int>(this->pixels_size()); i < n; i++) {
    target = ::google::protobuf::internal::WireFormatLite::
      InternalWriteMessageToArray(
        1, this->pixels(static_cast<int>(i)), deterministic, target);
  }

  if ((_internal_metadata_.have_unknown_fields() &&  ::google::protobuf::internal::GetProto3PreserveUnknownsDefault())) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        (::google::protobuf::internal::GetProto3PreserveUnknownsDefault()   ? _internal_metadata_.unknown_fields()   : _internal_metadata_.default_instance()), target);
  }
  // @@protoc_insertion_point(serialize_to_array_end:proto.Frame)
  return target;
}

size_t Frame::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:proto.Frame)
  size_t total_size = 0;

  if ((_internal_metadata_.have_unknown_fields() &&  ::google::protobuf::internal::GetProto3PreserveUnknownsDefault())) {
    total_size +=
      ::google::protobuf::internal::WireFormat::ComputeUnknownFieldsSize(
        (::google::protobuf::internal::GetProto3PreserveUnknownsDefault()   ? _internal_metadata_.unknown_fields()   : _internal_metadata_.default_instance()));
  }
  // repeated .proto.Pixel pixels = 1;
  {
    unsigned int count = static_cast<unsigned int>(this->pixels_size());
    total_size += 1UL * count;
    for (unsigned int i = 0; i < count; i++) {
      total_size +=
        ::google::protobuf::internal::WireFormatLite::MessageSize(
          this->pixels(static_cast<int>(i)));
    }
  }

  int cached_size = ::google::protobuf::internal::ToCachedSize(total_size);
  SetCachedSize(cached_size);
  return total_size;
}

void Frame::MergeFrom(const ::google::protobuf::Message& from) {
// @@protoc_insertion_point(generalized_merge_from_start:proto.Frame)
  GOOGLE_DCHECK_NE(&from, this);
  const Frame* source =
      ::google::protobuf::internal::DynamicCastToGenerated<const Frame>(
          &from);
  if (source == NULL) {
  // @@protoc_insertion_point(generalized_merge_from_cast_fail:proto.Frame)
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
  // @@protoc_insertion_point(generalized_merge_from_cast_success:proto.Frame)
    MergeFrom(*source);
  }
}

void Frame::MergeFrom(const Frame& from) {
// @@protoc_insertion_point(class_specific_merge_from_start:proto.Frame)
  GOOGLE_DCHECK_NE(&from, this);
  _internal_metadata_.MergeFrom(from._internal_metadata_);
  ::google::protobuf::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  pixels_.MergeFrom(from.pixels_);
}

void Frame::CopyFrom(const ::google::protobuf::Message& from) {
// @@protoc_insertion_point(generalized_copy_from_start:proto.Frame)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void Frame::CopyFrom(const Frame& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:proto.Frame)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool Frame::IsInitialized() const {
  return true;
}

void Frame::Swap(Frame* other) {
  if (other == this) return;
  InternalSwap(other);
}
void Frame::InternalSwap(Frame* other) {
  using std::swap;
  CastToBase(&pixels_)->InternalSwap(CastToBase(&other->pixels_));
  _internal_metadata_.Swap(&other->_internal_metadata_);
}

::google::protobuf::Metadata Frame::GetMetadata() const {
  protobuf_frame_2eproto::protobuf_AssignDescriptorsOnce();
  return ::protobuf_frame_2eproto::file_level_metadata[kIndexInFileMessages];
}


// @@protoc_insertion_point(namespace_scope)
}  // namespace proto
namespace google {
namespace protobuf {
template<> GOOGLE_PROTOBUF_ATTRIBUTE_NOINLINE ::proto::Pixel* Arena::CreateMaybeMessage< ::proto::Pixel >(Arena* arena) {
  return Arena::CreateInternal< ::proto::Pixel >(arena);
}
template<> GOOGLE_PROTOBUF_ATTRIBUTE_NOINLINE ::proto::Frame* Arena::CreateMaybeMessage< ::proto::Frame >(Arena* arena) {
  return Arena::CreateInternal< ::proto::Frame >(arena);
}
}  // namespace protobuf
}  // namespace google

// @@protoc_insertion_point(global_scope)
