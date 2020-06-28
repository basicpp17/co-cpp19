#pragma once
#include "ArchiveMode.h"
#include "SizeAppender.h"
#include "serialize.h"

namespace serialize19 {

struct SizeArchive {
    static constexpr auto mode = ArchiveMode::Size;

    constexpr auto size() const -> size_t { return m_appender.count(); }

    template<class T> void withPrimitive(const T& value) { m_appender = m_appender.appendValue(value); }

private:
    SizeAppender m_appender;
};

// Writing wont modify the value, but serialize methods can only have one signature
template<class T> void serialize(SizeArchive& wa, const T& v) { serialize(wa, const_cast<T&>(v)); }

} // namespace serialize19
