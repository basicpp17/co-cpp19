
Product {
    Depends { name: "meta19" }

    Export {
        Depends { name: "cpp" }
        cpp.includePaths: [exportingProduct.sourceDirectory + "/.."]
        Depends { name: "meta19" }
    }

    files: [
        "Bitset.h",
        "Partial.equals.h",
        "Partial.h",
        "Partial.ostream.h",
        "align.h",
    ]
}
