include(./geometry/CMakeLists.txt)
include(./cli/CMakeLists.txt)

add_subdirectory(tests)

# -------------------- CREATE VARIABLES FOR LISTS OF HEADERS AND SOURCE FILES -------------------- #

set(_ovf_geometry_headers)  # create a variable to store the full paths to all geometry-related headers
foreach(_header ${_geometry_headers})
  # take all header files in '_geometry_headers' and prepend its full path
  list(APPEND _ovf_geometry_headers ${CMAKE_CURRENT_SOURCE_DIR}/geometry/${_header})
endforeach()

set(_ovf_geometry_headers_dir ${CMAKE_CURRENT_SOURCE_DIR}/geometry/headers) # create a variable to store the directory for all geometry-related code

set(_ovf_geometry_sources)
foreach(_source ${_geometry_sources})
  list(APPEND _ovf_geometry_sources ${CMAKE_CURRENT_SOURCE_DIR}/geometry/${_source})
endforeach()

set(_ovf_cli_sources)
foreach(_source ${_cli_sources})
  list(APPEND _ovf_cli_sources ${CMAKE_CURRENT_SOURCE_DIR}/cli/${_source})
endforeach()

# -------------------- ADD DIRECTORIES FOR COMPILER TO SEARCH FOR INCLUDE FILES -------------------- #

include_directories(${_ovf_geometry_headers_dir})
include_directories(${THIRD_PARTY_INCLUDE_DIR}) # this variable gets defined in the CMakeLists file one level up

# -------------------- BUILD GEOMETRY LIBRARY -------------------- #

add_library(ovfgeometry STATIC ${_ovf_geometry_sources})  # add a static library target from the geometry source files
add_compile_definitions(OVF_VERSION=${OVF_VERSION})
include_directories(${PROJECT_BINARY_DIR}/include)

# -------------------- BUILD COMMAND-LINE EXECUTABLE -------------------- #
add_executable(openviewfactor ${_ovf_cli_sources})
set_target_properties(openviewfactor PROPERTIES LINKER_LANGUAGE CXX)

target_link_libraries(openviewfactor ovfgeometry Boost::program_options Boost::boost)