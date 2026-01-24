import qbs
import qbs.File
import qbs.FileInfo
import online

StaticLibrary {
    name: "googletest"

    cpp.cxxLanguageVersion: "c++17"
    cpp.includePaths: [
        FileInfo.joinPaths(source.sourceDirectory, "googlemock"),
        FileInfo.joinPaths(source.sourceDirectory, "googlemock/include"),
        FileInfo.joinPaths(source.sourceDirectory, "googletest"),
        FileInfo.joinPaths(source.sourceDirectory, "googletest/include"),
    ]
    cpp.defines: ["GTEST_LANG_CXX11"]

    condition: source.found
    files: [
        FileInfo.joinPaths(source.sourceDirectory, "googlemock/src/gmock-all.cc"),
        FileInfo.joinPaths(source.sourceDirectory, "googletest/src/gtest-all.cc"),
        FileInfo.joinPaths(source.sourceDirectory, "googlemock/include/**/*.h"),
        FileInfo.joinPaths(source.sourceDirectory, "googletest/include/**/*.h"),
    ]

    online.Source {
      id: source

      name: "googletest"
      uri: "gh:google/googletest@1.17.0"
    }
    Properties {
        condition: qbs.toolchain.contains('gcc') // also set for clang
        cpp.cxxFlags: {
            var flags = base.concat(
                "-Wno-unused-parameter",
                "-Wno-missing-field-initializers",
                "-Wno-deprecated-copy"
            );
            if (!qbs.toolchain.contains('clang')) {
                flags.push("-Wno-maybe-uninitialized")
            }
            return flags;
        }
    }
    Export {
        property bool useMain: true

        cpp.systemIncludePaths: [
            FileInfo.joinPaths(source.sourceDirectory, "googlemock/include"),
            FileInfo.joinPaths(source.sourceDirectory, "googletest/include")
        ]
        cpp.defines: ["GTEST_LANG_CXX11"]

        Group {
            name: "Main"
            condition: product.googletest.useMain

            files: FileInfo.joinPaths(source.sourceDirectory, "googlemock/src/gmock_main.cc")
        }
        Properties {
            condition: qbs.toolchain.contains('gcc') // also set for clang
            cpp.cxxFlags: base.concat(
                "-Wno-unused-parameter",
                "-Wno-deprecated-copy"
            )
            cpp.dynamicLibraries: [ "pthread" ]
        }
        Depends { name: "cpp" }
    }
    Depends { name: "cpp" }
}
