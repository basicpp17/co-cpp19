
Application {
    name: "strong19.tests"
    condition: googletest.present

    consoleApplication: true
    type: ["application", "autotest"]

    Depends { name: "strong19" }
    Depends { name: "googletest" }

    files: [
    ]
}
