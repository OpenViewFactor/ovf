cmake_minimum_required(VERSION 3.20)
project(OpenViewFactor)

set(OVF_VERSION 0.2)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(third_party/lean-vtk)
add_subdirectory(third_party/boost-cmake)

add_subdirectory(ovf)