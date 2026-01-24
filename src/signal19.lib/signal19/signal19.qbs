import qbs.FileInfo

Product {
    files: [
        "SignalWith.h",
        "Subscription.h",
        "Subscriptions.h",
    ]

    Export {
        cpp.includePaths: FileInfo.joinPaths(exportingProduct.sourceDirectory, "..")

        Depends { name: "cpp" }
        Depends { name: "meta19" }
    }
    Depends { name: "cpp" }
    Depends { name: "meta19" }
}
