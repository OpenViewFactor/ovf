message("Creating list of geometry headers")

# -------------------- ADD DIRECTORIES FOR COMPILER TO SEARCH FOR INCLUDE FILES -------------------- #

message("Include geometry headers directory and third-party directory")
include_directories(${_ovf_geometry_headers_dir})
include_directories(${THIRD_PARTY_INCLUDE_DIR}) # this variable gets defined in the CMakeLists file one level up

set(_geometry_headers
    headers/Vector3.hpp
    headers/Ray.hpp
    headers/Triangle.hpp
    headers/Triangulation.hpp
    headers/BVHNode.hpp
    headers/BVH.hpp
    headers/FileReader.hpp
    headers/STLReader.hpp
)

message("Creating list of geometry sources")

set(_geometry_sources
    src/Vector3.cpp
    src/Ray.cpp
    src/Triangle.cpp
    src/Triangulation.cpp
    src/BVHNode.cpp
    src/BVH.cpp
    src/STLReader.cpp
)

message("Add geometry headers to list")

set(_ovf_geometry_headers)  # create a variable to store the full paths to all geometry-related headers
foreach(_header ${_geometry_headers})
  # take all header files in '_geometry_headers' and prepend its full path
  list(APPEND _ovf_geometry_headers ${CMAKE_CURRENT_SOURCE_DIR}/${_header})
endforeach()

set(_ovf_geometry_headers_dir ${CMAKE_CURRENT_SOURCE_DIR}/headers) # create a variable to store the directory for all geometry-related code

message("Add geometry sources to list")

set(_ovf_geometry_sources)
foreach(_source ${_geometry_sources})
  list(APPEND _ovf_geometry_sources ${CMAKE_CURRENT_SOURCE_DIR}/${_source})
endforeach()

# -------------------- BUILD GEOMETRY LIBRARY -------------------- #

message("Building ovfgeometry static library")

add_library(ovfgeometry STATIC ${_ovf_geometry_sources})  # add a static library target from the geometry source files
add_compile_definitions(OVF_VERSION=${OVF_VERSION})
include_directories(${PROJECT_BINARY_DIR}/include)