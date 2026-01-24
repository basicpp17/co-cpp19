import qbs.FileInfo

Product {
    files: [
        "Tuple.bind.h",
        "Tuple.fmt.h",
        "Tuple.h",
        "Tuple.join.h",
        "Tuple.ostream.h",
        "Tuple.trait.h",
    ]

    Export {
        cpp.includePaths: FileInfo.joinPaths(exportingProduct.sourceDirectory, "..")

        Depends { name: "cpp" }
        Depends { name: "meta19" }
    }
    Depends { name: "meta19" }
}
