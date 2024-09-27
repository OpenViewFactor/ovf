message("Set testing executable name")

set(ovf_gtest_executable_name ovfAllTests)

message("Configuring OpenViewFactor Unit Tests")

set(_test_sources
  test_Vector3.cpp
  main.cpp
)

include_directories(
  ${GTEST_INCLUDE_DIR}
  ${THIRD_PARTY_INCLUDE_DIR}
)

# -------------------- BUILD TEST EXECUTABLE -------------------- #
add_executable(ovfAllTests ${_test_sources})
target_link_libraries(ovfAllTests ${GTEST_LIBS} ovfgeometry)

add_test(NAME ovfAllTests COMMAND ovfAllTests WORKING_DIRECTORY ${TESTS_SRC_DIR})