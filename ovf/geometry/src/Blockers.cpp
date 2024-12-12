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
      std::cout << "[LOG] Loading Blocking Mesh : " << file << '\n';
      auto blocker_mesh = reader.getMesh(file);
      auto blocker_bvh = std::make_shared<BVH<FLOAT_TYPE>>();
      blocker_bvh->linkToTriangulation(blocker_mesh);
      std::cout << "[LOG] Constructing BVH for Blocker : " << file << '\n';
      blocker_bvh->constructBVH();
      std::cout << "[LOG] Construction Finished" << '\n';
      this->addBlocker(blocker_bvh);
    }
    return *this;
  }

  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE Blockers<FLOAT_TYPE>& Blockers<FLOAT_TYPE>::addBlocker(std::shared_ptr<BVH<FLOAT_TYPE>> blocker) {
    _blocking_bvhs.push_back(blocker);
    return *this;
  }

  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE std::shared_ptr<BVH<FLOAT_TYPE>> Blockers<FLOAT_TYPE>::getBVH(unsigned int index) const { return _blocking_bvhs.at(index); }

  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE unsigned int Blockers<FLOAT_TYPE>::size() const { return _blocking_bvhs.size(); }

  template class Blockers<float>;
  template class Blockers<double>;
}