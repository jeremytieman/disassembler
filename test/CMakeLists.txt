add_executable(DisassemblerTest Main.cpp)
target_link_libraries(DisassemblerTest gtest_main)
set_target_properties(DisassemblerTest PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin/test/)

add_test(NAME DisTest COMMAND DisassemblerTest)
