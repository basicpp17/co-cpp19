
Application {
    name: "optional19.tests"
    consoleApplication: true
    type: base.concat(["autotest"])
    files: [
        "Optional.test.cpp",
        "PackedOptional.test.cpp",
        "PackedOptionalNan.test.cpp",
    ]

    Depends { name: "optional19" }
    Depends { name: "googletest" }
}
