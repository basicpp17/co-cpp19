
Application {
    name: "tuple19.tests"
    condition: googletest.present

    consoleApplication: true
    type: ["application", "autotest"]

    Depends { name: "tuple19" }
    Depends { name: "googletest" }

    files: [
        "Tuple.test.cpp",
    ]
}
