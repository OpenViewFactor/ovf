include(./geometry/CMakeLists.txt)

set(_ovf_geometry_headers)  # create a variable to store the full paths to all geometry-related headers
foreach(_header ${_geometry_headers})
  # take all header files in '_geometry_headers' and prepend its full path
  list(APPEND _ovf_geometry_headers ${CMAKE_CURRENT_SOURCE_DIR}/geometry/${_header})
endforeach()

set(_ovf_geometry_dir ${CMAKE_CURRENT_SOURCE_DIR}/geometry) # create a variable to store the directory for all geometry-related code

set(_ovf_geometry_sources)
foreach(_source ${_geometry_sources})
  list(APPEND _ovf_geometry_sources ${CMAKE_CURRENT_SOURCE_DIR}/geometry/${_source})
endforeach()

include_directories(${_ovf_geometry_dir})
include_directories(${THIRD_PARTY_INCLUDE_DIR}) # this variable gets defined in the CMakeLists file one level up

# -------------------- BUILD GEOMETRY LIBRARY -------------------- #

add_library(ovfgeometry STATIC ${_ovf_geometry_sources})  # add a static library target from the geometry source files
include_directories(${PROJECT_BINARY_DIR}/include)