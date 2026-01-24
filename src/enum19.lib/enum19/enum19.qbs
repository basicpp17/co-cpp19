import qbs.FileInfo

Product {
    files: [
        "ADL.h",
        "Enum.extras.ostream.h",
        "Enum.fmt.h",
        "Enum.h",
        "Enum.max.h",
        "Enum.names.h",
        "Enum.ostream.h",
        "visitEnumMemberNames.h",
    ]

    Export {
        cpp.includePaths: FileInfo.joinPaths(exportingProduct.sourceDirectory, "..")

        Depends { name: "cpp" }
        Depends { name: "string19" }
    }
    Depends { name: "cpp" }
    Depends { name: "string19" }
    Depends { name: "fmt"; required: false }
}
