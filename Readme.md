# Co-C++19
## The Fastest Meta Programming Library

A versatile collection of compile time optimized tools for C++ programming.

[![Actions: Test Runner](https://github.com/basicpp17/co-cpp19/workflows/Test%20Runner/badge.svg)](https://github.com/basicpp17/co-cpp19/actions)

## Target Compilers

This library uses the latest C++ features available in Clang and Visual Studio 2019.
It compiles in "c++2a" on clang and "c++latest" on Visual Studio.

Currently testd Compilers:
* Clang 12
* Visual Studio 2019 v16.10.x

GCC may be supported once the following bugs have been fixed:
* Lambdas in constexpr context

## Contents

Co-Cpp19 contains a lot of sublibraries and is splitted into many small headers.
This should optimize for pay only for what you use policy of C++, also at compile time.

* cpp19 - compiler configuration (yours should be somewhat similar)
* array19 - static and dynamic arrays, slices
* meta19 - utilities, type, index and pack wrappers and operations
* coro19 - basic coroutine with generator support
* strong19 - named strong types with tags
* enum19 - inspectable enum definitions
* string19 - compiletime string, view and string rope
* tuple19 - faster to compile `std::tuple` replacement
* optional19 - continuable optional and packed optional
* variant19 - faster compile and runtime `std::variant` replacement
* partial19 - densly packed and heap managed `tuple<optional<>>`
* serialize19 - customizable binary serialisation and deserialisation

Work in progress libraries, with less tests:

* signal19 - fast signal and slot handling
* lookup19 - ordered dynamic array lookup container

## Naming Schema

C++ uses a unified namespace between types, functions and constants.
Therefore we use the following patterns.

* `UpperCamelCasing` for types
* `lowerCamelCasing` for functions
* `UPPER_CASING` for precompiler definitions
* `lower_casing` for constexpr constants, concepts and namespaces

## Words and Phrases

* `*Of` - reflects something of a type
* `*At` - uses something at an index
* `[v]From()` - converts from arguments to v
* `*Type*` - indicates that the method is just a signature for usage with `decltype()`
