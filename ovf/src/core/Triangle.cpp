#include "ovf/core/Triangle.hpp"
#include <cmath>

namespace openviewfactor {
  //* ----- CLASS CONSTRUCTORS ----- *//
  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE Triangle<FLOAT_TYPE>::Triangle()
    : _OA(Vector3()), _OB(Vector3()), _OC(Vector3()), _n(Vector3()), _a(0) {}

  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE Triangle<FLOAT_TYPE>::Triangle(Vector3<FLOAT_TYPE> OA, Vector3<FLOAT_TYPE> OB, Vector3<FLOAT_TYPE> OC)
    : _OA(OA), _OB(OB), _OC(OC), _n(cross(_OB - _OA, _OC - _OA)), _a(_n.getMagnitude() / 2) {}
  
  //* ----- ACCESSOR METHODS ----- *//
  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE const Vector3<FLOAT_TYPE>& Triangle<FLOAT_TYPE>::getOA() const { return *_OA; }
  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE const Vector3<FLOAT_TYPE>& Triangle<FLOAT_TYPE>::getOB() const { return *_OB; }
  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE const Vector3<FLOAT_TYPE>& Triangle<FLOAT_TYPE>::getOC() const { return *_OC; }

  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE const Vector3<FLOAT_TYPE> Triangle<FLOAT_TYPE>::getAB() const { return _OB - _OA; }
  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE const Vector3<FLOAT_TYPE> Triangle<FLOAT_TYPE>::getBC() const { return _OC - _OB; }
  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE const Vector3<FLOAT_TYPE> Triangle<FLOAT_TYPE>::getCA() const { return _OA - _OC; }

  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE const Vector3<FLOAT_TYPE>& Triangle<FLOAT_TYPE>::normal() const { return *_n; };
  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE const Vector3<FLOAT_TYPE> Triangle<FLOAT_TYPE>::centroid() const { 
    return (_OA + _OB + _OC).scale(1/3);
  }
  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE const FLOAT_TYPE Triangle<FLOAT_TYPE>::area() const { return _a; }

  //* ----- MUTATOR METHODS ----- *//
  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE Triangle<FLOAT_TYPE>& Triangle<FLOAT_TYPE>::setOA(FLOAT_TYPE x, FLOAT_TYPE y, FLOAT_TYPE z) {
    _OA.setX(x).setY(y).setZ(z);
    return *this;
  }
  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE Triangle<FLOAT_TYPE>& Triangle<FLOAT_TYPE>::setOA(Vector3<FLOAT_TYPE> v) {
    _OA.setX(v.getX()).setY(v.getY()).setZ(v.getZ());
    return *this;
  }
  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE Triangle<FLOAT_TYPE>& Triangle<FLOAT_TYPE>::setOB(FLOAT_TYPE x, FLOAT_TYPE y, FLOAT_TYPE z) {
    _OB.setX(x).setY(y).setZ(z);
    return *this;
  }
  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE Triangle<FLOAT_TYPE>& Triangle<FLOAT_TYPE>::setOB(Vector3<FLOAT_TYPE> v) {
    _OB.setX(v.getX()).setY(v.getY()).setZ(v.getZ());
    return *this;
  }
  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE Triangle<FLOAT_TYPE>& Triangle<FLOAT_TYPE>::setOC(FLOAT_TYPE x, FLOAT_TYPE y, FLOAT_TYPE z) {
    _OC.setX(x).setY(y).setZ(z);
    return *this;
  }
  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE Triangle<FLOAT_TYPE>& Triangle<FLOAT_TYPE>::setOC(Vector3<FLOAT_TYPE> v) {
    _OC.setX(v.getX()).setY(v.getY()).setZ(v.getZ());
    return *this;
  }

  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE bool Triangle<FLOAT_TYPE>::operator==(const Triangle<FLOAT_TYPE> &rhs) const {
    return ((_OA == rhs.getOA() && _OB == rhs.getOB() && _OC == rhs.getOC()) ||
            (_OB == rhs.getOA() && _OC == rhs.getOB() && _OA == rhs.getOC()) ||
            (_OC == rhs.getOA() && _OA == rhs.getOB() && _OB == rhs.getOC()));
  }
  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE bool Triangle<FLOAT_TYPE>::operator!=(const Triangle<FLOAT_TYPE> &rhs) const {
    return !(this == rhs);
  }

template class Triangle<float>;
template class Triangle<double>;
template bool operator==(const Triangle<float> &, const Triangle<float> &);
template bool operator==(const Triangle<double> &, const Triangle<double> &);
template bool operator!=(const Triangle<float> &, const Triangle<float> &);
template bool operator!=(const Triangle<double> &, const Triangle<double> &);
}