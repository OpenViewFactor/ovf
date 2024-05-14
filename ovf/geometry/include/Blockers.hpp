#ifndef OVF_BLOCKERS_H_
#define OVF_BLOCKERS_H_

#include "geometry/include/Triangulation.hpp"
#include <vector>

namespace openviewfactor {
  template <typename FLOAT_TYPE> class Blockers {
    private:
      std::vector<Triangulation<FLOAT_TYPE>> _blockers;

    public:
      OVF_HOST_DEVICE Blockers();

      OVF_HOST_DEVICE Blockers<FLOAT_TYPE>& addBlocker(const Triangulation<FLOAT_TYPE> &b);
  };
}

#endif