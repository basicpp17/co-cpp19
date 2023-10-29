
Product {
    Depends { name: "cpp" }
    Depends { name: "meta19" }

    Depends { name: "optional19"; required: false }
    Depends { name: "partial19"; required: false }
    Depends { name: "strong19"; required: false }
    Depends { name: "tuple19"; required: false }
    Depends { name: "variant19"; required: false }

    Export {
        Depends { name: "cpp" }
        cpp.includePaths: [".."]
        Depends { name: "meta19" }
    }

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
            "BufferSpan.h",
            "Endianess.h",
            "SizeAppender.h",
            "SpanReader.h",
            "UniqueBuffer.h",
            "WriteAppender.h",
        ]
    }
    Group {
        name: "serialize co-cpp19"
        files: [
            "serialize.AllocatedArray.h",
            "serialize.Array.h",
            "serialize.BufferSpan.h",
            "serialize.DynamicArray.h",
            "serialize.None.h",
            "serialize.Optional.h",
            "serialize.PackedOptional.h",
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
    files: [
        "dynamicWrite.h",
        "serialize.h",
    ]
}
