if(OVF_BUILD_GTEST OR OVF_BUILD_BENCHMARK)
    # Set input and output directories for gtest
    set(TESTS_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR})
    set(TESTS_INPUT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/assets)
    set(TESTS_OUTPUT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/test_outputs)

    # Adds testing
    enable_testing()

    # Make output directory
    file(MAKE_DIRECTORY test_outputs)

endif()

if(OVF_BUILD_GTEST)
    # Include testing subdirectories
    add_subdirectory(gtest)
endif()

if (OVF_BUILD_BENCHMARK)
    # Include benchmark subdirectories
    add_subdirectory(bench)
endif()