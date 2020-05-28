
Application {
    name: "array19.tests"

    Depends { name: "array19" }
    // all tests are compile time only!

    Depends { name: "cpp" }
    // cpp.combineCxxSources: true

    files: [
        "*.test.cpp",
    ]
}
