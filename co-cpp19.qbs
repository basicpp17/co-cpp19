import qbs

Project {
    name: "Co-Cpp19"

    property string version: "2026.01"
    property string configProductName: "Co-Cpp19-Config"
    property bool enableQbsImports: (sourceDirectory === path)
    property bool enableTests: (sourceDirectory === path)
    property bool enableThirdParty: (sourceDirectory === path)

    minimumQbsVersion: "3.0"
    qbsSearchPaths: enableQbsImports ? ["qbs"] : []
    references: ["src/src.qbs"]

    // note: if do not use this .qbs project you need a similar setup
    Product {
        name: "Co-Cpp19-Config"
        condition: parent.configProductName === "Co-Cpp19-Config"

        Export {
            cpp.cxxLanguageVersion: "c++23"
            cpp.treatWarningsAsErrors: true
            // to use libc++ set
            // modules.cpp.cxxStandardLibrary:libc++
            // modules.cpp.staticLibraries:c++,c++abi

            Properties {
                condition: qbs.toolchain.contains('msvc')
                // for testing use:
                // * qbs build cpp.cxxFlags:/fsanitize=address
                cpp.cxxFlags: base.concat(
                    "/permissive-", "/Zc:__cplusplus", "/Zc:externConstexpr", "/Zc:inline", "/Zc:preprocessor", "/Zc:throwingNew", // best C++ compatibility
                    "/diagnostics:caret", // better errors
                    "/experimental:external", "/external:anglebrackets", "/external:W0", // remove diagnostics from external headers
                    "/wd4068", // ignore unknown pragmas
                    "/D_ENABLE_EXTENDED_ALIGNED_STORAGE" // use real alignments
                )
            }
            Properties {
                condition: qbs.toolchain.contains('gcc') // also set for clang
                cpp.cxxFlags: base.concat(
                    "--pedantic", // best C++ compatibility
                    "-ftemplate-backtrace-limit=0", // do not cut template backtraces
                    "-Wno-gnu-zero-variadic-macro-arguments" // accept this extensions for opaque strong types
                )
            }
            Depends { name: "cpp" }
        }
    }
    SubProject {
        condition: parent.enableTests
        filePath: "src/tests.qbs"
    }
    SubProject {
        condition: parent.enableThirdParty
        filePath: "third_party/third_party.qbs"
    }
    Product {
        name: "[CoCpp19 Extra Files]"
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
            "script/build_cmake.sh",
            "script/docker_run_cmake_clang.bat",
            "script/docker_run_cmake_gcc.bat",
            "script/docker_run_qbs_clang_autotest-runner.bat",
            "script/test_cmake.bat",
            "script/test_cmake.sh",
        ]
    }
}
