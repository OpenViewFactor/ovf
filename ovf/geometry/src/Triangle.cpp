#include "Vector3.hpp"
#include "Triangle.hpp"

#include <iostream>

namespace openviewfactor {
  //* ----- CLASS CONSTRUCTORS ----- *//
  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE Triangle<FLOAT_TYPE>::Triangle()
    : _pts(std::array<Vector3<FLOAT_TYPE>,3>({Vector3<FLOAT_TYPE>(), Vector3<FLOAT_TYPE>(), Vector3<FLOAT_TYPE>()})) {}

  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE Triangle<FLOAT_TYPE>::Triangle(std::array<Vector3<FLOAT_TYPE>,3> pts)
    : _pts(pts) {}

  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE Triangle<FLOAT_TYPE>::Triangle(Vector3<FLOAT_TYPE> OA, Vector3<FLOAT_TYPE> OB, Vector3<FLOAT_TYPE> OC)
    : _pts(std::array<Vector3<FLOAT_TYPE>,3>({OA,OB,OC})) {}
  
  //* ----- ACCESSOR METHODS ----- *//
  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE const Vector3<FLOAT_TYPE> Triangle<FLOAT_TYPE>::getOA() const { return _pts[0]; }
  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE const Vector3<FLOAT_TYPE> Triangle<FLOAT_TYPE>::getOB() const { return _pts[1]; }
  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE const Vector3<FLOAT_TYPE> Triangle<FLOAT_TYPE>::getOC() const { return _pts[2]; }

  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE const Vector3<FLOAT_TYPE> Triangle<FLOAT_TYPE>::getAB() const { return _pts[1] - _pts[0]; }
  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE const Vector3<FLOAT_TYPE> Triangle<FLOAT_TYPE>::getBC() const { return _pts[2] - _pts[1]; }
  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE const Vector3<FLOAT_TYPE> Triangle<FLOAT_TYPE>::getCA() const { return _pts[0] - _pts[2]; }

  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE const Vector3<FLOAT_TYPE> Triangle<FLOAT_TYPE>::getNormal() const {
    Vector3<FLOAT_TYPE> unnormalized_normal = cross(_pts[1]-_pts[0], _pts[2]-_pts[0]);
    return unnormalized_normal.scale(1/unnormalized_normal.getMagnitude());
  }
  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE const Vector3<FLOAT_TYPE> Triangle<FLOAT_TYPE>::getCentroid() const {
    Vector3<FLOAT_TYPE> centroid = (_pts[0]+_pts[1]+_pts[2]).scale(1.0/3.0);
    return centroid;
  }
  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE const FLOAT_TYPE Triangle<FLOAT_TYPE>::getArea() const {
    Vector3<FLOAT_TYPE> unnormalized_normal = cross(_pts[1]-_pts[0], _pts[2]-_pts[0]);
    return (unnormalized_normal.getMagnitude() / 2);
  }

  //* ----- MUTATOR METHODS ----- *//
  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE Triangle<FLOAT_TYPE>& Triangle<FLOAT_TYPE>::setOA(FLOAT_TYPE x, FLOAT_TYPE y, FLOAT_TYPE z) {
    _pts[0].setX(x).setY(y).setZ(z);
    return *this;
  }
  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE Triangle<FLOAT_TYPE>& Triangle<FLOAT_TYPE>::setOA(Vector3<FLOAT_TYPE> v) {
    _pts[0] = v;
    return *this;
  }
  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE Triangle<FLOAT_TYPE>& Triangle<FLOAT_TYPE>::setOB(FLOAT_TYPE x, FLOAT_TYPE y, FLOAT_TYPE z) {
    _pts[1].setX(x).setY(y).setZ(z);
    return *this;
  }
  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE Triangle<FLOAT_TYPE>& Triangle<FLOAT_TYPE>::setOB(Vector3<FLOAT_TYPE> v) {
    _pts[1] = v;
    return *this;
  }
  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE Triangle<FLOAT_TYPE>& Triangle<FLOAT_TYPE>::setOC(FLOAT_TYPE x, FLOAT_TYPE y, FLOAT_TYPE z) {
    _pts[2].setX(x).setY(y).setZ(z);
    return *this;
  }
  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE Triangle<FLOAT_TYPE>& Triangle<FLOAT_TYPE>::setOC(Vector3<FLOAT_TYPE> v) {
    _pts[2] = v;
    return *this;
  }

  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE bool Triangle<FLOAT_TYPE>::operator==(const Triangle<FLOAT_TYPE> &rhs) const {
    return ((_pts[0] == rhs.getOA() && _pts[1] == rhs.getOB() && _pts[2] == rhs.getOC()) ||
            (_pts[1] == rhs.getOA() && _pts[2] == rhs.getOB() && _pts[0] == rhs.getOC()) ||
            (_pts[2] == rhs.getOA() && _pts[0] == rhs.getOB() && _pts[1] == rhs.getOC()));
  }
  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE bool Triangle<FLOAT_TYPE>::operator!=(const Triangle<FLOAT_TYPE> &rhs) const {
    return !(*this == rhs);
  }

  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE bool operator==(const Triangle<FLOAT_TYPE> &lhs, const Triangle<FLOAT_TYPE> &rhs) {
    return (lhs.getOA() == rhs.getOA() && lhs.getOB() == rhs.getOB() && lhs.getOC() == rhs.getOC());
  }
  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE bool operator!=(const Triangle<FLOAT_TYPE> &lhs, const Triangle<FLOAT_TYPE> &rhs) {
    return !(lhs == rhs);
  }

template class Triangle<float>;
template class Triangle<double>;
template bool operator==(const Triangle<float> &, const Triangle<float> &);
template bool operator==(const Triangle<double> &, const Triangle<double> &);
template bool operator!=(const Triangle<float> &, const Triangle<float> &);
template bool operator!=(const Triangle<double> &, const Triangle<double> &);
}