
Application {
    name: "string19.tests"
    consoleApplication: true
    type: base.concat(["autotest"])
    files: [
        "Rope.decimal.test.cpp",
        "Rope.join.test.cpp",
        "Rope.test.cpp",
        "StringStore.test.cpp",
        "StringView.test.cpp",
    ]

    Depends { name: "string19" }
    Depends { name: "googletest" }
}
