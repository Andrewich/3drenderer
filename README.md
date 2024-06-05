Learn 3D Computer Graphics Programming by Pikuma
================================================

Overview
--------

This project is the realization of 3D rendering from scratch step by step 
based on a course by [Pikuma](https://pikuma.com/courses/learn-3d-computer-graphics-programming).

Dependences
-----------

- **C compiler** - used **Visual Studio 2019**
- **CMake** - used *3.29* verison
- **SDL-2.28.3** - installed with **conan**

Preparation
-----------

Creating a conan profile for Clang on Microsoft Windows (used Ninja):

```
[settings]
os=Windows
arch=x86_64
build_type=Debug
compiler=clang
compiler.version=17

[conf]
tools.build:compiler_executables={'c': 'C:/Program Files/LLVM/bin/clang.exe', 'cpp': 'C:/Program Files/LLVM/bin/clang++.exe' }
tools.cmake.cmaketoolchain:generator=Ninja

[buildenv]
CC=clang-cl
CXX=clang-cl
RC=clang-cl
CONAN_CMAKE_GENERATOR=Ninja
CMAKE_MAKE_PROGRAM=Ninja
```

Build
-----

Visual Studio 2019
------------------
```
conan install . --output-folder=build -s build_type=Debug --build=missing
cmake -S . -B build/ -G "Visual Studio 16 2019" -DCMAKE_TOOLCHAIN_FILE="build/conan_toolchain.cmake"
cmake --build build/ --config Debug
```

Ninja
-----
```
conan install . --output-folder=build -s build_type=Debug --build=missing --profile=clang
cmake -S . -B build/ -G "Ninja" -DCMAKE_TOOLCHAIN_FILE="build/conan_toolchain.cmake" -DCMAKE_BUILD_TYPE=Debug
cmake --build build/ --config Debug
```

Issues
------

Build error with **LLVM Clang** compiler on **Windows**:
```
CMake Error at build/cmakedeps_macros.cmake:67 (message):
  Library 'SDL2d' not found in package.  If 'SDL2d' is a system library,
  declare it with 'cpp_info.system_libs' property
Call Stack (most recent call first):
  build/SDL2-Target-debug.cmake:24 (conan_package_library_targets)
  build/SDL2Targets.cmake:30 (include)
  build/SDL2Config.cmake:16 (include)
  CMakeLists.txt:4 (find_package)
```

Solution
```
sed -i 's/SDL2d)/SDL2-staticd)/g' ./build/SDL2-debug-x86_64-data.cmake
```


Links
-----

- https://pikuma.com/courses/learn-3d-computer-graphics-programming
- https://docs.conan.io/2.0/tutorial/consuming_packages/different_configurations.html
- https://docs.conan.io/2.0/tutorial/consuming_packages/use_tools_as_conan_packages.html
- https://blog.conan.io/2023/07/20/introduction-to-game-dev-with-sdl2.html