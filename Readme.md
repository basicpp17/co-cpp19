# Co-C++19
## The Fastest Meta Programming Library

A versatile collection of compile time optimized tools for C++ programming.

## Target Compilers

This library uses the latest C++ features available in Clang and Visual Studio 2019.
It compiles in "c++2a" on clang and "c++latest" on Visual Studio.

Currently testd Compilers:
* Clang 10
* Visual Studio 2019 v16.6.0

GCC may be supported once the following bugs have been fixed:
* Lambdas in constexpr context

## Naming Schema

C++ uses a unified namespace between types, functions and constants.
Therefore we use the following patterns.

* `UpperCamelCasing` for types
* `lowerCamelCasing` for functions
* `UPPER_CASING` for precompiler definitions
* `lower_casing` for constexpr constants & namespaces

## Words and Phrases

* `*Of` - reflects something of a type
* `*At` - uses something at an index
* `[v]From()` - converts from arguments to v
* `*Type*` - indicates that the method is just a signature for usage with `decltype()`
