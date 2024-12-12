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
  template <typename FLOAT_TYPE> class Blockers {
    private:
      std::vector<std::shared_ptr<BVH<FLOAT_TYPE>>> _blocking_bvhs;
    protected:
    public:
      OVF_HOST_DEVICE Blockers();

      OVF_HOST_DEVICE Blockers<FLOAT_TYPE>& setBlockers(std::vector<std::string> blocking_filenames);
      OVF_HOST_DEVICE Blockers<FLOAT_TYPE>& addBlocker(std::shared_ptr<BVH<FLOAT_TYPE>> blocker);
      OVF_HOST_DEVICE std::shared_ptr<BVH<FLOAT_TYPE>> getBVH(unsigned int index) const;
      OVF_HOST_DEVICE unsigned int size() const;
  };
}

#endif