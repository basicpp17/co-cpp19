
Application {
    name: "variant19.tests"
    condition: googletest.present

    consoleApplication: true
    type: ["application", "autotest"]

    Depends { name: "variant19" }
    Depends { name: "googletest" }

    files: [
        "Variant.test.cpp",
    ]
}
