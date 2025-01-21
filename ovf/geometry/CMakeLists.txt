message("Creating list of geometry headers")

# -------------------- ADD DIRECTORIES FOR COMPILER TO SEARCH FOR INCLUDE FILES -------------------- #

message("Include geometry headers directory and third-party directory")
include_directories(${THIRD_PARTY_INCLUDE_DIR} ${_ovf_geometry_headers_dir})

set(_geometry_headers
    headers/Vector3.hpp
    headers/Ray.hpp
    headers/Triangle.hpp
    headers/Triangulation.hpp
    headers/FileReader.hpp
    headers/STLReader.hpp
    headers/BVHNode.hpp
    headers/BVH.hpp
    headers/Blockers.hpp
)

message("Creating list of geometry sources")

set(_geometry_sources
    src/Vector3.cpp
    src/Ray.cpp
    src/Triangle.cpp
    src/Triangulation.cpp
    src/STLReader.cpp
    src/BVHNode.cpp
    src/BVH.cpp
    src/Blockers.cpp
)

message("Add geometry headers to list")

set(_ovf_geometry_headers)  # create a variable to store the full paths to all geometry-related headers
foreach(_header ${_geometry_headers})
  # take all header files in '_geometry_headers' and prepend its full path
  list(APPEND _ovf_geometry_headers ${CMAKE_CURRENT_SOURCE_DIR}/${_header})
endforeach()

message("Add geometry sources to list")

set(_ovf_geometry_sources)
foreach(_source ${_geometry_sources})
  list(APPEND _ovf_geometry_sources ${CMAKE_CURRENT_SOURCE_DIR}/${_source})
endforeach()

# -------------------- BUILD GEOMETRY LIBRARY -------------------- #

message("Building ovfgeometry static library")

add_library(ovfgeometry STATIC ${_ovf_geometry_sources})  # add a static library target from the geometry source files
target_link_libraries(ovfgeometry PRIVATE OpenMP::OpenMP_CXX)
add_compile_definitions(OVF_VERSION=${OVF_VERSION})

if(CUDAToolkit_FOUND)
  enable_language(CUDA)
  message(STATUS "Set ovfgeometry source files language to CUDA")
  set_source_files_properties(${_ovf_geometry_sources} PROPERTIES LANGUAGE CUDA)
  message(STATUS "Set ovfgeometry library linker language to CUDA")
  set_target_properties(ovfgeometry PROPERTIES LINKER_LANGUAGE CUDA)
  message(STATUS "Link ovfgeometry library to CUDA runtime")
  target_link_libraries(ovfgeometry PRIVATE CUDA::cudart)
  # message(STATUS "Set separable compilation [ON] and resolve device symbols [OFF]")
  # set_target_properties(ovfgeometry PROPERTIES CUDA_SEPARABLE_COMPILATION ON CUDA_RESOLVE_DEVICE_SYMBOLS OFF)
endif()

include_directories(${PROJECT_BINARY_DIR}/include)