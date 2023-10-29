#pragma once
#include "Endianess.h"
#include "array19/Span.h"

#include <stdint.h> // uint8_t

namespace serialize19 {

using array19::Span;

namespace details {

struct WriteAppenderCommon {
    constexpr WriteAppenderCommon() = default;
    constexpr explicit WriteAppenderCommon(uint8_t* data) : m_data(data) {}

protected:
    uint8_t* m_data{};
};
} // namespace details

/// appends primitive values to a given pointer
/// note: caller has to ensure that enough storage is available (see dynamicWrite)
template<EndianBehaviour> struct WriteAppender;

template<> struct WriteAppender<EndianBehaviour::Keep> final : details::WriteAppenderCommon {
    using details::WriteAppenderCommon::WriteAppenderCommon;

    template<class T> auto appendValue(T value) const -> WriteAppender {
        *(T*)m_data = value;
        return WriteAppender{m_data + sizeof(T)};
    }
    template<class T> auto appendSpan(Span<const T> span) const -> WriteAppender {
        auto p = (T*)m_data;
        for (auto v : span) *p++ = v;
        return WriteAppender{(uint8_t*)p};
    }
};

template<> struct WriteAppender<EndianBehaviour::Flip> final : details::WriteAppenderCommon {
    using details::WriteAppenderCommon::WriteAppenderCommon;

    template<class T> auto appendValue(T value) const -> WriteAppender {
        *(T*)m_data = endianFlipFor<T>(value);
        return WriteAppender{m_data + sizeof(T)};
    }
    template<class T> auto appendSpan(Span<const T> span) const -> WriteAppender {
        auto p = (T*)m_data;
        for (auto v : span) *p++ = endianFlipFor<T>(v);
        return WriteAppender{(uint8_t*)p};
    }
};

} // namespace serialize19
