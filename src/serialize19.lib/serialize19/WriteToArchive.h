#pragma once
#include "Archive.h"
#include "BufferSpan.h"
#include "WriteAppender.h"
#include "serialize.h"

namespace serialize19 {

/// Archive adapter for writing to a span of memory
/// see: WriteAppender
template<EndianBehaviour endian = EndianBehaviour::Keep> struct WriteToArchive {
    static constexpr auto mode = ArchiveMode::Write;

    WriteToArchive(Span<uint8_t> span) : m_appender(span.begin()) {}

    void withSpan(BufferSpan span) { m_appender = m_appender.appendSpan(span); }

    template<class T> void withPrimitive(const T& value) { m_appender = m_appender.appendValue(value); }

    void withPrimitive(const bool& value) {
        auto v = value ? uint8_t{1} : uint8_t{};
        m_appender = m_appender.appendValue(v);
    }

private:
    WriteAppender<endian> m_appender;
};

static_assert(Archive<WriteToArchive<>>);

// note: Writing wont modify the value, but serialize methods can only have one signature
template<class T, EndianBehaviour endian> requires(std::is_const_v<T>)
void serialize(WriteToArchive<endian>& wa, T& v) {
    serialize(wa, const_cast<std::remove_const_t<T>&>(v));
}

} // namespace serialize19
