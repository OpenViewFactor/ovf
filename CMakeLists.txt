# Project-level CMake file
cmake_minimum_required(VERSION 3.20)

# Name the project
project(OpenViewFactor)

# Set the project version
set(OVF_CORE_VERSION 0.1)
set(OVF_CLI_VERSION 0.1)

# Set a C++ version
set(CMAKE_CXX_STANDARD 17)

# Add VTK Support
add_subdirectory(third_party/lean-vtk)

# Add Boost
add_subdirectory(third_party/boost-cmake)

# Add the Compiler Flag Options
option(OVF_BUILD_OMP "Build OpenMP Solver Implementation" ON) # default OMP to on
option(OVF_BUILD_CUDA "Build CUDA Solver Implementation" OFF) # default CUDA to off
option(OVF_BUILD_CLI "Build the Command-Line Interface" ON) # default to build the CLI

# Add paths
set(THIRD_PARTY_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR})  # directory from which third_party directory branches

# Locate the OpenMP Package
if(OVF_BUILD_OMP)
  find_package(OpenMP)  # shouldn't really error because OpenMP is usually just included
  message("OVF OpenMP Enabled")
endif()

# Locate CUDA
if(OVF_BUID_CUDA)
  find_package(CUDAToolkit REQUIRED)
  enable_language(CUDA)
  message("OVF CUDA Enabled")
  set(CMAKE_CUDA_FLAGS_DEBUG ${CMAKE_CUDA_FLAGS_DEBUG} " -g -G ") # enables CUDA debugging
endif()

# Pull the GoogleTest library
if(OVF_BUILD_GTEST)
  add_subdirectory(third_party/google-test)
  set(GTEST_LIBS gtest gtest_main)
  set(GTEST_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/third_party/google-test/include)
endif()

# Add project directory
add_subdirectory(ovf)