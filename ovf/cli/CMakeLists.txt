include_directories(${_ovf_geometry_headers_dir})
include_directories(${_ovf_viewfactor_headers_dir})
include_directories(${_ovf_solvers_headers_dir})
include_directories(${THIRD_PARTY_INCLUDE_DIR})

set(_cli_sources
  main.cpp
)

message("Add cli sources to list")

set(_ovf_cli_sources)
foreach(_source ${_cli_sources})
  list(APPEND _ovf_cli_sources ${CMAKE_CURRENT_SOURCE_DIR}/${_source})
endforeach()

# -------------------- BUILD COMMAND-LINE EXECUTABLE -------------------- #

message("Check for OpenMP")
find_package(OpenMP REQUIRED)
message("OVF OpenMP Enabled")

message("Adding openviewfactor executable")

add_executable(openviewfactor ${_ovf_cli_sources})
set_target_properties(openviewfactor PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(openviewfactor ovfgeometry ovfviewfactor ovfsolvers Boost::program_options Boost::boost OpenMP::OpenMP_CXX)

message("Add binary directory to a user-scope environment variable")
get_target_property(MY_BINARY_DIR openviewfactor BINARY_DIR)
execute_process(COMMAND setx MY_BINARY_DIR "${MY_BINARY_DIR}")