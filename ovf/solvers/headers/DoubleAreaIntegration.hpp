#ifndef _OVF_DOUBLEAREAINTEGRATION_H_
#define _OVF_DOUBLEAREAINTEGRATION_H_

#include "Solver.hpp"

// #define _USE_MATH_DEFINES
// #include <cmath>
// #include <numeric>

namespace openviewfactor {
  template <typename t> class DoubleAreaIntegration : public Solver<t> {
    private:
    protected:
    public:
      gpuify t solveViewFactorBetweenElements(const Vector3<t>& emitter_centroid, const Vector3<t>& emitter_normal, const Triangle<t>& receiver_element) const;
  };
}

#endif