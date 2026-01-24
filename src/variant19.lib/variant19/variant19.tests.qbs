
Application {
    name: "variant19.tests"
    consoleApplication: true
    type: base.concat(["autotest"])
    files: [
        "None.test.cpp",
        "Variant.test.cpp",
    ]

    Depends { name: "variant19" }
    Depends { name: "googletest" }
}
