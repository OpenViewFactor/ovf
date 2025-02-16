// This file tells NVCC to build for host and device if CUDA is enabled
// If CUDA isn't enabled, it just does nothing

// gpuify must precede any code that would have to be compiled for device code

#ifndef CONFIG_HPP_
#define CONFIG_HPP_

#ifdef OVF_SUPPORT_CUDA // compiler flag from CMake

#define gpuify

  __host__ __device__

#else

#define gpuify

#endif

#endif

#define OVF_PI 3.1415926535897932