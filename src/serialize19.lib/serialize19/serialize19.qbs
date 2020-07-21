
Product {
    Depends { name: "cpp" }
    Depends { name: "meta19" }

    files: [
        "dynamicWrite.h",
        "serialize.h",
    ]

    Group {
        name: "Archive"
        files: [
            "Archive.h",
            "ArchiveMode.h",
            "ReadArchive.h",
            "SizeArchive.h",
            "WriteToArchive.h",
        ]
    }
    Group {
        name: "Impl"
        files: [
            "BufferSlice.h",
            "Endianess.h",
            "SizeAppender.h",
            "SliceReader.h",
            "UniqueBuffer.h",
            "WriteAppender.h",
        ]
    }

    Group {
        name: "serialize co-cpp19"
        files: [
            "serialize.Array.h",
            "serialize.BufferSlice.h",
            "serialize.DynamicArrayOf.h",
            "serialize.None.h",
            "serialize.Partial.h",
            "serialize.Strong.h",
            "serialize.Tuple.h",
            "serialize.Variant.h",
        ]
    }
    Group {
        name: "serialize std"
        files: [
            "serialize.std_array.h",
            "serialize.std_bitset.h",
            "serialize.std_map.h",
            "serialize.std_optional.h",
            "serialize.std_string.h",
            "serialize.std_tuple.h",
            "serialize.std_variant.h",
            "serialize.std_vector.h",
        ]
    }

    Export {
        Depends { name: "cpp" }
        cpp.includePaths: [".."]
        Depends { name: "meta19" }
    }
}
