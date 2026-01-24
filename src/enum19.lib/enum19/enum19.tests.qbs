
Application {
    name: "enum19.tests"
    consoleApplication: true
    type: base.concat(["autotest"])
    files: [
        "Enum.ostream.test.cpp",
        "Enum.test.cpp",
        "Enum.max.test.cpp",
    ]

    Depends { name: "enum19" }
    Depends { name: "googletest" }
}
