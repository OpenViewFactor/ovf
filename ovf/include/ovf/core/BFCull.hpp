#ifndef OVF_BFCULL_H_
#define OVF_BFCULL_H_

#include "ovf/core/ViewFactor.hpp"
#include <vector>

namespace openviewfactor {
  template <typename FLOAT_TYPE> class BFCull {
    private:
      ViewFactor<FLOAT_TYPE> _vf;
      std::vector<size_t> _culled_pairs;

    public:
      OVF_HOST_DEVICE BFCull(ViewFactor<FLOAT_TYPE> vf);

      OVF_HOST_DEVICE std::vector<size_t> getCulledPairs() const;

      OVF_HOST_DEVICE bool isCulled(const Triangle<FLOAT_TYPE> &emitter, const Triangle<FLOAT_TYPE> &receiver) const;
      OVF_HOST_DEVICE void checkCulling();
  };
}

#endif