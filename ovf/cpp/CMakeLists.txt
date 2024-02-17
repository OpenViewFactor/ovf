# Build a cpp solver library
add_library(ovfsolverscpp STATIC ${_ovf_cpp_solver_srcs}) # using the variable defined in wider directory
target_compile_definitions(ovfsolverscpp PRIVATE OVF_IMPL_CPP)

if(OVF_BUILD_OMP) # if OpenMP is enabled
  add_library(ovfsolversomp STATIC ${_ovf_omp_solver_srcs}) # build an omp solver library
  target_link_libraries(ovfsolversomp PUBLIC OpenMP::OpenMP_CXX)
  target_compile_definitions(ovfsolversomp PRIVATE OVF_IMPL_OMP)
endif()