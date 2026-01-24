
Application {
    name: "signal19.tests"
    consoleApplication: true
    type: base.concat(["autotest"])
    files: [
        "SignalWith.test.cpp",
        "Subscriptions.test.cpp",
    ]

    Depends { name: "googletest" }
    Depends { name: "signal19" }
}
