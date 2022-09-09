from conans import ConanFile, CMake


class YAAACDConan(ConanFile):
    name = "yaaacd"
    version = "1.0"
    settings = "os", "compiler", "arch", "build_type"
    options = {"shared": [True, False]}
    default_options = "shared=False"
    generators = "cmake"
    exports_sources = "src/*", "include/*", "CMakeLists.txt", "test/*"

    def _configure_cmake(self):
        cmake = CMake(self)
        cmake.definitions["CMAKE_BUILD_TYPE"] = str(self.settings.build_type).upper()
        cmake.configure()
        return cmake

    def build(self):
        cmake = self._configure_cmake()
        cmake.build()
        cmake.test()

    def package(self):
        cmake = self._configure_cmake()
        cmake.install()

    def package_info(self):
        self.cpp_info.libs = ["yaaacd"]

