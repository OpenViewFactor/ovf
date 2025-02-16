#include "SolverOptions.hpp"

namespace openviewfactor {
  gpuify SolverOptions::SolverOptions()
    : _self_intersection(BOTH), _back_face_cull(ON), _numerics(DAI), _compute(CPU_N), _precision(DOUBLE) {}
  
  gpuify SelfIntersectionMode SolverOptions::getSelfIntersectionMode() const { return _self_intersection; }
  gpuify BackFaceCullMode SolverOptions::getBackFaceCullMode() const { return _back_face_cull; }
  gpuify NumericMode SolverOptions::getNumericMode() const { return _numerics; }
  gpuify ComputeMode SolverOptions::getComputeMode() const { return _compute; }
  gpuify PrecisionMode SolverOptions::getPrecisionMode() const { return _precision; }

  gpuify SolverOptions& SolverOptions::setSelfIntersectionMode(SelfIntersectionMode sim) { _self_intersection = sim; return *this;}
  gpuify SolverOptions& SolverOptions::setBackFaceCullMode(BackFaceCullMode bfm) { _back_face_cull = bfm; return *this; }
  gpuify SolverOptions& SolverOptions::setNumericMode(NumericMode nm) { _numerics = nm; return *this; }
  gpuify SolverOptions& SolverOptions::setComputeMode(ComputeMode cm) { _compute = cm; return *this; }
  gpuify SolverOptions& SolverOptions::setPrecisionMode(PrecisionMode pm) { _precision = pm; return *this; }
}