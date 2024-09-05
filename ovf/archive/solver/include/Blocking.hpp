#ifndef OVF_BLOCKING_H_
#define OVF_BLOCKING_H_

#include "solver/include/ViewFactor.hpp"
#include "geometry/include/Blockers.hpp"
#include <vector>

namespace openviewfactor {
  template <typename FLOAT_TYPE> class Blocking {
    protected:
      ViewFactor<FLOAT_TYPE> _vf;
      Blockers<FLOAT_TYPE> _blockers;
      std::vector<size_t> _blocked_pairs;

    public:
      OVF_HOST_DEVICE Blocking() : _vf(ViewFactor<FLOAT_TYPE>()), _blockers(Blockers<FLOAT_TYPE>()),_blocked_pairs(std::vector<size_t>()) {}

      OVF_HOST_DEVICE virtual ~Blocking() {} //! Subclassable

      OVF_HOST_DEVICE Blocking<FLOAT_TYPE>& addViewFactor(const ViewFactor<FLOAT_TYPE> &vf);
      OVF_HOST_DEVICE Blocking<FLOAT_TYPE>& addBlockers(const Blockers<FLOAT_TYPE> &b);
      OVF_HOST_DEVICE std::vector<size_t> getBlockedPairs() const;

      OVF_HOST_DEVICE virtual bool isElementPairBlocked(size_t emitter_index, size_t receiver_index) = 0;
      OVF_HOST_DEVICE virtual void checkOneBlockingMesh(const Triangulation<FLOAT_TYPE> &blocker) = 0;
      OVF_HOST_DEVICE virtual void checkAllBlockingMeshes() = 0;
  };
}

#endif