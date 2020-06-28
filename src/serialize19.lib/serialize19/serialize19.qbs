
Product {
    Depends { name: "cpp" }
    Depends { name: "meta19" }

    files: [
        "Archive.h",
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
        "serialize.Array.h",
        "serialize.DynamicArrayOf.h",
        "serialize.h",
        "serialize.std_array.h",
        "serialize.std_bitset.h",
        "serialize.std_map.h",
        "serialize.std_optional.h",
        "serialize.std_string.h",
        "serialize.std_tuple.h",
        "serialize.std_variant.h",
        "serialize.std_vector.h",
    ]

    Export {
        Depends { name: "cpp" }
        cpp.includePaths: [".."]
        Depends { name: "meta19" }
    }
}
