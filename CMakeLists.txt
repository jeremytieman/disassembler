cmake_minimum_required (VERSION 2.6)
project (Disassembler LANGUAGES CXX)
set (CMAKE_CXX_STANDARD 17)

add_subdirectory(external)
add_subdirectory(src)

enable_testing()
add_subdirectory(test)
