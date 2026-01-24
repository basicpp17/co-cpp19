import qbs.FileInfo

Product {
    files: [
        "OrderedSetOf.h",
        "OrderedSliceOf.h",
    ]

    Export {
        cpp.includePaths: FileInfo.joinPaths(exportingProduct.sourceDirectory, "..")

        Depends { name: "cpp" }
        Depends { name: "array19" }
    }
    Depends { name: "array19" }
}
