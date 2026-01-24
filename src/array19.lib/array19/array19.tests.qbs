
Application {
    name: "array19.tests"
    consoleApplication: true
    type: base.concat(["autotest"])
    files: [
        "*.test.cpp",
    ]

    Depends { name: "array19" }
    Depends { name: "googletest" }
}
