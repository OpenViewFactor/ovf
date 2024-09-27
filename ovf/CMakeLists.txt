message("Adding Subdirectories to ovf CMakeLists file")


add_subdirectory(geometry)
add_subdirectory(cli)

# message("Checking whether Google Test is enabled in ovf directory CMakeLists")

# if(OVF_BUILD_GTEST)
  # message("Google Test is enabled in ovf directory CMakeLists")
  add_subdirectory(tests)
# endif()