
Application {
    name: "tuple19.tests"
    consoleApplication: true
    type: base.concat(["autotest"])
    files: [
        "Tuple.bind.test.cpp",
        "Tuple.join.test.cpp",
        "Tuple.test.cpp",
    ]

    Depends { name: "tuple19" }
    Depends { name: "googletest" }
}
