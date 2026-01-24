
Application {
    name: "flags19.tests"
    consoleApplication: true
    type: base.concat(["autotest"])
    files: [
        "FlagsOf.test.cpp",
    ]

    Depends { name: "flags19" }
    Depends { name: "googletest" }
}
