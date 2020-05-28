#pragma once
#include "SizeAppender.h"
#include "SliceReader.h"
#include "WriteAppender.h"
#include "meta19/Type.h"

#include <type_traits> // std::is_arithmetic_v, std::is_enum_v

namespace serialize19 {

using meta19::type;

enum class ArchiveMode { Read, Write, Size };

template<class T> constexpr auto is_serialize_primitive = std::is_arithmetic_v<T> || std::is_enum_v<T>;

struct SerializeFallback {
    template<class T> SerializeFallback(const T&) {
        static_assert(!std::is_same_v<T, T>, "Missing serialize overload!");
    }
};
template<class Archive> auto serialize(Archive&, SerializeFallback) {}

template<class Archive, class T> auto serialize(Archive& a, T& v) -> std::enable_if_t<is_serialize_primitive<T>> {
    a.withPrimitive(v);
}

template<EndianBehaviour endian = EndianBehaviour::Keep> struct ReadArchive {
    static constexpr auto mode = ArchiveMode::Read;

    ReadArchive(BufferSlice slice) : m_slice(slice) {}

    auto slice() const -> BufferSlice { return m_slice; }

    template<class T> void withPrimitive(T& value) {
        value = m_slice.as(type<T>);
        m_slice = m_slice.skipOf(type<T>);
    }

private:
    SliceReader<endian> m_slice;
};

struct SizeArchive {
    static constexpr auto mode = ArchiveMode::Size;

    constexpr auto size() const -> size_t { return m_appender.count(); }

    template<class T> void withPrimitive(const T& value) { m_appender = m_appender.appendValue(value); }

private:
    SizeAppender m_appender;
};

template<EndianBehaviour endian = EndianBehaviour::Keep> struct WriteToArchive {
    static constexpr auto mode = ArchiveMode::Write;

    WriteToArchive(SliceOf<uint8_t> slice) : m_appender(slice.begin()) {}

    template<class T> void withPrimitive(const T& value) { m_appender = m_appender.appendValue(value); }

private:
    WriteAppender<endian> m_appender;
};

// Writing wont modify the value, but serialize methods can only have one signature
template<class T> void serialize(SizeArchive& wa, const T& v) { serialize(wa, const_cast<T&>(v)); }
template<class T, EndianBehaviour endian> void serialize(WriteToArchive<endian>& wa, const T& v) {
    serialize(wa, const_cast<T&>(v));
}

} // namespace serialize19
