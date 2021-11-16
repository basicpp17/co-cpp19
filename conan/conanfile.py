from conans import ConanFile, CMake
from conan.tools.cmake import CMakeToolchain
import os

class Cocpp19Conan(ConanFile):
    name = "co-cpp19"
    version = "1.0"
    
    # Optional metadata
    license = "MIT License"
    author = "Hicknhack Software"
    url = "https://github.com/basicpp17/co-cpp19"
    description = "C++17/20 Library with the fastest runtime and compile times"
    topics = ("algorithm", "container", "common", "utility")
    generator = "cmake", "cmake_find_package"
    # Binary configuration
    settings = "os", "compiler", "build_type", "arch", 
    options = {
        "shared": [True, False], 
        "fPIC": [True, False]
    }

    default_options = {
        "shared": False, 
        "fPIC": True
    }
    
    def export_sources(self):
        self.copy("*", src=os.pardir)
        source_dir = os.path.join(os.getcwd(), os.pardir, "src")
        include_dirs = [os.path.join(source_dir, name) for name in os.listdir(source_dir)]
        include_dirs = [name for name in include_dirs if os.path.isdir(name) ]
        for include_dir in include_dirs:
            self.copy("*.h", src=include_dir, dst="include")

    def config_options(self):
        print("config_options")
        if self.settings.os == "Windows":
            del self.options.fPIC

    def generate(self):
        tc = CMakeToolchain(self)
        tc.generate()

    def build(self):
        cmake = CMake(self)
        directory = os.path.join(os.getcwd(), "src", "coro19.lib")
        cmake.configure(source_folder=directory, build_folder= "coro19.lib")
        cmake.build()

    def package(self):
        self.copy("*.lib", dst="lib", keep_path=False)
        self.copy("*.dll", dst="bin", keep_path=False)
        self.copy("*.so", dst="lib", keep_path=False)
        self.copy("*.dylib", dst="lib", keep_path=False)
        self.copy("*.a", dst="lib", keep_path=False)

        include_dirs = [name for name in os.listdir("include")]
        for dir in include_dirs:
            include_dir = os.path.join("include", dir)
            self.copy("*.h", src=include_dir, dst=include_dir)
    
    def package_info(self):
        postfix = "d" if self.settings.build_type == "Debug" else ""
        self.cpp_info.libs = ["coro19" + postfix]

    def requirements(self):
        self.requires("gtest/1.11.0", private=True, override=False)