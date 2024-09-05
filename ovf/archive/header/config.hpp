// This file tells NVCC to build for host and device if CUDA is enabled
// If CUDA isn't enabled, it just does nothing

// OVF_HOST_DEVICE must precede any code that would have to be compiled for device code

#ifdef OVF_SUPPORT_CUDA // compiler flag from CMake

#define OVF_HOST_DEVICE

  __host__ __device__

#else

#define OVF_HOST_DEVICE

#endif