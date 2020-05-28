
Product {
    Depends { name: "cpp" }
    Depends { name: "tuple19" }

    files: [
        "ADL.h",
        "DynamicString.h",
    ]
    Group {
        name: "Rope"
        files: [
            "Rope.count.h",
            "Rope.decimal.h",
            "Rope.h",
            "Rope.store.h",
        ]
    }
    Group {
        name: "StringStore"
        files: [
            "StringStore.equals.h",
            "StringStore.h",
            "StringStore.literal.h",
            "StringStore.range.h",
        ]
    }
    Group {
        name: "StringView"
        files: [
            "StringView.equals.h",
            "StringView.h",
            "StringView.literal.h",
            "StringView.ostream.h",
            "StringView.range.h",
            "StringView.std.h",
            "StringView.store.h",
        ]
    }

    Export {
        Depends { name: "cpp" }
        cpp.includePaths: [".."]
        Depends { name: "tuple19" }
    }
}
