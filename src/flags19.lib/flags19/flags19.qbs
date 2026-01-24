import qbs.FileInfo

Product {
    files: [
        "FlagsOf.fmt.h",
        "FlagsOf.h",
        "FlagsOf.ostream.h",
        "FlagsOf.trait.h",
    ]

    Export {
        cpp.includePaths: FileInfo.joinPaths(exportingProduct.sourceDirectory, "..")

        Depends { name: "cpp" }
        Depends { name: "enum19" }
    }
    Depends { name: "cpp" }
    Depends { name: "enum19" }
    Depends { name: "fmt"; required: false }
}
