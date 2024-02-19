if(OVF_BUILD_GTEST)
  # Set the input and output directories for gtest
  set(ovf_test_working_path ${CMAKE_CURRENT_SOURCE_DIR})
  set(ovf_test_input_path ${CMAKE_CURRENT_SOURCE_DIR}/test_inputs)
  set(ovf_test_output_path ${CMAKE_CURRENT_SOURCE_DIR}/test_outputs)

  enable_testing()

  configure_file("${CMAKE_CURRENT_SOURCE_DIR}/TestGlobals.h.in"
                 "{PROJECT_BINARY_DIR}/include/TestGlobals.h")

  file(MAKE_DIRECTORY test_outputs) # create the test outputs directory

endif()

if(OVF_BUILD_GTEST)
  # Include the testing subdirectories
  add_subdirectory(gtest)
endif()