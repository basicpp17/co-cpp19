import qbs.FileInfo

Product {
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
        cpp.includePaths: FileInfo.joinPaths(exportingProduct.sourceDirectory, "..")

        Properties {
            condition: qbs.toolchain.contains('clang')
            cpp.cxxFlags: base.concat(
                "-Wno-gnu-zero-variadic-macro-arguments" // accept this extensions for opaque strong types
            )
        }
        Depends { name: "cpp" }
        Depends { name: "string19" }
    }
    Depends { name: "string19" }
    Depends { name: "fmt"; required: false }
    Depends { name: "array19"; required: false }
}
