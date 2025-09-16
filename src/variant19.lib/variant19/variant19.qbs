
Product {
    Depends { name: "meta19" }

    Export {
        Depends { name: "cpp" }
        cpp.includePaths: [exportingProduct.sourceDirectory + "/.."]
        Depends { name: "meta19" }
    }

    files: [
        "None.fmt.h",
        "None.h",
        "None.ostream.h",
        "Variant.equals.h",
        "Variant.fmt.h",
        "Variant.h",
        "Variant.ostream.h",
        "VariantWhich.fmt.h",
    ]
}
