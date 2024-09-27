cmake_minimum_required(VERSION 3.20)
project(OpenViewFactor)
set(OVF_VERSION 0.2)
set(CMAKE_CXX_STANDARD 17)

message("Beginning OVF Build")

# -------------------- ADD SUBDIRECTORIES -------------------- #

message("Adding Subdirectories to root CMakeLists file")

message("Checking whether Google Test is enabled in root directory CMakeLists")
if(OVF_BUILD_GTEST)
  message("Google Test is enabled in root directory CMakeLists")
  set(GTEST_LIBS gtest gtest_main)
  set(GTEST_INCLUDE_DIR ${THIRD_PARTY_INCLUDE_DIR}/google-test/googletest/include)
endif()

add_subdirectory(ovf)

set(THIRD_PARTY_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/third_party)
add_subdirectory(${THIRD_PARTY_INCLUDE_DIR}/lean-vtk)
add_subdirectory(${THIRD_PARTY_INCLUDE_DIR}/boost-cmake)
add_subdirectory(${THIRD_PARTY_INCLUDE_DIR}/google-test)

# -------------------- ADD COMPILE OPTIONS -------------------- #

message("Add OVF Build Options")

option(OVF_BUILD_OMP "Build OpenMP solver implementations (Default: ON)" ON)
option(OVF_BUILD_CUDA "Build CUDA solver implementations (Default: OFF)")
option(OVF_BUILD_GTEST "Build unit tests (Default: OFF)" ON)
option(OVF_BUILD_BENCHMARK "Build benchmarking tests (Default: OFF)")

# -------------------- DEALING WITH COMPILE OPTIONS -------------------- #

if(OVF_BUILD_OMP)
  message("Check for OpenMP")
  find_package(OpenMP)
  message("OVF OpenMP Enabled")
endif()

if (OVF_BUILD_CUDA)
  message("Check for CUDA")
  find_package(CUDAToolkit REQUIRED)
  enable_language(CUDA)
  message("OVF CUDA Enabled")
  set(CMAKE_CUDA_FLAGS_DEBUG ${CMAKE_CUDA_FLAGS_DEBUG} " -g -G ") # this enables the cuda-gdb
  set(CUDA_NVCC_FLAGS "${CUDA_NVCC_FLAGS} --ptxas-options=-v")
endif()