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
      OVF_HOST_DEVICE std::unique_ptr<ViewFactor<FLOAT_TYPE>> solveViewFactor(std::shared_ptr<Triangulation<FLOAT_TYPE>> emitter_mesh, std::shared_ptr<Triangulation<FLOAT_TYPE>> receiver_mesh, std::vector<unsigned int> unblocked_indices) const;
  };
}

#endif