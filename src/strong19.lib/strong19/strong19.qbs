
Product {
    Depends { name: "cpp" }
    Depends { name: "string19" }

    files: [
        "ADL.h",
        "Macro.h",
        "Strong.extras.ostream.h",
        "Strong.fmt.h",
        "Strong.h",
        "Strong.hash.h",
        "Strong.less.h",
        "Strong.ostream.h",
        "Strong.traits.h",
        "WeakOf.h",
    ]

    Export {
        Depends { name: "cpp" }
        cpp.includePaths: [".."]
        Depends { name: "string19" }

        Properties {
            condition: qbs.toolchain.contains('clang')
            cpp.cxxFlags: base.concat(
                "-Wno-gnu-zero-variadic-macro-arguments" // accept this extensions for opaque strong types
            )
        }
    }
}
