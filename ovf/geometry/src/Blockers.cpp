#include "Blockers.hpp"

namespace openviewfactor {
  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE Blockers<FLOAT_TYPE>::Blockers()
    : _blocking_bvhs(std::vector<std::shared_ptr<BVH<FLOAT_TYPE>>>()) {}

  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE Blockers<FLOAT_TYPE>& Blockers<FLOAT_TYPE>::setBlockers(std::vector<std::string> blocking_filenames) {
    STLReader<FLOAT_TYPE> reader;
    //TODO This loop should probably get parallelized
    for (auto file : blocking_filenames) {
      auto blocker_mesh = reader.getMesh(file);
      auto blocker_bvh = std::make_shared<BVH<FLOAT_TYPE>>();
      blocker_bvh->linkToTriangulation(blocker_mesh);
      blocker_bvh->constructBVH();
      _blocking_bvhs.push_back(blocker_bvh);
    }
    return *this;
  }

  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE std::shared_ptr<BVH<FLOAT_TYPE>> Blockers<FLOAT_TYPE>::getBVH(unsigned int index) const {
    return _blocking_bvhs[index];
  }

  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE typename std::vector<std::shared_ptr<BVH<FLOAT_TYPE>>>::iterator Blockers<FLOAT_TYPE>::begin() {
    return _blocking_bvhs.begin();
  }

  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE typename std::vector<std::shared_ptr<BVH<FLOAT_TYPE>>>::iterator Blockers<FLOAT_TYPE>::end() {
    return _blocking_bvhs.end();
  }

  template class Blockers<float>;
  template class Blockers<double>;
}