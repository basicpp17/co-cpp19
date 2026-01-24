
Application {
    name: "meta19.tests"
    consoleApplication: true
    files: [
        "*.test.cpp",
    ]

    Depends { name: "meta19" }
    Depends { name: "cpp" } // note: all tests are compile time only!
}
