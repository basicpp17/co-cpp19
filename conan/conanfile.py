from conans import ConanFile, CMake
from conan.tools.cmake import CMakeToolchain
import os

class cocpp19Conan(ConanFile):
    name = "co-cpp19"
    version = "1.0"
    
    # Optional metadata
    license = "MIT License"
    author = "Hicknhack Software"
    url = "https://github.com/basicpp17/co-cpp19"
    description = "C++17/20 Library with the fastest runtime and compile times"
    topics = ("algorithm", "container", "common", "utility")

    # Binary configuration
    settings = "os", "compiler", "build_type", "arch"
    options = {"shared": [True, False], "fPIC": [True, False]}
    default_options = {"shared": False, "fPIC": True}

    def export_sources(self):
        self.copy("*", src=os.pardir)
        source_dir = os.path.join(os.getcwd(), os.pardir, "src")
        include_dirs = [os.path.join(source_dir, name) for name in os.listdir(source_dir)]
        include_dirs = [name for name in include_dirs if os.path.isdir(name) ]
        for include_dir in include_dirs:
            self.copy("*.h", src=include_dir, dst="include")

    def config_options(self):
        if self.settings.os == "Windows":
            del self.options.fPIC

    def generate(self):
        tc = CMakeToolchain(self)
        tc.generate()

    def build(self):
        cmake = CMake(self)
        cmake.definitions["co-cpp19-enable-tests"] = "ON"
        cmake.configure()
        cmake.build()
        cmake.test()

    def package(self):
        self.copy("*.h", src="include", dst="include")
        self.copy("*.lib", dst="lib", excludes="*gtest*")
