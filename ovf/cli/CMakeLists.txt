set(_cli_sources
  main.cpp
)

message("Add cli sources to list")

set(_ovf_cli_sources)
foreach(_source ${_cli_sources})
  list(APPEND _ovf_cli_sources ${CMAKE_CURRENT_SOURCE_DIR}/${_source})
endforeach()

# -------------------- BUILD COMMAND-LINE EXECUTABLE -------------------- #

message("Adding openviewfactor executable")

add_executable(openviewfactor ${_ovf_cli_sources})
set_target_properties(openviewfactor PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(openviewfactor ovfgeometry Boost::program_options Boost::boost)