from conans import ConanFile

class Signal19Conan(ConanFile):
    name = "signal19"
    version = "1.0"
    
    # Optional metadata
    license = "MIT License"
    author = "Hicknhack Software"
    url = "https://github.com/basicpp17/co-cpp19"
    description = "C++17/20 Library with the fastest runtime and compile times"
    topics = ("algorithm", "container", "common", "utility")

    def export_sources(self):
        self.copy("CMakeLists.txt")
        self.copy("*", src="signal19", dst="signal19")

    def package(self):
        self.copy("*.h", dst="include")

    def requirements(self):
        self.requires("array19/1.0")