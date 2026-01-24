import qbs.FileInfo
import qbs.Utilities

StaticLibrary {
    files: [
        "CoEnumerator.h",
        "CoEnumerator.range.h",
        "coroutine.cpp",
        "coroutine.h",
    ]

    Export {
        cpp.includePaths: FileInfo.joinPaths(exportingProduct.sourceDirectory, "..")

        Depends { name: "cpp" }
        Depends { name: "array19" }
    }
    Depends { name: "cpp" }
    Depends { name: "array19" }
}
