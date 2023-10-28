
Product {
    Depends { name: "cpp19" }
    Depends { name: "array19" }

    Export {
        Depends { name: "cpp" }
        cpp.includePaths: [".."]
        Depends { name: "cpp19" }

        Depends { name: "array19" }
    }

    files: [
        "OrderedMapOf.equals.h",
        "OrderedMapOf.h",
        "OrderedMapOf.ostream.h",
        "OrderedSliceOf.h",
    ]
}
