cmake_minimum_required(VERSION 3.20)
project(ovf)
set(CMAKE_CXX_STANDARD 20)
add_executable(ovf ovf.cpp)
add_executable(meshanalysis meshAnalysis.cpp)

if(UNIX AND NOT APPLE)
  message(STATUS ">>> Configuring for Linux")
  set(LINUX TRUE)
elseif(WIN32)
  message(STATUS ">>> Configuring for Windows")
  set(WINDOWS TRUE)
endif()

set(CMAKE_BUILD_TYPE Release)

set(THIRD_PARTY_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/third_party)
include_directories(${THIRD_PARTY_INCLUDE_DIR})
include_directories(headers)
add_subdirectory(${THIRD_PARTY_INCLUDE_DIR}/build-boost)
add_subdirectory(${THIRD_PARTY_INCLUDE_DIR}/lean-vtk)

find_package(OpenMP REQUIRED)

set_target_properties(ovf PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(ovf Boost::program_options Boost::boost LeanVTK OpenMP::OpenMP_CXX)
target_link_libraries(meshanalysis Boost::program_options Boost::boost LeanVTK OpenMP::OpenMP_CXX)

if(LINUX)
  #TODO FIX THIS BIT HERE.
  # - NOT PROPERLY CHECKING THE SHELL CONFIG FILE FOR THE LINE

  message("[Linux] Add binary directory to environment variable")
  # Get the directory where the binary is located
  get_target_property(MY_BINARY_DIR ovf BINARY_DIR)
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
  get_target_property(MY_BINARY_DIR ovf BINARY_DIR)
  execute_process(COMMAND setx MY_BINARY_DIR "${MY_BINARY_DIR}/Debug")
  
endif()