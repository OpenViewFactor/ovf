# Pull source and header files
include(./include/CMakeLists.txt)
include(./src/CMakeLists.txt)

# ---------- Add list of headers ---------- #
set(_ovf_public_headers)
foreach(_header ${_public_headers}) # referencing all the header files listed in the `_public_headers` variable defined in `include/CMakeLists.txt`
  list(APPEND _ovf_public_headers ${CMAKE_CURRENT_SOURCE_DIR}/include/${_header}) # add each header, including its location in the include directory
endforeach()

# Define the directory for all public headers
set(_ovf_public_header_dir ${CMAKE_CURRENT_SOURCE_DIR}/include)

# ---------- Add list of core source files ---------- #
set(_ovf_core_srcs)
foreach(_src ${_core_srcs}) # referencing all the header files listed in the `_core_srcs` variable defined in `src/CMakeLists.txt`
  list(APPEND _ovf_core_srcs ${CMAKE_CURRENT_SOURCE_DIR}/src/${_src})
endforeach()

# ---------- Add list of solver files ---------- #
set(_ovf_cpp_solver_srcs)
foreach(_src ${_ovf_cpp_solver_srcs})
  list(APPEND _ovf_cpp_solver_srcs ${CMAKE_CURRENT_SOURCE_DIR}/src/${_src})
endforeach()
set(_ovf_omp_solver_srcs)
foreach(_src ${_ovf_omp_solver_srcs})
  list(APPEND _ovf_omp_solver_srcs ${CMAKE_CURRENT_SOURCE_DIR}/src/${_src})
endforeach()
set(_ovf_cuda_solver_srcs)
foreach(_src ${_ovf_cuda_solver_srcs})
  list(APPEND _ovf_cuda_solver_srcs ${CMAKE_CURRENT_SOURCE_DIR}/src/${_src})
endforeach()

# ---------- Add list of solver manager files ---------- #
set(_ovf_manager_srcs)
foreach(_src ${_manager_srcs})
  list(APPEND _ovf_manager_srcs ${CMAKE_CURRENT_SOURCE_DIR}/src/${_src})
endforeach()

# ---------- Add list of cli files ---------- #
set(_ovf_cli_srcs)
foreach(_src ${_cli_srcs})
list(APPEND _ovf_cli_srcs ${CMAKE_CURRENT_SOURCE_DIR}/src/${_src})
endforeach()

# ---------- Include Directories ---------- #
include_directories(${_ovf_public_header_dir})  # ovf include files
include_directories(${THIRD_PARTY_INCLUDE_DIR}) # third party modules

# <><><><><> BUILD THE CORE LIBRARY <><><><><> #

add_library(ovfcore STATIC ${_ovf_core_srcs}) # create a new `ovfcore` library comprised of core source files
add_compile_definitions(OVF_CORE_VERSION = ${OVF_CORE_VERSION})
target_link_libraries(ovfcore PRIVATE LeanVTK)  # link `LeanVTK` to `ovfcore` for visualization
include_directories(${PROJECT_BINARY_DIR}/include)  # ?? i don't understand this line

if(OVF_BUILD_CUDA) # if cuda is enabled
  if(MSVC)         # if the active compiler is MSVC
    set(CMAKE_EXE_LINKER_FLAGS /NODEFAULTLIB:LIBCMT)  # don't link the LIBCMT library (?)
  endif()

  target_compile_definitions(ovfcore PRIVATE OVF_SUPPORT_CUDA)  # enable compilation for CUDA
  set_source_files_properties(${_ovf_core_srcs} PROPERTIES LANGUAGE CUDA) # compile the core sources for CUDA
  target_link_libraries(ovfcore PRIVATE CUDA::cudart) # link the cuda runtime library to ovfcore
  set_target_properties(ovfcore PROPERTIES CUDA_SEPARABLE_COMPILATION ON CUDA_RESOLVE_DEVICE_SYMBOLS OFF) # compile device code separately
endif()

# <><><><><> END CORE BUILD LIBRARY <><><><><> #

# ---------- Build the CPU and CUDA libaries ---------- #
add_subdirectory(cpp)
add_subdirectory(cuda)

# ---------- Build the Solver Interface ---------- #
add_library(ovfmanager STATIC ${_ovf_manager_srcs})
target_link_libraries(ovfmanager PUBLIC ovfsolverscpp)
if(OVF_BUILD_OMP)
  target_link_libraries(ovfmanager PUBLIC ovfsolversomp)
  target_compile_definitions(ovfmanager PRIVATE OVF_MANAGER_SUPPORT_OMP)
endif()
if(OVF_BUILD_CUDA)
  target_link_libraries(ovfmanager PUBLIC ovfsolverscuda)
  set_target_properties(ovfmanager PROPERTIES LINKER_LANGUAGE CUDA)
  target_compile_definitions(ovfmanager PRIVATE OVF_MANAGER_SUPPORT_CUDA)
endif()

# ---------- Build the CLI ---------- #
if(OVF_BUILD_CLI)
  add_executable(openviewfactor ${_ofv_cli_srcs})
  set_target_properties(openviewfactor PROPERTIES LINKER_LANGUAGE CXX)
  target_link_libraries(openviewfactor ovfcore ovfmanager Boost::program_options Boost::boost)
endif()

# ---------- Add Test Subdirectory ---------- #
add_subdirectory(test)