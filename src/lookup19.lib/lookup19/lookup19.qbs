
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
        "OrderedMapArray.equals.h",
        "OrderedMapArray.h",
        "OrderedMapArray.ostream.h",
        "OrderedSpan.h",
    ]
}
