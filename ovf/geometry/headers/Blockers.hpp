#ifndef _OVF_BLOCKERS_H_
#define _OVF_BLOCKERS_H_

#include "../../config.hpp"
#include "Vector3.hpp"
#include "Triangulation.hpp"
#include "BVHNode.hpp"
#include "BVH.hpp"
#include "STLReader.hpp"
#include <memory>
#include <vector>
#include <iostream>

namespace openviewfactor {
  template <typename t> class Blockers {
    private:
      std::vector<std::shared_ptr<BVH<t>>> _blocking_bvhs;
    protected:
    public:
      gpuify Blockers();

      gpuify Blockers<t>& setBlockers(std::vector<std::string> blocking_filenames);
      gpuify Blockers<t>& addBlocker(std::shared_ptr<BVH<t>> blocker);
      gpuify std::shared_ptr<BVH<t>> getBVH(unsigned int index) const;
      gpuify unsigned int size() const;
  };
}

#endif