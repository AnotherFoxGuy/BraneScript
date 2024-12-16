import os
from conan import ConanFile
from conan.tools.cmake import CMakeToolchain, CMakeDeps
from conan.tools.files import copy

class BraneScript(ConanFile):
    name = "BraneScript"
    settings = "os", "compiler", "build_type", "arch"

    def layout(self):
        self.folders.generators = os.path.join(self.folders.build, "generators")

    def requirements(self):
        # self.requires("llvm-core/13.0.0")
        self.requires("tree-sitter/0.24.4")
        self.requires("gtest/1.15.0")

    def generate(self):
        tc = CMakeToolchain(self)
        tc.generate()
        deps = CMakeDeps(self)
        deps.generate()
        if self.settings.os == "Windows" and self.settings.build_type == "Release":
            deps.configuration = "RelWithDebInfo"
            deps.generate()

        for dep in self.dependencies.values():
            for f in dep.cpp_info.bindirs:
                self.cp_data(f)
            for f in dep.cpp_info.libdirs:
                self.cp_data(f)

    def cp_data(self, src):
        bindir = os.path.join(self.build_folder, "bin")
        copy(self, "*.dll", src, bindir, False)
        copy(self, "*.so*", src, bindir, False)