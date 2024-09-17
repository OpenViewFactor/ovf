set(ovf_gtest_executable_name ovfAllTests)

message("Configuring OpenViewFactor Unit Tests")

set(_gtest_tests
  test_Vector3.cpp
  main.cpp
)

include_directories(
  ${GTEST_INCLUDE_DIR}
  ${THIRD_PARTY_INCLUDE_DIR}
)

add_executable(${ovf_gtest_executable_name} ${_gtest_tests})
target_link_libraries(${ovf_gtest_executable_name} ${GTEST_LIBS} ovfgeometry)