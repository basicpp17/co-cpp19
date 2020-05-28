
Product {
    Depends { name: "cpp" }
    Depends { name: "meta19" }

    files: [
        "Variant.equals.h",
        "Variant.h",
    ]

    Export {
        Depends { name: "cpp" }
        cpp.includePaths: [".."]
        Depends { name: "meta19" }
    }
}
