
Product {
    Depends { name: "cpp" }
    Depends { name: "meta19" }

    files: [
        "ADL.h",
        "Strong.compare.h",
        "Strong.h",
        "Strong.hash.h",
        "Strong.ostream.h",
    ]

    Export {
        Depends { name: "cpp" }
        cpp.includePaths: [".."]
        Depends { name: "meta19" }
    }
}
