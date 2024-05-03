#ifndef OVF_DOUBLEAREAINTEGRATION_H_
#define OVF_DOUBLEAREAINTEGRATION_H_

#include "solver/include/Solver.hpp"

namespace openviewfactor {
  template <typename FLOAT_TYPE> class DoubleAreaIntegration : public Solver<FLOAT_TYPE> {

    using V3 = Vector3<FLOAT_TYPE>;
    using T = Triangle<FLOAT_TYPE>;
    using TGLN = Triangulation<FLOAT_TYPE>;

    public:
      DoubleAreaIntegration();
      DoubleAreaIntegration(const SolverOptions<FLOAT_TYPE> &options);

      std::unique_ptr<ViewFactor<FLOAT_TYPE>> solveViewFactor(const Triangulation<FLOAT_TYPE> &mesh);
      std::unique_ptr<ViewFactor<FLOAT_TYPE>> solveViewFactor(const Triangulation<FLOAT_TYPE> &emitter_mesh,
                                                              const Triangulation<FLOAT_TYPE> &receiver_mesh);
  };
}

#endif