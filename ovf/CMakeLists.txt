message("Adding Subdirectories to ovf CMakeLists file")

add_subdirectory(geometry)
add_subdirectory(cli)
add_subdirectory(tests)

# message("Checking whether Google Test is enabled in ovf directory CMakeLists")

# if(OVF_BUILD_GTEST)
  # message("Google Test is enabled in ovf directory CMakeLists")
  
# endif()