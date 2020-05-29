
Product {
    Depends { name: "cpp19" }

    files: [
        "Array.equals.h",
        "Array.h",
        "Array.ostream.h",
        "Array.slice.h",
        "DynamicArrayOf.equals.h",
        "DynamicArrayOf.h",
        "DynamicArrayOf.ostream.h",
        "SliceOf.carray.h",
        "SliceOf.equals.h",
        "SliceOf.h",
        "SliceOf.max.h",
        "SliceOf.ostream.h",
        "WithIndex.h",
        "Zip.h",
        "adlRange.h",
    ]

    Export {
        Depends { name: "cpp" }
        cpp.includePaths: [".."]
        Depends { name: "cpp19" }
    }
}
