from conan import ConanFile
from conan.tools.cmake import CMake, CMakeToolchain, cmake_layout
from conan.tools.files import copy, rmdir
from conan.tools.build import check_min_cppstd

import os

class Cocpp19Conan(ConanFile):
    name = "co-cpp19"
    version = "1.0"
    description = "C++20 Library with the fastest runtime and compile times"
    author = "Hicknhack Software"
    license = "MIT License"
    url = "https://github.com/basicpp17/co-cpp19"
    homepage = "https://github.com/basicpp17/co-cpp19"
    topics = ("algorithm", "container", "common", "utility")

    settings = "os", "arch", "compiler", "build_type"
    options = {
        "shared": [True, False],
        "fPIC": [True, False]
    }
    default_options = {
        "shared": False,
        "fPIC": True
    }
    test_requires = "gtest/1.11.0"
    generators = "CMakeDeps"

    def layout(self):
        self.folders.root = ".."
        cmake_layout(self)

    def export_sources(self):
        folder = os.path.join(self.recipe_folder, "..")
        copy(self, "CMakeLists.txt", folder, self.export_sources_folder)
        copy(self, "CoCpp19Config.cmake.in", folder, self.export_sources_folder)
        copy(self, "LICENSE", folder, self.export_sources_folder)
        copy(self, "src/*", folder, self.export_sources_folder, excludes="*.qbs")
        copy(self, "third_party/CMakeLists.txt", folder, self.export_sources_folder)
        copy(self, "third_party/googletest.cmake", folder, self.export_sources_folder)

    def config_options(self):
        if self.settings.os == "Windows":
            del self.options.fPIC

    def validate(self):
        check_min_cppstd(self, "20")

    def generate(self):
        tc = CMakeToolchain(self)
        tc.generate()

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def package(self):
        copy(self, "LICENSE", self.source_folder, self.package_folder)
        cmake = CMake(self)
        cmake.install()
        rmdir(self, os.path.join(self.package_folder, "lib", "pkgconfig"))
        rmdir(self, os.path.join(self.package_folder, "lib", "cmake"))

    def package_info(self):
        self.cpp_info.set_property("cmake_find_mode", "both")
        self.cpp_info.set_property("cmake_file_name", "CoCpp19")
        self.cpp_info.set_property("cmake_target_name", "CoCpp19")

        components = [
            {"name": "array19"},
            {"name": "coro19", "requires": ["array19"], "libs": ["coro19"]},
            {"name": "enum19", "requires": ["string19"]},
            {"name": "lookup19", "requires": ["array19"]},
            {"name": "meta19", "requires": ["array19"]},
            {"name": "optional19", "requires": ["meta19"]},
            {"name": "partial19", "requires": ["meta19"]},
            {"name": "serialize19", "requires": ["meta19"]},
            {"name": "signal19", "requires": ["array19"]},
            {"name": "string19", "requires": ["tuple19"]},
            {"name": "strong19", "requires": ["string19"]},
            {"name": "tuple19", "requires": ["meta19"]},
            {"name": "variant19", "requires": ["meta19"]}
        ]
        for comp in components:
            name = comp["name"]
            self.cpp_info.components[name].set_property("cmake_target_name", f"CoCpp19::{name}")
            self.cpp_info.components[name].set_property("pkg_config_name", name)
            if "requires" in comp:
                self.cpp_info.components[name].requires = comp["requires"]
            if "libs" in comp:
                self.cpp_info.components[name].libs = comp["libs"]

        # self.cpp_info.names["cmake_find_package"] = "CoCpp19"
        # self.cpp_info.names["cmake_find_package_multi"] = "CoCpp19"

    def test(self):
        cmake = CMake(self)
        cmake.test(output_on_failure=True)
