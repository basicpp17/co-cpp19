
Application {
    name: "lookup19.tests"
    consoleApplication: true
    type: base.concat(["autotest"])
    files: [
        "OrderedSetOf.test.cpp",
    ]

    Depends { name: "lookup19" }
    Depends { name: "googletest" }
}
