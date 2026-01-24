
Application {
    name: "partial19.tests"
    consoleApplication: true
    type: base.concat(["autotest"])
    files: [
        "Bitset.test.cpp",
        "Partial.test.cpp",
    ]

    Depends { name: "partial19" }
    Depends { name: "googletest" }
}
