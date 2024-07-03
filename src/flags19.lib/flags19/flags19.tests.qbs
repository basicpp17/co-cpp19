
Application {
    name: "flags19.tests"
    condition: googletest.present

    consoleApplication: true
    type: ["application", "autotest"]

    Depends { name: "flags19" }
    Depends { name: "googletest" }

    files: [
        "FlagsOf.test.cpp",
    ]
}
