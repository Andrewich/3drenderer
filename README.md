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

Build
-----

```
conan install . --output-folder=build -s build_type=Debug --build=missing
cmake -S . -B build/ -G "Visual Studio 16 2019" -DCMAKE_TOOLCHAIN_FILE="build/conan_toolchain.cmake"
cmake --build build/ --config Debug
```

Links
-----

- https://pikuma.com/courses/learn-3d-computer-graphics-programming
- https://docs.conan.io/2.0/tutorial/consuming_packages/different_configurations.html
- https://docs.conan.io/2.0/tutorial/consuming_packages/use_tools_as_conan_packages.html
- https://blog.conan.io/2023/07/20/introduction-to-game-dev-with-sdl2.html