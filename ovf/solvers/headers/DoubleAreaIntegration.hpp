#ifndef _OVF_DOUBLEAREAINTEGRATION_H_
#define _OVF_DOUBLEAREAINTEGRATION_H_

#define _USE_MATH_DEFINES
#include <cmath>
#include <numeric>

#include <vector>
#include <algorithm>

#include "Vector3.hpp"
#include "Triangle.hpp"
#include "Triangulation.hpp"
#include "BVH.hpp"
#include "Ray.hpp"
#include "Solver.hpp"

namespace openviewfactor {
  template <typename FLOAT_TYPE> class DoubleAreaIntegration : public Solver<FLOAT_TYPE> {
    private:
    protected:
    public:
      OVF_HOST_DEVICE FLOAT_TYPE solveViewFactorBetweenElements(const Triangle<FLOAT_TYPE>& emitter_element, const Triangle<FLOAT_TYPE>& receiver_element) const;
  };
}

#endif