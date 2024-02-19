# Set a name for the testing executable "ovfAllTests"
set(ovf_gtest_all_tests_exe_name ovfAllTests)

message("Configuring OVF Unit Tests")

# Add the testing source files
set(_gtest_tests
    test_Vector3.cpp
)

# Include the googletest headers
include_directories(${GTEST_INCLUDE_DIR} 
                    ${GMOCK_INCLUDE_DIR}
                    ${THIRD_PARTY_INCLUDE_DIR}
)

# Define the executable
add_executable(${ovf_gtest_all_tests_exe_name} ${_gtest_tests})

# Link the googletest and ovf libraries
target_link_libraries(${ovf_gtest_all_tests_exe_name} ${GTEST_LIBS} ${GMOCK_LIBS} LeanVTK ovfcore ovfmanager)

# Add compilation flags for OMP and CUDA
if(OVF_BUILD_OMP)
  target_compile_definitions(${ovf_gtest_all_tests_exe_name} PRIVATE OVF_GTEST_SUPPORT_OMP)
endif()
if(OVF_BUILD_CUDA)
  target_compile_definitions(${ovf_gtest_all_tests_exe_name} PRIVATE OVF_GTEST_SUPPORT_CUDA)
endif()

# Add the compiled tests to to cmake ctest
add_test(NAME ${ovf_gtest_all_tests_exe_name} COMMAND ${ovf_gtest_all_tests_exe_name} WORKING_DIRECTORY ${ovf_test_working_path})