#include "solver/include/Blocking.hpp"

namespace openviewfactor {
  template <typename FLOAT_TYPE> class MollerTrumbore : protected Blocking {
    public:
      OVF_HOST_DEVICE MollerTrumbore() : _vf(ViewFactor<FLOAT_TYPE>()), _blockers(Blockers<FLOAT_TYPE>()),_blocked_pairs(std::vector<size_t>()) {}

      OVF_HOST_DEVICE bool isElementPairBlocked(size_t emitter_index, size_t receiver_index) override;
      OVF_HOST_DEVICE void checkOneBlockingMesh(const Triangulation<FLOAT_TYPE> &blocker) override;
      OVF_HOST_DEVICE void checkAllBlockingMeshes() override;
  };
}