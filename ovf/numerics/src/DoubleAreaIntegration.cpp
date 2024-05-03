#include "numerics/include/DoubleAreaIntegration.hpp"

namespace openviewfactor {
  template <typename FLOAT_TYPE>
  DoubleAreaIntegration<FLOAT_TYPE>::DoubleAreaIntegration() : Solver<FLOAT_TYPE>() {}
  template <typename FLOAT_TYPE>
  DoubleAreaIntegration<FLOAT_TYPE>::DoubleAreaIntegration(const SolverOptions<FLOAT_TYPE> &options) : Solver<FLOAT_TYPE>(options) {}

  template <typename FLOAT_TYPE>
  std::unique_ptr<ViewFactor<FLOAT_TYPE>> DoubleAreaIntegration<FLOAT_TYPE>::solveViewFactor(const Triangulation<FLOAT_TYPE> &mesh) {
    return solveViewFactor(mesh, mesh);
  }
  template <typename FLOAT_TYPE>
  std::unique_ptr<ViewFactor<FLOAT_TYPE>> DoubleAreaIntegration<FLOAT_TYPE>::solveViewFactor(const Triangulation<FLOAT_TYPE> &emitter_mesh,
                                                                                             const Triangulation<FLOAT_TYPE> &receiver_mesh) {
    auto num_emitter_elements = emitter_mesh.numElem();
    auto num_receiver_elements = receiver_mesh.numElem();

    //TODO FINISH THIS METHOD
  }
}