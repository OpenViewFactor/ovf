#ifndef OVF_BLOCKING_H_
#define OVF_BLOCKING_H_

#include "solver/include/ViewFactor.hpp"
#include <vector>

namespace openviewfactor {
  template <typename FLOAT_TYPE> class Blocking {
    protected:
      ViewFactor<FLOAT_TYPE> _vf;
      std::vector<size_t> _blocked_pairs;

    public:
      OVF_HOST_DEVICE Blocking(const ViewFactor<FLOAT_TYPE> &vf) : _vf(vf), _blocked_pairs(std::vector<size_t>()) {}

      OVF_HOST_DEVICE virtual ~Blocking() {} //! Subclassable

      OVF_HOST_DEVICE std::vector<size_t> getBlockedPairs() const;

      OVF_HOST_DEVICE virtual bool isBlocked(size_t emitter_index, size_t receiver_index) = 0;
      OVF_HOST_DEVICE virtual void checkBlocking() = 0;
  };
}

#endif