#ifndef OVF_VIEW_FACTOR_H_
#define OVF_VIEW_FACTOR_H_

#include <vector>

#include "IndexValuePair.hpp"

enum ViewFactorState { UNLINKED, LINKED };

namespace openviewfactor {
  template <typename FLOAT_TYPE> class ViewFactor {
    private:
      ViewFactorState _state;
      std::vector<IndexValuePair<FLOAT_TYPE>> _nonzero_vf;
    public:
  };
}
#endif