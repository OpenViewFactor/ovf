#ifndef OVF_SOLVER_H_
#define OVF_SOLVER_H_

#include "solver/include/SolverOptions.hpp"
#include "solver/include/ViewFactor.hpp"

namespace openviewfactor {

template <typename FLOAT_TYPE> class Solver {
  protected:
    SolverOptions<FLOAT_TYPE> _options;
  
  public:
    Solver() : _options(SolverOptions<FLOAT_TYPE>()) {} //! uses default options
    Solver(const SolverOptions<FLOAT_TYPE> &options) : _options(options) {}

    Solver &setOptions(const SolverOptions<FLOAT_TYPE> &options) { _options = options; return *this; }

    virtual ~Solver() {} //! Subclassable

    virtual std::unique_ptr<ViewFactor<FLOAT_TYPE>> solveViewFactor(const Triangulation<FLOAT_TYPE> &mesh) = 0;
    virtual std::unique_ptr<ViewFactor<FLOAT_TYPE>> solveViewFactor(const Triangulation<FLOAT_TYPE> &emitter_mesh
                                                                    const Triangulation<FLOAT_TYPE> &receiver_mesh) = 0;
};

}

#endif