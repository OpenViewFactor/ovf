message("Creating list of view factor headers")

# -------------------- ADD DIRECTORIES FOR COMPILER TO SEARCH FOR INCLUDE FILES -------------------- #

message("Include view factor headers directory and third-party directory")
include_directories(${THIRD_PARTY_INCLUDE_DIR} ${_ovf_geometry_headers_dir} ${_ovf_viewfactor_headers_dir})

set(_viewfactor_headers
    headers/ViewFactor.hpp
    headers/OutputWriter.hpp
)

message("Creating list of viewfactor sources")

set(_viewfactor_sources
    src/ViewFactor.cpp
    src/OutputWriter.cpp
)

message("Add viewfactor headers to list")

set(_ovf_viewfactor_headers)  # create a variable to store the full paths to all geometry-related headers
foreach(_header ${_viewfactor_headers})
  # take all header files in '_geometry_headers' and prepend its full path
  list(APPEND _ovf_viewfactor_headers ${CMAKE_CURRENT_SOURCE_DIR}/${_header})
endforeach()

message("Add viewfactor sources to list")

set(_ovf_viewfactor_sources)
foreach(_source ${_viewfactor_sources})
  list(APPEND _ovf_viewfactor_sources ${CMAKE_CURRENT_SOURCE_DIR}/${_source})
endforeach()

# -------------------- BUILD GEOMETRY LIBRARY -------------------- #

message("Check for OpenMP")
find_package(OpenMP REQUIRED)
message("OVF OpenMP Enabled")

message("Building ovfviewfactor static library")

add_library(ovfviewfactor STATIC ${_ovf_viewfactor_sources})
target_link_libraries(ovfviewfactor OpenMP::OpenMP_CXX LeanVTK)
add_compile_definitions(OVF_VERSION=${OVF_VERSION})
include_directories(${PROJECT_BINARY_DIR}/include)