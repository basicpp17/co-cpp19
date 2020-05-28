
Product {
    Depends { name: "meta19" }

    files: [
        "Bitset.equals.h",
        "Bitset.h",
        "Partial.h",
        "align.h",
    ]

    Export {
        Depends { name: "cpp" }
        cpp.includePaths: [".."]
        Depends { name: "meta19" }
    }
}
