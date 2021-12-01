from conans import ConanFile

class Tuple19Conan(ConanFile):
    name = "tuple19"
    version = "1.0"
    
    # Optional metadata
    license = "MIT License"
    author = "Hicknhack Software"
    url = "https://github.com/basicpp17/co-cpp19"
    description = "C++17/20 Library with the fastest runtime and compile times"
    topics = ("algorithm", "container", "common", "utility")

    def export_sources(self):
        self.copy("CMakeLists.txt")
        self.copy("*", src="tuple19", dst="tuple19")

    def package(self):
        self.copy("*.h", dst="include")

    def requirements(self):
        self.requires("meta19/1.0")
