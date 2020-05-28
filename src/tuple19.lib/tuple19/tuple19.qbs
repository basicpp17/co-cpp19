
Product {
    Depends { name: "cpp" }
    Depends { name: "meta19" }

    files: [
        "Tuple.bind.h",
        "Tuple.equals.h",
        "Tuple.h",
        "Tuple.trait.h",
    ]

    Export {
        Depends { name: "cpp" }
        cpp.includePaths: [".."]
        Depends { name: "meta19" }
    }
}
