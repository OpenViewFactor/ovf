#ifndef _OVF_SOLVEROPTIONS_H_
#define _OVF_SOLVEROPTIONS_H_

#include "../../config.hpp"

namespace openviewfactor {

  enum SelfIntersectionMode { NONE, BOTH, EMITTER, RECEIVER };
  enum NumericMode { DAI, SAI };
  enum ComputeMode { CPU, CPU_N, GPU, GPU_N };
  enum PrecisionMode { SINGLE, DOUBLE };

  class SolverOptions {
    private:
      SelfIntersectionMode _self_intersection;
      NumericMode _numerics;
      ComputeMode _compute;
      PrecisionMode _precision;
    protected:
    public:
      OVF_HOST_DEVICE SolverOptions();

      OVF_HOST_DEVICE SelfIntersectionMode getSelfIntersectionMode() const;
      OVF_HOST_DEVICE NumericMode getNumericMode() const;
      OVF_HOST_DEVICE ComputeMode getComputeMode() const;
      OVF_HOST_DEVICE PrecisionMode getPrecisionMode() const;

      OVF_HOST_DEVICE SolverOptions& setSelfIntersectionMode(SelfIntersectionMode sim);
      OVF_HOST_DEVICE SolverOptions& setNumericMode(NumericMode nm);
      OVF_HOST_DEVICE SolverOptions& setComputeMode(ComputeMode cm);
      OVF_HOST_DEVICE SolverOptions& setPrecisionMode(PrecisionMode pm);
  };
}

#endif