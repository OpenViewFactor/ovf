if(OVF_BUILD_CUDA)
  add_library(ovfsolverscuda STATIC ${_ovf_cuda_solver_srcs})
  set_source_files_properties(${_ovf_cuda_solver_srcs} PROPERTIES LANGUAGE CUDA)
  target_compile_definitions(ovfsolverscuda PRIVATE OVF_IMPL_CUDA OVF_SUPPORT_CUDA)
  set_target_properties(ovfsolverscuda PROPERTIES LINKER_LANGUAGE CUDA)
  target_link_libraries(ovfsolverscuda PRIVATE ovfcore) # ovfcore is used only in the implementations of the ovfsolverscuda library, but not in the headers?
  set_target_properties(ovfsolverscuda PROPERTIES CUDA_SEPARABLE_COMPILATION ON CUDA_RESOLVE_DEVICE_SYMBOLS ON) # not sure why this is different than the other time we used this command to build for cuda
endif()