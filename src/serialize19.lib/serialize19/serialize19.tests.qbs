
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
        "serialize.std_bitset.test.cpp",
        "serialize.std_optional.test.cpp",
        "serialize.std_tuple.test.cpp",
        "serialize.std_variant.test.cpp",
        "serialize.std_vector.test.cpp",
    ]
}
