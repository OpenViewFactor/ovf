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

message("Adding openviewfactor executable")

add_executable(openviewfactor ${_ovf_cli_sources})
set_target_properties(openviewfactor PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(openviewfactor ovfgeometry ovfviewfactor ovfsolvers Boost::program_options Boost::boost OpenMP::OpenMP_CXX)

if(LINUX)
  message("[Linux] Add binary directory to environment variable")
  # Get the directory where the binary is located
  get_target_property(MY_BINARY_DIR openviewfactor BINARY_DIR)
  # Define the line to append
  set(export_line "export MY_BINARY_DIR=${MY_BINARY_DIR}")
  # Path to the shell configuration file
  set(shell_config_file "$ENV{HOME}/.bashrc")
  file(READ ${shell_config_file} file_contents)
  # Check if the export line already exists in the file
  if(NOT file_contents MATCHES "^export MY_BINARY_DIR=.*$")
    # Append the export line if it doesn't exist
    file(APPEND ${shell_config_file} "${export_line}\n")
  endif()
elseif(WINDOWS)
  message("[Windows] Add binary directory to environment variable")
  get_target_property(MY_BINARY_DIR openviewfactor BINARY_DIR)
  execute_process(COMMAND setx MY_BINARY_DIR "${MY_BINARY_DIR}/Debug")
endif()