from conans import ConanFile, CMake, tools
import os

class LibblinkstickConan(ConanFile):
    name = "LibBlinkStickCpp"
    version = "0.1"
    license = "<Put the package license here>"
    url = "<Package recipe repository url here, for issues about the package>"
    description = "<Description of Libblinkstick here>"
    settings = "os", "compiler", "build_type", "arch"
    options = {"shared": [True, False]}
    default_options = "shared=False"
    generators = "cmake"
    build_requires="cmake/[*]@*/*"
    requires="hidapi/0.8.0-rc1@av3m/stable", \
             "Boost/[*]@*/*", \
             "Eigen3/[*]@*/*", \
             "Qt/4.8.7@intence/dep"

    #def source(self):
    #    self.run("git clone https://github.com/Av3m/LibBlinkStickCpp.git libblinkstickcpp")
    #    # This small hack might be useful to guarantee proper /MT /MD linkage in MSVC
    #    # if the packaged project doesn't have variables to set it properly

    def build(self):
        cmake = CMake(self)
        cmake.definitions["CMAKE_INSTALL_PREFIX"] = os.path.join(self.conanfile_directory,"install")
        cmake.configure(source_dir="%s" % self.source_folder, build_dir=".")
        cmake.build()
        cmake.install()

        # Explicit way:
        # self.run('cmake %s/hello %s' % (self.source_folder, cmake.command_line))
        # self.run("cmake --build . %s" % cmake.build_config)

    def package(self):
        self.copy("*.h", dst="include", src="libblinkstickcpp")
        self.copy("*.lib", dst="lib", keep_path=False)
        self.copy("*.dll", dst="bin", keep_path=False)
        self.copy("*.so", dst="lib", keep_path=False)
        self.copy("*.dylib", dst="lib", keep_path=False)
        self.copy("*.a", dst="lib", keep_path=False)

    def package_info(self):
        self.cpp_info.libs = ["hello"]
