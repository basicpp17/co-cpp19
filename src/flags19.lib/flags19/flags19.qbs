
Product {
    Depends { name: "cpp" }
    Depends { name: "enum19" }
    Depends { name: "fmt"; required: false }

    Export {
        Depends { name: "cpp" }
        cpp.includePaths: [exportingProduct.sourceDirectory + "/.."]
        Depends { name: "enum19" }
    }

    files: [
        "FlagsOf.fmt.h",
        "FlagsOf.h",
        "FlagsOf.ostream.h",
        "FlagsOf.trait.h",
    ]
}
