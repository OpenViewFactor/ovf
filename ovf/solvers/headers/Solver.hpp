#ifndef _OVF_SOLVER_H_
#define _OVF_SOLVER_H_

#include "../../config.hpp"
#include "Ray.hpp"
#include "Triangulation.hpp"
#include "BVH.hpp"
#include "Blockers.hpp"
#include "SolverOptions.hpp"
#include "ViewFactor.hpp"

namespace openviewfactor {
  template <typename FLOAT_TYPE> class Solver {
    private:
    protected:
      SolverOptions _options;
    public:
      OVF_HOST_DEVICE Solver()
        : _options(SolverOptions()) {}
      OVF_HOST_DEVICE Solver(SolverOptions options)
        : _options(options) {}
      
      OVF_HOST_DEVICE Solver& setOptions(const SolverOptions &options) { _options = options; return *this; }

      virtual ~Solver() {}

      //* ------------------------------ COMMON METHODS ------------------------------ *//

      OVF_HOST_DEVICE bool backFaceCullElements(const Triangle<FLOAT_TYPE>& emitter_element, const Triangle<FLOAT_TYPE>& receiver_element) const;
      OVF_HOST_DEVICE bool backFaceCullElements(std::shared_ptr<Triangulation<FLOAT_TYPE>> emitter_mesh, std::shared_ptr<Triangulation<FLOAT_TYPE>> receiver_mesh, unsigned int emitter_index, unsigned int receiver_index) const;

      OVF_HOST_DEVICE std::vector<unsigned int> backFaceCullMeshes(std::shared_ptr<Triangulation<FLOAT_TYPE>> emitter_mesh, std::shared_ptr<Triangulation<FLOAT_TYPE>> receiver_mesh) const;

      OVF_HOST_DEVICE bool evaluateBlockingBetweenElements(std::shared_ptr<Triangulation<FLOAT_TYPE>> emitter_mesh, std::shared_ptr<Triangulation<FLOAT_TYPE>> receiver_mesh, unsigned int emitter_index, unsigned int receiver_index, Blockers<FLOAT_TYPE> blockers) const;

      OVF_HOST_DEVICE std::vector<unsigned int> evaluateBlockingBetweenMeshes(std::shared_ptr<Triangulation<FLOAT_TYPE>> emitter_mesh, std::shared_ptr<Triangulation<FLOAT_TYPE>> receiver_mesh, Blockers<FLOAT_TYPE> blockers) const;

      OVF_HOST_DEVICE std::unique_ptr<ViewFactor<FLOAT_TYPE>> solveViewFactorBetweenMeshes(std::shared_ptr<Triangulation<FLOAT_TYPE>> emitter_mesh, std::shared_ptr<Triangulation<FLOAT_TYPE>> receiver_mesh, std::vector<unsigned int> unblocked_indices) const;

      //* ------------------------------ VIRTUAL METHODS ------------------------------ *//

      virtual OVF_HOST_DEVICE FLOAT_TYPE solveViewFactorBetweenElements(const Triangle<FLOAT_TYPE>& emitter_element, const Triangle<FLOAT_TYPE>& receiver_element) const = 0;
  };
}

#endif