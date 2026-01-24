import qbs.FileInfo

Product {
    files: [
        "Bitset.h",
        "Partial.equals.h",
        "Partial.fmt.h",
        "Partial.h",
        "Partial.ostream.h",
        "PartialWhich.fmt.h",
        "align.h",
    ]

    Export {
        cpp.includePaths: FileInfo.joinPaths(exportingProduct.sourceDirectory, "..")

        Depends { name: "cpp" }
        Depends { name: "meta19" }
    }
    Depends { name: "meta19" }
}
