
Product {
    Depends { name: "cpp19" }

    Export {
        Depends { name: "cpp" }
        cpp.includePaths: [".."]
        Depends { name: "cpp19" }
    }

    files: [
        "AllocatedArray.equals.h",
        "AllocatedArray.h",
        "AllocatedArray.ostream.h",
        "AllocatedArrayUtils.h",
        "Array.h",
        "Array.ostream.h",
        "DynamicArray.equals.h",
        "DynamicArray.h",
        "DynamicArray.ostream.h",
        "DynamicSortedSet.h",
        "Span.carray.h",
        "Span.equals.h",
        "Span.h",
        "Span.max.h",
        "Span.one.h",
        "Span.ostream.h",
        "Span.store.h",
        "WithIndex.h",
        "Zip.h",
        "adlRange.h",
    ]
}
