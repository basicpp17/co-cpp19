import qbs.FileInfo

Product {
    files: [
        "Optional.equals.h",
        "Optional.h",
        "Optional.ostream.h",
        "PackedOptional.equals.h",
        "PackedOptional.h",
        "PackedOptional.ostream.h",
        "PackedOptionalNan.h",
    ]

    Export {
        cpp.includePaths: FileInfo.joinPaths(exportingProduct.sourceDirectory, "..")

        Depends { name: "cpp" }
        Depends { name: "meta19" }
    }
    Depends { name: "cpp" }
    Depends { name: "meta19" }
}
