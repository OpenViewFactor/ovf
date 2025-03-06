#include "SolverOptions.hpp"

namespace openviewfactor {
  OVF_HOST_DEVICE SolverOptions::SolverOptions()
    : _self_intersection(BOTH), _back_face_cull(ON), _numerics(DAI), _compute(CPU_N), _precision(DOUBLE) {}
  
  OVF_HOST_DEVICE SelfIntersectionMode SolverOptions::getSelfIntersectionMode() const { return _self_intersection; }
  OVF_HOST_DEVICE BackFaceCullMode SolverOptions::getBackFaceCullMode() const { return _back_face_cull; }
  OVF_HOST_DEVICE NumericMode SolverOptions::getNumericMode() const { return _numerics; }
  OVF_HOST_DEVICE ComputeMode SolverOptions::getComputeMode() const { return _compute; }
  OVF_HOST_DEVICE PrecisionMode SolverOptions::getPrecisionMode() const { return _precision; }

  OVF_HOST_DEVICE SolverOptions& SolverOptions::setSelfIntersectionMode(SelfIntersectionMode sim) { _self_intersection = sim; return *this;}
  OVF_HOST_DEVICE SolverOptions& SolverOptions::setBackFaceCullMode(BackFaceCullMode bfm) { _back_face_cull = bfm; return *this; }
  OVF_HOST_DEVICE SolverOptions& SolverOptions::setNumericMode(NumericMode nm) { _numerics = nm; return *this; }
  OVF_HOST_DEVICE SolverOptions& SolverOptions::setComputeMode(ComputeMode cm) { _compute = cm; return *this; }
  OVF_HOST_DEVICE SolverOptions& SolverOptions::setPrecisionMode(PrecisionMode pm) { _precision = pm; return *this; }
}