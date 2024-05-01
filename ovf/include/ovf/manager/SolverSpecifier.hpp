#ifndef OVF_SOLVERSPECIFIER_H_
#define OVF_SOLVERSPECIFIER_H_

#include "ovf/core/Solver.hpp"
#include <ostream>

namespace openviewfactor {

class SolverSpecifier {
  private:
    std::string _method;
    std::string _compute;

  public:
    SolverSpecifier(const std::string &method, const std::string &compute)
      : _method(method), _compute(compute) {}
    
    std::string getMethod() const { return _method; }
    std::string getCompute() const { return _compute; }

    SolverSpecifier& setCompute(const std::string &compute) { _compute = compute; return *this; }
    SolverSpecifier& setMethod(const std::string &method) { _method = method; return *this; }
};

std::ostream &operator<<(std::ostream &os, SolverSpecifier specifier);

}

#endif