#ifndef _OVF_SOLVEROPTIONS_H_
#define _OVF_SOLVEROPTIONS_H_

#include "../../config.hpp"

namespace openviewfactor {

  enum SelfIntersectionMode { NONE, BOTH, EMITTER, RECEIVER };
  enum BackFaceCullMode { ON, OFF };
  enum NumericMode { DAI, SAI };
  enum ComputeMode { CPU, CPU_N, GPU, GPU_N };
  enum PrecisionMode { SINGLE, DOUBLE };

  class SolverOptions {
    private:
      SelfIntersectionMode _self_intersection;
      BackFaceCullMode _back_face_cull;
      NumericMode _numerics;
      ComputeMode _compute;
      PrecisionMode _precision;
    protected:
    public:
      gpuify SolverOptions();

      gpuify SelfIntersectionMode getSelfIntersectionMode() const;
      gpuify BackFaceCullMode getBackFaceCullMode() const;
      gpuify NumericMode getNumericMode() const;
      gpuify ComputeMode getComputeMode() const;
      gpuify PrecisionMode getPrecisionMode() const;

      gpuify SolverOptions& setSelfIntersectionMode(SelfIntersectionMode sim);
      gpuify SolverOptions& setBackFaceCullMode(BackFaceCullMode bfm);
      gpuify SolverOptions& setNumericMode(NumericMode nm);
      gpuify SolverOptions& setComputeMode(ComputeMode cm);
      gpuify SolverOptions& setPrecisionMode(PrecisionMode pm);
  };
}

#endif