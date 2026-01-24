
Application {
    name: "coro19.tests"
    consoleApplication: true
    type: base.concat(["autotest"])
    files: [
        "CoEnumerator.rang.test.cpp",
        "CoEnumerator.test.cpp",
    ]

    Depends { name: "coro19" }
    Depends { name: "googletest" }
}
