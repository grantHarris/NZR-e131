// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: frame.proto

#ifndef PROTOBUF_INCLUDED_frame_2eproto
#define PROTOBUF_INCLUDED_frame_2eproto

#include <string>

#include <google/protobuf/stubs/common.h>

#if GOOGLE_PROTOBUF_VERSION < 3005000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please update
#error your headers.
#endif
#if 3005001 < GOOGLE_PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please
#error regenerate this file with a newer version of protoc.
#endif

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
#include <google/protobuf/unknown_field_set.h>
// @@protoc_insertion_point(includes)
#define PROTOBUF_INTERNAL_EXPORT_protobuf_frame_2eproto 

namespace protobuf_frame_2eproto {
// Internal implementation detail -- do not use these members.
struct TableStruct {
  static const ::google::protobuf::internal::ParseTableField entries[];
  static const ::google::protobuf::internal::AuxillaryParseTableField aux[];
  static const ::google::protobuf::internal::ParseTable schema[2];
  static const ::google::protobuf::internal::FieldMetadata field_metadata[];
  static const ::google::protobuf::internal::SerializationTable serialization_table[];
  static const ::google::protobuf::uint32 offsets[];
};
void AddDescriptors();
}  // namespace protobuf_frame_2eproto
namespace proto {
class Frame;
class FrameDefaultTypeInternal;
extern FrameDefaultTypeInternal _Frame_default_instance_;
class Pixel;
class PixelDefaultTypeInternal;
extern PixelDefaultTypeInternal _Pixel_default_instance_;
}  // namespace proto
namespace google {
namespace protobuf {
template<> ::proto::Frame* Arena::CreateMaybeMessage<::proto::Frame>(Arena*);
template<> ::proto::Pixel* Arena::CreateMaybeMessage<::proto::Pixel>(Arena*);
}  // namespace protobuf
}  // namespace google
namespace proto {

// ===================================================================

class Pixel : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:proto.Pixel) */ {
 public:
  Pixel();
  virtual ~Pixel();

  Pixel(const Pixel& from);

  inline Pixel& operator=(const Pixel& from) {
    CopyFrom(from);
    return *this;
  }
  #if LANG_CXX11
  Pixel(Pixel&& from) noexcept
    : Pixel() {
    *this = ::std::move(from);
  }

  inline Pixel& operator=(Pixel&& from) noexcept {
    if (GetArenaNoVirtual() == from.GetArenaNoVirtual()) {
      if (this != &from) InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }
  #endif
  static const ::google::protobuf::Descriptor* descriptor();
  static const Pixel& default_instance();

  static void InitAsDefaultInstance();  // FOR INTERNAL USE ONLY
  static inline const Pixel* internal_default_instance() {
    return reinterpret_cast<const Pixel*>(
               &_Pixel_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    0;

  void Swap(Pixel* other);
  friend void swap(Pixel& a, Pixel& b) {
    a.Swap(&b);
  }

  // implements Message ----------------------------------------------

  inline Pixel* New() const final {
    return CreateMaybeMessage<Pixel>(NULL);
  }

  Pixel* New(::google::protobuf::Arena* arena) const final {
    return CreateMaybeMessage<Pixel>(arena);
  }
  void CopyFrom(const ::google::protobuf::Message& from) final;
  void MergeFrom(const ::google::protobuf::Message& from) final;
  void CopyFrom(const Pixel& from);
  void MergeFrom(const Pixel& from);
  void Clear() final;
  bool IsInitialized() const final;

  size_t ByteSizeLong() const final;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input) final;
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const final;
  ::google::protobuf::uint8* InternalSerializeWithCachedSizesToArray(
      bool deterministic, ::google::protobuf::uint8* target) const final;
  int GetCachedSize() const final { return _cached_size_.Get(); }

  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const final;
  void InternalSwap(Pixel* other);
  private:
  inline ::google::protobuf::Arena* GetArenaNoVirtual() const {
    return NULL;
  }
  inline void* MaybeArenaPtr() const {
    return NULL;
  }
  public:

  ::google::protobuf::Metadata GetMetadata() const final;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // int32 r = 1;
  void clear_r();
  static const int kRFieldNumber = 1;
  ::google::protobuf::int32 r() const;
  void set_r(::google::protobuf::int32 value);

  // int32 g = 2;
  void clear_g();
  static const int kGFieldNumber = 2;
  ::google::protobuf::int32 g() const;
  void set_g(::google::protobuf::int32 value);

  // int32 b = 3;
  void clear_b();
  static const int kBFieldNumber = 3;
  ::google::protobuf::int32 b() const;
  void set_b(::google::protobuf::int32 value);

  // @@protoc_insertion_point(class_scope:proto.Pixel)
 private:

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  ::google::protobuf::int32 r_;
  ::google::protobuf::int32 g_;
  ::google::protobuf::int32 b_;
  mutable ::google::protobuf::internal::CachedSize _cached_size_;
  friend struct ::protobuf_frame_2eproto::TableStruct;
};
// -------------------------------------------------------------------

class Frame : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:proto.Frame) */ {
 public:
  Frame();
  virtual ~Frame();

  Frame(const Frame& from);

  inline Frame& operator=(const Frame& from) {
    CopyFrom(from);
    return *this;
  }
  #if LANG_CXX11
  Frame(Frame&& from) noexcept
    : Frame() {
    *this = ::std::move(from);
  }

  inline Frame& operator=(Frame&& from) noexcept {
    if (GetArenaNoVirtual() == from.GetArenaNoVirtual()) {
      if (this != &from) InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }
  #endif
  static const ::google::protobuf::Descriptor* descriptor();
  static const Frame& default_instance();

  static void InitAsDefaultInstance();  // FOR INTERNAL USE ONLY
  static inline const Frame* internal_default_instance() {
    return reinterpret_cast<const Frame*>(
               &_Frame_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    1;

  void Swap(Frame* other);
  friend void swap(Frame& a, Frame& b) {
    a.Swap(&b);
  }

  // implements Message ----------------------------------------------

  inline Frame* New() const final {
    return CreateMaybeMessage<Frame>(NULL);
  }

  Frame* New(::google::protobuf::Arena* arena) const final {
    return CreateMaybeMessage<Frame>(arena);
  }
  void CopyFrom(const ::google::protobuf::Message& from) final;
  void MergeFrom(const ::google::protobuf::Message& from) final;
  void CopyFrom(const Frame& from);
  void MergeFrom(const Frame& from);
  void Clear() final;
  bool IsInitialized() const final;

  size_t ByteSizeLong() const final;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input) final;
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const final;
  ::google::protobuf::uint8* InternalSerializeWithCachedSizesToArray(
      bool deterministic, ::google::protobuf::uint8* target) const final;
  int GetCachedSize() const final { return _cached_size_.Get(); }

  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const final;
  void InternalSwap(Frame* other);
  private:
  inline ::google::protobuf::Arena* GetArenaNoVirtual() const {
    return NULL;
  }
  inline void* MaybeArenaPtr() const {
    return NULL;
  }
  public:

  ::google::protobuf::Metadata GetMetadata() const final;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // repeated .proto.Pixel pixels = 1;
  int pixels_size() const;
  void clear_pixels();
  static const int kPixelsFieldNumber = 1;
  ::proto::Pixel* mutable_pixels(int index);
  ::google::protobuf::RepeatedPtrField< ::proto::Pixel >*
      mutable_pixels();
  const ::proto::Pixel& pixels(int index) const;
  ::proto::Pixel* add_pixels();
  const ::google::protobuf::RepeatedPtrField< ::proto::Pixel >&
      pixels() const;

  // @@protoc_insertion_point(class_scope:proto.Frame)
 private:

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  ::google::protobuf::RepeatedPtrField< ::proto::Pixel > pixels_;
  mutable ::google::protobuf::internal::CachedSize _cached_size_;
  friend struct ::protobuf_frame_2eproto::TableStruct;
};
// ===================================================================


// ===================================================================

#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
// Pixel

// int32 r = 1;
inline void Pixel::clear_r() {
  r_ = 0;
}
inline ::google::protobuf::int32 Pixel::r() const {
  // @@protoc_insertion_point(field_get:proto.Pixel.r)
  return r_;
}
inline void Pixel::set_r(::google::protobuf::int32 value) {
  
  r_ = value;
  // @@protoc_insertion_point(field_set:proto.Pixel.r)
}

// int32 g = 2;
inline void Pixel::clear_g() {
  g_ = 0;
}
inline ::google::protobuf::int32 Pixel::g() const {
  // @@protoc_insertion_point(field_get:proto.Pixel.g)
  return g_;
}
inline void Pixel::set_g(::google::protobuf::int32 value) {
  
  g_ = value;
  // @@protoc_insertion_point(field_set:proto.Pixel.g)
}

// int32 b = 3;
inline void Pixel::clear_b() {
  b_ = 0;
}
inline ::google::protobuf::int32 Pixel::b() const {
  // @@protoc_insertion_point(field_get:proto.Pixel.b)
  return b_;
}
inline void Pixel::set_b(::google::protobuf::int32 value) {
  
  b_ = value;
  // @@protoc_insertion_point(field_set:proto.Pixel.b)
}

// -------------------------------------------------------------------

// Frame

// repeated .proto.Pixel pixels = 1;
inline int Frame::pixels_size() const {
  return pixels_.size();
}
inline void Frame::clear_pixels() {
  pixels_.Clear();
}
inline ::proto::Pixel* Frame::mutable_pixels(int index) {
  // @@protoc_insertion_point(field_mutable:proto.Frame.pixels)
  return pixels_.Mutable(index);
}
inline ::google::protobuf::RepeatedPtrField< ::proto::Pixel >*
Frame::mutable_pixels() {
  // @@protoc_insertion_point(field_mutable_list:proto.Frame.pixels)
  return &pixels_;
}
inline const ::proto::Pixel& Frame::pixels(int index) const {
  // @@protoc_insertion_point(field_get:proto.Frame.pixels)
  return pixels_.Get(index);
}
inline ::proto::Pixel* Frame::add_pixels() {
  // @@protoc_insertion_point(field_add:proto.Frame.pixels)
  return pixels_.Add();
}
inline const ::google::protobuf::RepeatedPtrField< ::proto::Pixel >&
Frame::pixels() const {
  // @@protoc_insertion_point(field_list:proto.Frame.pixels)
  return pixels_;
}

#ifdef __GNUC__
  #pragma GCC diagnostic pop
#endif  // __GNUC__
// -------------------------------------------------------------------


// @@protoc_insertion_point(namespace_scope)

}  // namespace proto

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_INCLUDED_frame_2eproto
