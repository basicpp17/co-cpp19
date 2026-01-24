import qbs.FileInfo

Product {
    files: [
        "AllocatedArrayOf.equals.h",
        "AllocatedArrayOf.h",
        "AllocatedArrayOf.ostream.h",
        "AllocatedArrayUtils.h",
        "Array.h",
        "Array.ostream.h",
        "DynamicArrayOf.AsHex.ostream.h",
        "DynamicArrayOf.equals.h",
        "DynamicArrayOf.h",
        "DynamicArrayOf.ostream.h",
        "DynamicSortedSet.h",
        "MoveSliceOf.h",
        "MoveSliceOf.single.h",
        "SliceOf.carray.h",
        "SliceOf.equals.h",
        "SliceOf.h",
        "SliceOf.max.h",
        "SliceOf.ostream.h",
        "SliceOf.single.h",
        "SliceOf.store.h",
        "WithIndex.h",
        "Zip.h",
        "adlRange.h",
    ]

    Export {
        cpp.includePaths: FileInfo.joinPaths(exportingProduct.sourceDirectory, "..")

        Depends { name: "cpp" }
        Depends { name: project.configProductName }
    }
    Depends { name: project.configProductName }
}
