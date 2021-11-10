from conans import ConanFile, CMake
from conan.tools.cmake import CMakeToolchain
import os

class Optional19Conan(ConanFile):
    name = "optional19"
    version = "1.0"
    
    # Optional metadata
    license = "MIT License"
    author = "Hicknhack Software"
    url = "https://github.com/basicpp17/co-cpp19"
    description = "C++17/20 Library with the fastest runtime and compile times"
    topics = ("algorithm", "container", "common", "utility")
    generators = "cmake_find_package", "cmake"
    # Binary configuration
    settings = "os", "compiler", "build_type", "arch"
    options = {"shared": [True, False], "fPIC": [True, False]}
    default_options = {"shared": False, "fPIC": True}

    def export_sources(self):
        self.copy("CMakeLists.txt")
        self.copy("*", src="optional19", dst="optional19")

    def config_options(self):
        if self.settings.os == "Windows":
            del self.options.fPIC

    def generate(self):
        tc = CMakeToolchain(self)
        tc.generate()

    def package(self):
        self.copy("*.h", src="optional19", dst="include")

