
Product {
    Depends { name: "cpp" }
    Depends { name: "meta19" }

    files: [
        "Archive.h",
        "Endianess.h",
        "SizeAppender.h",
        "SliceReader.h",
        "UniqueBuffer.h",
        "WriteAppender.h",
        "dynamicWrite.h",
    ]

    Export {
        Depends { name: "cpp" }
        cpp.includePaths: [".."]
        Depends { name: "meta19" }
    }
}
