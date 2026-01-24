import qbs.FileInfo

Product {
    files: [
        "Unreachable.h",
        "nullptr_to.h",
    ]

    Group {
        name: "Index"
        files: [
            "Index.h",
            "index_value.h",
        ]
    }
    Group {
        name: "IndexPack"
        files: [
            "IndexPack.Join.h",
            "IndexPack.h",
            "IndexPackTemplate.Change.h",
            "IndexPackTemplate.h",
            "index_pack.back.h",
            "index_pack.back.test.cpp",
            "index_pack.front.h",
            "index_pack.front.test.cpp",
            "index_pack.h",
            "index_pack.tail.h",
            "index_pack.to_array.h",
            "index_pack.to_array.test.cpp",
        ]
    }
    Group {
        name: "Type"
        files: [
            "RemoveReference.h",
            "Type.Unwrap.h",
            "Type.h",
            "TypeAt.h",
        ]
    }
    Group {
        name: "TypePack"
        files: [
            "TypePack.Front.h",
            "TypePack.count.h",
            "TypePack.h",
            "TypePackTemplate.Change.h",
            "TypePackTemplate.Join.h",
            "TypePackTemplate.contains.h",
            "TypePackTemplate.h",
        ]
    }
    Export {
        cpp.includePaths: FileInfo.joinPaths(exportingProduct.sourceDirectory, "..")

        Depends { name: "cpp" }
        Depends { name: "array19" }
    }
    Depends { name: "array19" }
}
