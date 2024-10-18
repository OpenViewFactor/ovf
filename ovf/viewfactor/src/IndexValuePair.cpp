#include "IndexValuePair.hpp"

namespace openviewfactor {
  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE IndexValuePair<FLOAT_TYPE>::IndexValuePair()
    : _pair(std::pair<unsigned int, FLOAT_TYPE>({0, 0.0})) {}

  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE IndexValuePair<FLOAT_TYPE>::IndexValuePair(unsigned int index, FLOAT_TYPE value)
    : _pair(std::pair<unsigned int, FLOAT_TYPE>({index, value})) {}
  
  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE unsigned int IndexValuePair<FLOAT_TYPE>::getFullMatrixIndex() const { return _pair.first; }
  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE FLOAT_TYPE IndexValuePair<FLOAT_TYPE>::getValue() const { return _pair.second; }

  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE IndexValuePair<FLOAT_TYPE>& IndexValuePair<FLOAT_TYPE>::setFullMatrixIndex(unsigned int index) {
    _pair.first = index;
    return *this;
  }
  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE IndexValuePair<FLOAT_TYPE>& IndexValuePair<FLOAT_TYPE>::setValue(FLOAT_TYPE value) {
    _pair.second = value;
    return *this;
  }

  template class IndexValuePair<float>;
  template class IndexValuePair<double>;
}