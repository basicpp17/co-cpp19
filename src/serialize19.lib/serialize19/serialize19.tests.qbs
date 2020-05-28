
Application {
    name: "serialize19.tests"
    condition: googletest.present

    consoleApplication: true
    type: ["application", "autotest"]

    Depends { name: "serialize19" }
    Depends { name: "googletest" }

    files: [
        "Codec.test.cpp",
        "Endianness.test.cpp",
    ]
}
