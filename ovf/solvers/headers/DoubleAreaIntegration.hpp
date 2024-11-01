#ifndef _OVF_DOUBLEAREAINTEGRATION_H_
#define _OVF_DOUBLEAREAINTEGRATION_H_

#include <vector>
#include <algorithm>

#include "Solver.hpp"

namespace openviewfactor {
  template <typename FLOAT_TYPE> class DoubleAreaIntegration : public Solver {
    private:
    protected:
    public:
      OVF_HOST_DEVICE std::vector<unsigned int> backFaceCull(std::shared_ptr<Triangulation<FLOAT_TYPE>> emitter_mesh, std::shared_ptr<Triangulation<FLOAT_TYPE>> receiver_mesh) const;

      OVF_HOST_DEVICE std::vector<unsigned int> evaluateBlocking(std::shared_ptr<Triangulation<FLOAT_TYPE>> emitter_mesh, std::shared_ptr<Triangulation<FLOAT_TYPE>> receiver_mesh, std::vector<unsigned int> unculled_indices, Blockers<FLOAT_TYPE> blockers) const;

      OVF_HOST_DEVICE std::unique_ptr<ViewFactor<FLOAT_TYPE>> solveViewFactor(std::shared_ptr<Triangulation<FLOAT_TYPE>> emitter_mesh, std::shared_ptr<Triangulation<FLOAT_TYPE>> receiver_mesh, std::vector<unsigned int> unblocked_indices) const;
  };
}

#endif