
Application {
    name: "strong19.tests"
    consoleApplication: true
    type: base.concat(["autotest"])
    files: [
        "Strong.extras.ostream.test.cpp",
        "Strong.hash.test.cpp",
        "Strong.test.cpp",
    ]

    Depends { name: "strong19" }
    Depends { name: "googletest" }
}
