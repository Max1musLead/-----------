cmake_minimum_required(VERSION 3.15)

project(
    labwork3
    VERSION 1.0
    LANGUAGES CXX
)


set(CMAKE_CXX_STANDARD 17)

add_subdirectory(bin)
add_subdirectory(bmp)
add_subdirectory(parser)
add_subdirectory(sandpile_model)