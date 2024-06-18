#ifndef OVF_BFCULL_H_
#define OVF_BFCULL_H_

#include "solver/include/ViewFactor.hpp"
#include <vector>

namespace openviewfactor {
  template <typename FLOAT_TYPE> class BFCull {
    private:
      ViewFactor<FLOAT_TYPE> _vf;
      std::vector<size_t> _culled_pairs;

    public:
      OVF_HOST_DEVICE BFCull(const ViewFactor<FLOAT_TYPE> &vf);

      OVF_HOST_DEVICE std::vector<size_t> getCulledPairs() const;

      OVF_HOST_DEVICE bool isCulled(size_t emitter_index, size_t receiver_index) const;
      OVF_HOST_DEVICE void checkCulling();
  };
}

#endif