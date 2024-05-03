cmake_minimum_required(VERSION 3.29)
project(3drenderer C)

find_package(SDL2 REQUIRED CONFIG)

add_executable(3drenderer)

target_sources(3drenderer 
    PRIVATE 
        src/main.c)

target_link_libraries(3drenderer PRIVATE SDL2::SDL2main SDL2::SDL2-static)