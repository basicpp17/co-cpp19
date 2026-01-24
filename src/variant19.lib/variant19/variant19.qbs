import qbs.FileInfo

Product {
    files: [
        "None.fmt.h",
        "None.h",
        "None.ostream.h",
        "Variant.equals.h",
        "Variant.fmt.h",
        "Variant.h",
        "Variant.ostream.h",
        "VariantWhich.fmt.h",
    ]

    Export {
        cpp.includePaths: FileInfo.joinPaths(exportingProduct.sourceDirectory, "..")

        Depends { name: "cpp" }
        Depends { name: "meta19" }
    }
    Depends { name: "meta19" }
}
