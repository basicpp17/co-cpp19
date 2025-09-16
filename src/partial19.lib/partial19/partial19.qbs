
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
        "Partial.fmt.h",
        "Partial.h",
        "Partial.ostream.h",
        "PartialWhich.fmt.h",
        "align.h",
    ]
}
