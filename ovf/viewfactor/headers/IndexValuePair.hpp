#ifndef OVF_INDEX_VALUE_PAIR_H_
#define OVF_INDEX_VALUE_PAIR_H_

#include <utility>

#include "../../config.hpp"

namespace openviewfactor {
  template <typename FLOAT_TYPE> class IndexValuePair {
    private:
      std::pair<unsigned int, FLOAT_TYPE> _pair;
    protected:
    public:
      OVF_HOST_DEVICE IndexValuePair();
      OVF_HOST_DEVICE IndexValuePair(unsigned int index, FLOAT_TYPE value);

      OVF_HOST_DEVICE unsigned int getFullMatrixIndex() const;
      OVF_HOST_DEVICE FLOAT_TYPE getValue() const;

      OVF_HOST_DEVICE IndexValuePair<FLOAT_TYPE>& setFullMatrixIndex(unsigned int index);
      OVF_HOST_DEVICE IndexValuePair<FLOAT_TYPE>& setValue(FLOAT_TYPE value);
  };
}

#endif