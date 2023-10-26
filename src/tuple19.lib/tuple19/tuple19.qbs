
Product {
    Depends { name: "meta19" }

    Export {
        Depends { name: "cpp" }
        cpp.includePaths: [".."]
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
