#pragma once
#include "array19/Span.h"

#include <stddef.h> // size_t

namespace serialize19 {

using array19::Span;

/// accumulates the size of everything that is appended to this
struct SizeAppender {
    constexpr SizeAppender() = default;
    constexpr explicit SizeAppender(size_t count) : m_count(count) {}

    constexpr auto count() const -> size_t { return m_count; }

    template<class T> constexpr auto appendValue(T) const -> SizeAppender { return SizeAppender{m_count + sizeof(T)}; }
    template<class T> constexpr auto appendSpan(Span<const T> span) const -> SizeAppender {
        return SizeAppender{m_count + sizeof(T) * span.count()};
    }

private:
    size_t m_count{};
};

} // namespace serialize19
