message("Creating list of view factor headers")

# -------------------- ADD DIRECTORIES FOR COMPILER TO SEARCH FOR INCLUDE FILES -------------------- #

message("Include Solver headers directory and third-party directory")
include_directories(${THIRD_PARTY_INCLUDE_DIR} ${_ovf_geometry_headers_dir} ${_ovf_viewfactor_headers_dir} ${_ovf_solvers_headers_dir})

set(_solvers_headers
  headers/SolverOptions.hpp
  headers/Solver.hpp
  headers/DoubleAreaIntegration.hpp
  headers/SingleAreaIntegration.hpp
)

message("Creating list of Solver sources")

set(_solvers_sources
  src/SolverOptions.cpp
  src/Solver.cpp
  src/DoubleAreaIntegration.cpp
  src/SingleAreaIntegration.cpp
)

message("Add Solver headers to list")

set(_ovf_solvers_headers)
foreach(_header ${_solvers_headers})
  list(APPEND _ovf_solvers_headers ${CMAKE_CURRENT_SOURCE_DIR}/${_header})
endforeach()

message("Add viewfactor sources to list")

set(_ovf_solvers_sources)
foreach(_source ${_solvers_sources})
  list(APPEND _ovf_solvers_sources ${CMAKE_CURRENT_SOURCE_DIR}/${_source})
endforeach()

# -------------------- BUILD GEOMETRY LIBRARY -------------------- #

message("Building ovfsolver static library")

add_library(ovfsolvers STATIC ${_ovf_solvers_sources})
target_link_libraries(ovfsolvers PRIVATE ovfgeometry ovfviewfactor OpenMP::OpenMP_CXX)
add_compile_definitions(OVF_VERSION=${OVF_VERSION})

if(CUDAToolkit_FOUND)
  enable_language(CUDA)
  message(STATUS "Set ovfsovlers source files language to CUDA")
  set_source_files_properties(${_ovf_solvers_sources} PROPERTIES LANGUAGE CUDA)
  message(STATUS "Set ovfsolvers library linker language to CUDA")
  set_target_properties(ovfsolvers PROPERTIES LINKER_LANGUAGE CUDA)
  message(STATUS "Link ovfsolvers library to CUDA runtime")
  target_link_libraries(ovfsolvers PRIVATE CUDA::cudart)
  # message(STATUS "Set separable compilation [ON] and resolve device symbols [OFF]")
  set_target_properties(ovfsolvers PROPERTIES CUDA_RESOLVE_DEVICE_SYMBOLS ON)
endif()

include_directories(${PROJECT_BINARY_DIR}/include)