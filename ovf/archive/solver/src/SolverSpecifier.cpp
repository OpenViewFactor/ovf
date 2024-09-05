#include "solver/include/SolverSpecifier.hpp"

namespace openviewfactor {
  std::ostream &operator<<(std::ostream &os, SolverSpecifier specifier) {
    os << specifier.getMethod() << " on " << specifier.getCompute();
    return os;
  }
}