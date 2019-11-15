cmake_minimum_required(VERSION 3.5)

project(allocator)

add_executable(allocator allocator.cpp container.h block_allocator.h hard.h easy.h utility.h)

set_target_properties(allocator PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
        COMPILE_OPTIONS "-Wpedantic;-Wall;-Wextra"
        )
