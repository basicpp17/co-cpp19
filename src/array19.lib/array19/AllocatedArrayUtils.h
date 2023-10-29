#pragma once
#include "Span.h"

#include <new> // ::operator new, ::operator delete
#include <stddef.h> // size_t
#include <string.h> // memcpy
#include <type_traits>
#include <utility>

namespace array19 {

template<class T, class B> concept will_construct = requires(T&& arg) { B{(T &&) arg}; };

/// Utility functions that implement the meat of allocated arrays
///
/// note:
/// * This replaces the algorithmns form std <memory> header.
/// * It works with spans instead of iterators to simplify the interface
template<class T> struct AllocatedArrayUtils {
    static_assert(std::is_nothrow_move_constructible_v<T>, "Please ensure move constructior is marked noexcept!");
    static_assert(std::is_nothrow_move_assignable_v<T>, "Please ensure move assignment is marked noexcept!");
    static_assert(std::is_nothrow_destructible_v<T>, "Please ensure destructor is marked noexcept!");

    using ConstSpan = Span<const T>;
    using AmendSpan = Span<T>;
    using MoveSpan = Span<T&&>;

    /// returns uninitialized storage for \param count elements with proper alignment
    /// note:
    /// * use deallocate to free the storage!
    /// * exceptions from new will terminate - without memory no recovery is possible!
    [[nodiscard]] static auto allocate(size_t count) noexcept -> T* {
        if (0 == count) return nullptr;
        if constexpr (__STDCPP_DEFAULT_NEW_ALIGNMENT__ < alignof(T)) {
            return reinterpret_cast<T*>(::operator new[](count * sizeof(T), std::align_val_t{alignof(T)}));
        }
        else {
            return reinterpret_cast<T*>(::operator new[](count * sizeof(T)));
        }
    }

    /// frees the storage pointed to by the span
    /// note:
    /// * span.begin() has to be the pointer returned by allocate(size.count())
    /// * exceptions form delete will terminate - we assume memory is corrupted!
    static void deallocate(AmendSpan span) noexcept {
#if __cpp_sized_deallocation
        if constexpr (__STDCPP_DEFAULT_NEW_ALIGNMENT__ < alignof(T)) {
            ::operator delete[](span.begin(), span.count() * sizeof(T), std::align_val_t{alignof(T)});
        }
        else {
            ::operator delete[](span.begin(), span.count() * sizeof(T));
        }
#else
        if constexpr (__STDCPP_DEFAULT_NEW_ALIGNMENT__ < alignof(T)) {
            ::operator delete[](span.begin(), std::align_val_t{alignof(T)});
        }
        else {
            ::operator delete[](span.begin());
        }
#endif
    }

    /// default construct every element of \param span if necessary
    static void defaultConstruct(AmendSpan span) noexcept(std::is_nothrow_default_constructible_v<T>) {
        if constexpr (!std::is_trivially_default_constructible_v<T>) {
            for (auto& e : span) new (&e) T();
        }
    }

    /// calls destructor for every element of \param span if necessary
    /// note:
    /// * we assume destructors are always noexcept
    static void destruct(AmendSpan span) noexcept {
        if constexpr (!std::is_trivially_destructible_v<T>)
            for (auto& elem : span) elem.~T();
    }

    /// constructs a copy of \param fromSpan at \param toPointer
    /// note:
    /// * toPointer has to point to least fromSpan.count() elements
    /// * assumes toPointer and fromSpan do not overlap
    static void copyConstruct(T* toPointer, ConstSpan fromSpan) noexcept(std::is_nothrow_copy_constructible_v<T>) {
        if constexpr (std::is_trivially_copy_constructible_v<T>) {
            if (!fromSpan.isEmpty()) {
                memcpy(toPointer, fromSpan.begin(), fromSpan.count() * sizeof(T));
            }
        }
        else {
            for (auto& from : fromSpan) new (toPointer++) T(from);
        }
    }

    /// move constructs elements of \param fromSpan into \param to Pointer
    /// note:
    /// * toPointer has to point to least fromSpan.count() elements
    /// * assumes toPointer and fromSpan do not overlap
    /// * we assume move is always noexcept!
    static void moveConstruct(T* toPointer, MoveSpan fromSpan) noexcept {
        if constexpr (std::is_trivially_move_constructible_v<T>) {
            if (!fromSpan.isEmpty()) {
                memcpy(toPointer, fromSpan.begin(), fromSpan.count() * sizeof(T));
            }
        }
        else {
            for (auto& from : fromSpan) new (toPointer++) T(std::move(from));
        }
    }

    /// assigns elements of \param fromSpan into \param toPointer
    /// note:
    /// * toPointer has to point to least fromSpan.count() initialized elements
    /// * assumes toPointer and fromSpan do not overlap
    static void copyAssign(T* toPointer, ConstSpan fromSpan) noexcept(std::is_nothrow_copy_assignable_v<T>) {
        if constexpr (std::is_trivially_copy_assignable_v<T>) {
            if (!fromSpan.isEmpty()) {
                memcpy(toPointer, fromSpan.begin(), fromSpan.count() * sizeof(T));
            }
        }
        else {
            for (const auto& from : fromSpan) *toPointer++ = from;
        }
    }

    /// move assigns elements of \param fromSpan into \param to Pointer
    /// note:
    /// * toPointer has to point to least fromSpan.count() elements
    /// * assumes toPointer and fromSpan do not overlap
    /// * we assume move is always noexcept!
    static void moveAssign(T* toPointer, MoveSpan fromSpan) noexcept {
        if constexpr (std::is_trivially_move_assignable_v<T>) {
            if (!fromSpan.isEmpty()) {
                memcpy(toPointer, fromSpan.begin(), fromSpan.count() * sizeof(T));
            }
        }
        else {
            for (auto& from : fromSpan) *toPointer++ = std::move(from);
        }
    }

    /// move assigns elements of \param fromSpan into \param to Pointer
    /// note:
    /// * toPointer has to point to least fromSpan.count() elements
    /// * toPointer has to be before fromSpan.begin() if ranges overlap
    /// * we assume move is always noexcept!
    ///
    /// example:
    ///         [ e e e e e e e ]
    /// toPointer ^   ^ fromSpan.begin()
    static void moveAssignForward(T* toPointer, MoveSpan fromSpan) noexcept {
        if constexpr (std::is_trivially_move_constructible_v<T>) {
            if (!fromSpan.isEmpty()) {
                memmove(toPointer, fromSpan.begin(), fromSpan.count() * sizeof(T));
            }
        }
        else {
            for (auto& from : fromSpan) *toPointer++ = std::move(from);
        }
    }

    /// move assigns elements of \param fromSpan into \param to Pointer
    /// note:
    /// * toPointer has to point to least fromSpan.count() elements
    /// * toPointer has to be behind fromSpan.begin() if ranges overlap
    /// * we assume move is always noexcept!
    ///
    /// example:
    ///                 [ e e e e e e e ]
    /// fromSpan.begin() ^     ^ toPointer
    static void moveAssignReverse(T* toPointer, MoveSpan fromSpan) noexcept {
        if constexpr (std::is_trivially_move_assignable_v<T>) {
            if (!fromSpan.isEmpty()) {
                memmove(toPointer, fromSpan.begin(), fromSpan.count() * sizeof(T));
            }
        }
        else {
            auto rTo = toPointer + fromSpan.count();
            auto rFrom = fromSpan.end();
            auto rFromEnd = fromSpan.begin();
            while (rFrom != rFromEnd) *(--rTo) = std::move(*(--rFrom));
        }
    }
};

} // namespace array19
