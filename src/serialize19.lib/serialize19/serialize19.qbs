
Product {
    Depends { name: "cpp" }
    Depends { name: "meta19" }

    files: [
        "ArchiveMode.h",
        "Endianess.h",
        "ReadArchive.h",
        "SizeAppender.h",
        "SizeArchive.h",
        "SliceReader.h",
        "UniqueBuffer.h",
        "WriteAppender.h",
        "WriteToArchive.h",
        "dynamicWrite.h",
        "serialize.h",
    ]

    Export {
        Depends { name: "cpp" }
        cpp.includePaths: [".."]
        Depends { name: "meta19" }
    }
}
