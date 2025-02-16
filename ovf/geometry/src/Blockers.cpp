#include "Blockers.hpp"

namespace openviewfactor {
  template <typename t>
  gpuify Blockers<t>::Blockers()
    : _blocking_bvhs(std::vector<std::shared_ptr<BVH<t>>>()) {}

  template <typename t>
  gpuify Blockers<t>& Blockers<t>::setBlockers(std::vector<std::string> blocking_filenames) {
    STLReader<t> reader;
    //TODO This loop should probably get parallelized
    for (auto file : blocking_filenames) {
      std::cout << "[LOG] Loading Blocking Mesh : " << file << '\n';
      auto blocker_mesh = reader.getMesh(file);
      auto blocker_bvh = std::make_shared<BVH<t>>();
      blocker_bvh->linkToTriangulation(blocker_mesh);
      std::cout << "[LOG] Constructing BVH for Blocker : " << file << '\n';
      blocker_bvh->constructBVH();
      std::cout << "[LOG] Construction Finished" << '\n';
      this->addBlocker(blocker_bvh);
    }
    return *this;
  }

  template <typename t>
  gpuify Blockers<t>& Blockers<t>::addBlocker(std::shared_ptr<BVH<t>> blocker) {
    _blocking_bvhs.push_back(blocker);
    return *this;
  }

  template <typename t>
  gpuify std::shared_ptr<BVH<t>> Blockers<t>::getBVH(unsigned int index) const { return _blocking_bvhs.at(index); }

  template <typename t>
  gpuify unsigned int Blockers<t>::size() const { return _blocking_bvhs.size(); }

  template class Blockers<float>;
  template class Blockers<double>;
}