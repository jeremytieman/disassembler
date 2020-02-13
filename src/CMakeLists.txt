set(SOURCE_FILES Main.cpp Disassembler8080.cpp)
add_executable(Disassembler ${SOURCE_FILES})
target_include_directories(Disassembler PRIVATE ${CMAKE_SOURCE_DIR}/inc/ PRIVATE ${PROJECT_BINARY_DIR}/external/cxxopts-src/include/)
set_target_properties(Disassembler PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin/)
