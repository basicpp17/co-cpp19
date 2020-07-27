
Product {
    Depends { name: "cpp" }
    Depends { name: "string19" }
    Depends { name: "fmt"; required: false }

    files: [
        "ADL.h",
        "Enum.extras.ostream.h",
        "Enum.fmt.h",
        "Enum.h",
        "Enum.names.h",
        "Enum.ostream.h",
        "visitEnumMemberNames.h",
    ]

    Export {
        Depends { name: "cpp" }
        cpp.includePaths: [".."]
        Depends { name: "string19" }
    }
}
