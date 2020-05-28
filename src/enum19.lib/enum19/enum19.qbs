
Product {
    Depends { name: "cpp" }
    Depends { name: "string19" }

    files: [
        "ADL.h",
        "Enum.h",
        "Enum.ostream.h",
        "visitEnumMemberNames.h",
    ]

    Export {
        Depends { name: "cpp" }
        cpp.includePaths: [".."]
        Depends { name: "string19" }
    }
}
