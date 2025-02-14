include_directories(${_ovf_geometry_headers_dir})
include_directories(${THIRD_PARTY_INCLUDE_DIR})

set(_analysis_sources
  main.cpp
)

set(_ovf_analysis_sources)
foreach(_source ${_analysis_sources})
  list(APPEND _ovf_analysis_sources ${CMAKE_CURRENT_SOURCE_DIR}/${_source})
endforeach()

# -------------------- BUILD COMMAND-LINE EXECUTABLE -------------------- #

message("Adding meshanalysis executable")

add_executable(meshanalysis ${_ovf_analysis_sources})
set_target_properties(meshanalysis PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(meshanalysis ovfgeometry Boost::program_options Boost::boost OpenMP::OpenMP_CXX LeanVTK)