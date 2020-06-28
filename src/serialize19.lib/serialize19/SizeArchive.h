#pragma once
#include "Archive.h"
#include "SizeAppender.h"
#include "serialize.h"

#include <stdint.h>

namespace serialize19 {

/// Simple Archive adapter to calculate the size required to store the data
/// see: SizeAppender
struct SizeArchive {
    static constexpr auto mode = ArchiveMode::Size;

    constexpr auto size() const -> size_t { return m_appender.count(); }

    template<class T> void withPrimitive(const T& value) { m_appender = m_appender.appendValue(value); }

    void withPrimitive(const bool& value) {
        auto v = value ? uint8_t{1} : uint8_t{};
        m_appender = m_appender.appendValue(v);
    }

private:
    SizeAppender m_appender;
};

static_assert(Archive<SizeArchive>);

// note: SizeArchive wont modify the value, but serialize methods can only have one signature
// note: Archive is a template argument to allow fallback to Fallback
template<Archive A, class T> requires(std::same_as<A, SizeArchive>) void serialize(A& a, const T& v) {
    serialize(a, const_cast<T&>(v));
}

} // namespace serialize19
