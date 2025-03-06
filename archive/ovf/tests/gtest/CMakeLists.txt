message("Set testing executable name")

set(ovf_gtest_executable_name ovfAllTests)

message("Configuring OpenViewFactor Unit Tests")

include_directories(${GTEST_INCLUDE_DIR} ${THIRD_PARTY_INCLUDE_DIR} ${_ovf_geometry_headers_dir} ${_ovf_viewfactor_headers_dir} ${_ovf_solvers_headers_dir} ${PROJECT_BINARY_DIR}/include)

set(_test_sources
  test_Vector3.cpp
  test_Triangle.cpp
  test_Triangulation.cpp
  test_BVHNode.cpp
  test_BVH.cpp
  test_ViewFactor.cpp
  test_DoubleAreaIntegration.cpp
  main.cpp
)

# -------------------- BUILD TEST EXECUTABLE -------------------- #
add_executable(ovfAllTests ${_test_sources})
target_link_libraries(ovfAllTests PRIVATE ${GTEST_LIBS} ovfgeometry ovfviewfactor ovfsolvers)

add_test(NAME ovfAllTests COMMAND ovfAllTests WORKING_DIRECTORY ${TESTS_SRC_DIR})