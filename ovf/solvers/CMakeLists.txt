message("Creating list of view factor headers")

# -------------------- ADD DIRECTORIES FOR COMPILER TO SEARCH FOR INCLUDE FILES -------------------- #

message("Include Solver headers directory and third-party directory")
include_directories(${THIRD_PARTY_INCLUDE_DIR} ${_ovf_geometry_headers_dir} ${_ovf_viewfactor_headers_dir} ${_ovf_solvers_headers_dir})

set(_solvers_headers
  headers/SolverOptions.hpp
  headers/Solver.hpp
  headers/DoubleAreaIntegration.hpp
)

message("Creating list of Solver sources")

set(_solvers_sources
  src/SolverOptions.cpp
  src/Solver.cpp
  src/DoubleAreaIntegration.cpp
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
target_link_libraries(ovfsolvers ovfgeometry)
add_compile_definitions(OVF_VERSION=${OVF_VERSION})
include_directories(${PROJECT_BINARY_DIR}/include)