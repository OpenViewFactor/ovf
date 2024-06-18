#include "geometry/include/Blockers.hpp"

namespace openviewfactor {
  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE Blockers<FLOAT_TYPE>::Blockers() : _blockers(std::vector<Triangulation<FLOAT_TYPE>>()) {}

  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE Blockers<FLOAT_TYPE>& Blockers<FLOAT_TYPE>::addBlocker(const Triangulation<FLOAT_TYPE> &b) {
    _blockers.push_back(b);
    return *this;
  }
}