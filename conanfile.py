from conan import ConanFile
import os
from conan.tools.cmake import CMakeToolchain, CMakeDeps
from conan.tools.files import replace_in_file

class ThreeDRendererRecipe(ConanFile):
    settings = "os", "compiler", "build_type", "arch"

    def requirements(self):
        self.requires("sdl/2.28.3", options={
            "shared": False,
            "iconv": False,
            "directx": False,
            "opengl": False,
            "opengles": False,
            "vulkan": False})

    def generate(self):
        tc = CMakeToolchain(self)
        tc.generate()

        td = CMakeDeps(self)
        td.generate()

        # Fix mistake for LLVM Clang
        replace_in_file(self, os.path.join(self.build_folder, "SDL2-debug-x86_64-data.cmake"), "SDL2d", "SDL2-staticd")