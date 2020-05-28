
Application {
    name: "enum19.tests"
    condition: googletest.present

    consoleApplication: true
    type: ["application", "autotest"]

    Depends { name: "enum19" }
    Depends { name: "googletest" }

    files: [
        "Enum.test.cpp",
    ]
}
