import qbs

Project {
    name: "Co-Cpp19"

    references: [
        "third_party/third_party.qbs",
        "src/array19.lib",
        "src/coro19.lib",
        "src/enum19.lib",
        "src/meta19.lib",
        "src/lookup19.lib",
        "src/optional19.lib",
        "src/partial19.lib",
        "src/serialize19.lib",
        "src/signal19.lib",
        "src/string19.lib",
        "src/strong19.lib",
        "src/tuple19.lib",
        "src/variant19.lib",
    ]

    AutotestRunner {}

    // note: if do not use this .qbs project you need a similar setup
    Product {
        name: "cpp19"

        Export {
            Depends { name: "cpp" }
            cpp.cxxLanguageVersion: "c++2a"
            cpp.treatWarningsAsErrors: true

            Properties {
                condition: qbs.toolchain.contains('msvc')
                // for testing use:
                // * qbs build cpp.cxxFlags:/fsanitize=address
                cpp.cxxFlags: base.concat(
                    "/permissive-", "/Zc:__cplusplus", "/Zc:externConstexpr", "/Zc:inline", "/Zc:preprocessor", "/Zc:throwingNew", // best C++ compatibility
                    "/diagnostics:caret", // better errors
                    "/wd4068", // ignore unknown pragmas
                    "/D_ENABLE_EXTENDED_ALIGNED_STORAGE" // use real alignments
                )
            }
            Properties {
                condition: qbs.toolchain.contains('clang') || qbs.toolchain.contains('gcc')
                cpp.cxxFlags: base.concat(
                    "--pedantic", // best C++ compatibility
                    "-ftemplate-backtrace-limit=0", // do not cut template backtraces
                    "-Wno-gnu-zero-variadic-macro-arguments" // accept this extensions for opaque strong types
                )
            }
            Properties {
                condition: qbs.toolchain.contains('clang')
                cpp.cxxStandardLibrary: "libc++"
                cpp.staticLibraries: ["c++", "c++abi"]
            }
        }
    }

    Product {
        name: "_Extra Files_"
        files: [
            ".clang-format",
            ".clang-tidy",
            ".editorconfig",
            ".github/workflows/cmake_tests.yml",
            ".github/workflows/qbs_tests.yml",
            ".github/workflows/static_analyse.yml",
            ".gitignore",
            "CMakeLists.txt",
            "CMakePresets.json",
            "LICENSE",
            "Readme.md",
            "helpers/array19.natvis",
            "helpers/gdbhelpers.py",
            "helpers/optional19.natvis",
            "helpers/partial19_natvis.sh",
            "helpers/tuple19_natvis.sh",
            "helpers/variant19_natvis.sh",
        ]
    }
}
