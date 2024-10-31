#ifndef _OVF_SOLVER_H_
#define _OVF_SOLVER_H_

#include "../../config.hpp"
#include "Ray.hpp"
#include "Triangulation.hpp"
#include "BVH.hpp"
#include "SolverOptions.hpp"

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

      virtual std::vector<unsigned int> backFaceCull(std::shared_ptr<Triangulation<FLOAT_TYPE>> emitter_mesh, std::shared_ptr<Triangulation<FLOAT_TYPE>> receiver_mesh) const = 0;

      virtual std::vector<unsigned int> evaluateBlocking(std::shared_ptr<Triangulation<FLOAT_TYPE>> emitter_mesh, std::shared_ptr<Triangulation<FLOAT_TYPE>> receiver_mesh, Blockers<FLOAT_TYPE> blockers) = 0;

      virtual std::unique_ptr<ViewFactor<FLOAT_TYPE>> solveViewFactor(std::shared_ptr<Triangulation<FLOAT_TYPE>> emitter_mesh, std::shared_ptr<Triangulation<FLOAT_TYPE>> receiver_mesh) = 0;
  };
}

#endif