#pragma once
#include "BufferSpan.h"
#include "Endianess.h"
#include "meta19/Type.h"

#include <stddef.h> // size_t

namespace serialize19 {

using meta19::Type;

namespace details {

struct SpanReaderCommon {
    constexpr SpanReaderCommon() = default;
    constexpr explicit SpanReaderCommon(const uint8_t* data, size_t count) : m_data(data), m_count(count) {}
    constexpr SpanReaderCommon(BufferSpan span) : SpanReaderCommon(span.begin(), span.count()) {}

    constexpr auto count() const -> size_t { return m_count; }
    constexpr operator BufferSpan() const { return BufferSpan{m_data, m_count}; }

    auto sliceBytes(size_t count) const -> BufferSpan { return Span{m_data, count}; }
    constexpr auto limitBytes(size_t count) const -> SpanReaderCommon { return SpanReaderCommon{m_data, count}; }

    constexpr auto skipBytes(size_t count) const -> SpanReaderCommon {
        return SpanReaderCommon{m_data + count, m_count - count};
    }
    template<class T> constexpr auto skipOf(Type<T>* = {}) const -> SpanReaderCommon {
        return SpanReaderCommon{m_data + sizeof(T), m_count - sizeof(T)};
    }
    template<class T> constexpr auto skipCountOf(size_t count, Type<T>* = {}) const -> SpanReaderCommon {
        return SpanReaderCommon{m_data + count * sizeof(T), m_count - count * sizeof(T)};
    }

protected:
    const uint8_t* m_data{};
    size_t m_count{};
};

} // namespace details

template<EndianBehaviour = EndianBehaviour::Keep> struct SpanReader;

template<> struct SpanReader<EndianBehaviour::Keep> final : details::SpanReaderCommon {
    using details::SpanReaderCommon::SpanReaderCommon;
    using details::SpanReaderCommon::operator=;

    template<class T> constexpr auto as(Type<T>* = {}) const -> T { return *(const T*)m_data; }
    template<class T> auto sliceCountOf(size_t count, Type<T>* = {}) const -> Span<const T> {
        return {(const T*)m_data, count};
    }
    template<class T, class F> void eachCountOf(size_t count, Type<T>*, F&& f) const {
        auto p = (const T*)m_data;
        for (auto i = 0u; i < count; ++i) f(*p++);
    }
};

template<> struct SpanReader<EndianBehaviour::Flip> final : details::SpanReaderCommon {
    using details::SpanReaderCommon::SpanReaderCommon;
    using details::SpanReaderCommon::operator=;

    template<class T> constexpr auto as(Type<T>* = {}) const -> T { return endianFlipFor<T>(*(const T*)m_data); }

    template<class T, class F> void eachCountOf(size_t count, Type<T>*, F&& f) const {
        auto p = (const T*)m_data;
        for (auto i = 0u; i < count; ++i) f(endianFlipFor<T>(*p++));
    }
};

} // namespace serialize19
