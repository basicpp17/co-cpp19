
Product {
    Depends { name: "meta19" }

    Export {
        Depends { name: "cpp" }
        cpp.includePaths: [exportingProduct.sourceDirectory + "/.."]
        Depends { name: "meta19" }
    }

    files: [
        "Tuple.bind.h",
        "Tuple.fmt.h",
        "Tuple.h",
        "Tuple.join.h",
        "Tuple.ostream.h",
        "Tuple.trait.h",
    ]
}
