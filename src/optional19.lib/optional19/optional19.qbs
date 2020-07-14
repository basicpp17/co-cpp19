
Product {
    Depends { name: "cpp" }
    Depends { name: "meta19" }

    files: [
        "NanOptional.h",
        "Optional.equals.h",
        "Optional.h",
        "Optional.ostream.h",
        "PackedOptional.equals.h",
        "PackedOptional.h",
        "PackedOptional.ostream.h",
    ]

    Export {
        Depends { name: "cpp" }
        cpp.includePaths: [".."]
        Depends { name: "meta19" }
    }
}
