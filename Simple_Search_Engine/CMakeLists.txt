cmake_minimum_required(VERSION 3.15)

project(
    labwork11
    VERSION 1.0
    DESCRIPTION "C++ Year 2024 labwork11"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(bin)
add_subdirectory(bin2)
add_subdirectory(Index)
add_subdirectory(Search)
add_subdirectory(Parser)

enable_testing()
add_subdirectory(tests)