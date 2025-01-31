cmake_minimum_required(VERSION 3.20)
project(OpenViewFactor CXX)
set(OVF_VERSION 0.2)
set(CMAKE_CXX_STANDARD 17)

message("Beginning OVF Build")

if(UNIX AND NOT APPLE)
  message(STATUS ">>> Configuring for Linux")
  set(LINUX TRUE)
elseif(WIN32)
  message(STATUS ">>> Configuring for Windows")
  set(WINDOWS TRUE)
endif()

# -------------------- ADD COMPILE OPTIONS -------------------- #

message("Add OVF Build Options")

# option(OVF_BUILD_OMP "Build OpenMP solver implementations (Default: ON)" ON)
# option(OVF_BUILD_CUDA "Build CUDA solver implementations (Default: OFF)")
# option(OVF_BUILD_GTEST "Build unit tests (Default: OFF)" ON)
# option(OVF_BUILD_BENCHMARK "Build benchmarking tests (Default: OFF)")

# -------------------- ADD SUBDIRECTORIES -------------------- #

message("Adding Subdirectories to root CMakeLists file")

set(THIRD_PARTY_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/third_party)
set(GTEST_INCLUDE_DIR ${THIRD_PARTY_INCLUDE_DIR}/google-test/googletest/include/gtest)
set(GTEST_LIBS gtest gtest_main CACHE STRING "gtest libraries")
set(_ovf_geometry_headers_dir ${CMAKE_CURRENT_SOURCE_DIR}/ovf/geometry/headers)
set(_ovf_viewfactor_headers_dir ${CMAKE_CURRENT_SOURCE_DIR}/ovf/viewfactor/headers)
set(_ovf_solvers_headers_dir ${CMAKE_CURRENT_SOURCE_DIR}/ovf/solvers/headers)

add_subdirectory(${THIRD_PARTY_INCLUDE_DIR}/lean-vtk)
add_subdirectory(${THIRD_PARTY_INCLUDE_DIR}/build-boost)
add_subdirectory(${THIRD_PARTY_INCLUDE_DIR}/google-test)

message("Check for OpenMP")
find_package(OpenMP REQUIRED)

message("Check for CUDA")
find_package(CUDAToolkit)
message(STATUS "CUDAToolkit_FOUND: ${CUDAToolkit_FOUND}")

if(CUDAToolkit_FOUND)
  message(STATUS "CUDA Supported")
  # set(OVF_SUPPORT_CUDA TRUE)
else()
  message(STATUS "No CUDA Support")
endif()

# message("OVF CUDA Enabled")
# set(CMAKE_CUDA_FLAGS_DEBUG ${CMAKE_CUDA_FLAGS_DEBUG} " -g -G ") # this enables the cuda-gdb
# set(CUDA_NVCC_FLAGS "${CUDA_NVCC_FLAGS} --ptxas-options=-v")

add_subdirectory(ovf)