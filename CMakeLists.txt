cmake_minimum_required(VERSION 3.29)
project(3drenderer C)

find_package(SDL2 REQUIRED CONFIG)

add_executable(3drenderer WIN32)

target_sources(3drenderer 
    PRIVATE 
        src/main.c
        src/display.h
        src/display.c
        src/vector.h
        src/vector.c
        src/triangle.h
        src/triangle.c
        src/array.h
        src/array.c
        src/mesh.h
        src/mesh.c)

target_compile_features(3drenderer PUBLIC c_std_11)
if (MSVC)
    target_compile_options(3drenderer PUBLIC /W4)
else()
    target_compile_options(3drenderer PUBLIC -Wall -Wextra -Wpedantic)
endif()

target_link_libraries(3drenderer PRIVATE SDL2::SDL2main SDL2::SDL2-static)