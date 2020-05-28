
Product {
    Depends { name: "cpp19" }

    files: [
        "*.h",
    ]

    Export {
        Depends { name: "cpp" }
        cpp.includePaths: [".."]
        Depends { name: "cpp19" }
    }
}
