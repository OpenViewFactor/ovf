#ifndef OVF_SOLVERFACTORY_H_
#define OVF_SOLVERFACTORY_H_

#include "ovf/core/Solver.hpp"
#include "ovf/manager/SolverSpecifier.hpp"
#include <exception>
#include <memory>

#include <map>  //? shane wanted to reorganize the implementation of this class using maps
#include <tuple>

namespace openviewfactor {

template <typename FLOAT_TYPE> class SolverFactory {
  public:
    std::unique_ptr<Solver<FLOAT_TYPE>> getSolver(const std::string &method, const std::string &compute);
    std::unique_ptr<Solver<FLOAT_TYPE>> getSolver(const SovlerSpecifier &specifier);

    std::vector<SolverSpecifier> getAvailableSolvers();
    std::vector<SolverSpecifier> getSolversForMethod(const std::string &method);
};

}

#endif