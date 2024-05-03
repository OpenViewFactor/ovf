#include "solver/include/BFCull.hpp"

namespace openviewfactor {
  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE BFCull<FLOAT_TYPE>::BFCull(ViewFactor<FLOAT_TYPE> vf) : _vf(vf), _culled_pairs(std::vector<size_t>()) {}

  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE std::vector<size_t> BFCull<FLOAT_TYPE>::getCulledPairs() const { return _culled_pairs; }

  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE bool BFCull<FLOAT_TYPE>::isCulled(const Triangle<FLOAT_TYPE> &emitter, const Triangle<FLOAT_TYPE> &receiver) const {
    Vector3<FLOAT_TYPE> centroid_centroid_ray = receiver.centroid() - emitter.centroid();
    return (centroid_centroid_ray.dot(emitter.normal()) < 0 || centroid_centroid_ray.dot(receiver.normal()) > 0);
  }

  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE void BFCull<FLOAT_TYPE>::checkCulling() {
    Triangulation<FLOAT_TYPE> emitter = _vf.getEmitterMesh();
    Triangulation<FLOAT_TYPE> receiver = _vf.getReceiverMesh();

    auto num_emitter_elements = emitter.numElem();
    auto num_receiver_elements = receiver.numElem();

    for (size_t emitter_index = 0; emitter_index < num_emitter_elements; emitter_index++) {
      for (size_t receiver_index = 0; receiver_index < num_receiver_elements; receiver_index++) {
        if (isCulled(emitter[emitter_index], receiver[receiver_index])) {
          _culled_pairs.push_back((emitter_index * num_receiver_elements) + receiver_index);
        }
      }
    }
  }
}