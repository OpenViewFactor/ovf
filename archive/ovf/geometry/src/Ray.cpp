#include "Ray.hpp"

namespace openviewfactor {
  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE Ray<FLOAT_TYPE>::Ray()
    : _origin(Vector3<FLOAT_TYPE>()), _direction(Vector3<FLOAT_TYPE>()), _intersection_distance(INFINITY) {}

  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE Vector3<FLOAT_TYPE> Ray<FLOAT_TYPE>::getOrigin() const { return _origin; }
  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE Vector3<FLOAT_TYPE> Ray<FLOAT_TYPE>::getDirection() const { return _direction; }
  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE FLOAT_TYPE Ray<FLOAT_TYPE>::getIntersectionDistance() const { return _intersection_distance; }

  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE Ray<FLOAT_TYPE>& Ray<FLOAT_TYPE>::setOrigin(Vector3<FLOAT_TYPE> origin) {
    _origin = origin;
    return *this;
  }
  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE Ray<FLOAT_TYPE>& Ray<FLOAT_TYPE>::setDirection(Vector3<FLOAT_TYPE> direction) {
    _direction = direction;
    return *this;
  }
  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE Ray<FLOAT_TYPE>& Ray<FLOAT_TYPE>::setIntersectionDistance(FLOAT_TYPE t) {
    _intersection_distance = t;
    return *this;
  }

  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE Ray<FLOAT_TYPE>& Ray<FLOAT_TYPE>::triangleIntersection(Triangle<FLOAT_TYPE> triangle) {
    Vector3<FLOAT_TYPE> V0 = triangle.getOA();
    Vector3<FLOAT_TYPE> E1 = triangle.getAB();
    Vector3<FLOAT_TYPE> E2 = triangle.getCA().flip();

    Vector3<FLOAT_TYPE> T = this->getOrigin() - V0;
    Vector3<FLOAT_TYPE> D = this->getDirection();
    Vector3<FLOAT_TYPE> P = D.cross(E2);
    Vector3<FLOAT_TYPE> Q = T.cross(E1);

    FLOAT_TYPE scalar_multiplier = 1 / P.dot(E1);
    FLOAT_TYPE u = P.dot(T) * scalar_multiplier;
    if (u < 0.0 || u > 1.0) {
      return *this;
    }
    FLOAT_TYPE v = Q.dot(D) * scalar_multiplier;
    if (v < 0.0 || v > 1.0) {
      return *this;
    }
   
    FLOAT_TYPE t = Q.dot(E2) * scalar_multiplier;
    if (t > 0.0 && t < _intersection_distance) { this->setIntersectionDistance(t); }

    return *this;
  }

  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE Ray<FLOAT_TYPE>& Ray<FLOAT_TYPE>::reset() {
    // _origin = Vector3<FLOAT_TYPE>();
    // _direction = Vector3<FLOAT_TYPE>();
    _intersection_distance = INFINITY;
    return *this;
  }

template class Ray<float>;
template class Ray<double>;
}