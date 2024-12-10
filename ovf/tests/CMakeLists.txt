message("Set directories for Google Test shenanigans")

# Set input and output directories for gtest
set(TESTS_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(TESTS_INPUT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/assets)
set(TESTS_OUTPUT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/test_outputs)

message("Enable testing")
# Adds testing
enable_testing()

configure_file("${CMAKE_CURRENT_SOURCE_DIR}/TestGlobals.h.in" "${PROJECT_BINARY_DIR}/include/TestGlobals.h")

message("Create a test_ouputs folder if it doesn't exist")
# Make output directory
file(MAKE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/test_outputs)

message("Add gtest subdirectory")
# Include testing subdirectories
add_subdirectory(gtest)