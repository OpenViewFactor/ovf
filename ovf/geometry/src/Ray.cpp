#include "Ray.hpp"

namespace openviewfactor {
  template <typename t>
  gpuify Ray<t>::Ray()
    : _origin(Vector3<t>()), _direction(Vector3<t>()), _intersection_distance(INFINITY) {}

  template <typename t>
  gpuify Vector3<t> Ray<t>::getOrigin() const { return _origin; }
  template <typename t>
  gpuify Vector3<t> Ray<t>::getDirection() const { return _direction; }
  template <typename t>
  gpuify t Ray<t>::getIntersectionDistance() const { return _intersection_distance; }

  template <typename t>
  gpuify Ray<t>& Ray<t>::setOrigin(Vector3<t> origin) {
    _origin = origin;
    return *this;
  }
  template <typename t>
  gpuify Ray<t>& Ray<t>::setDirection(Vector3<t> direction) {
    _direction = direction;
    return *this;
  }
  template <typename t>
  gpuify Ray<t>& Ray<t>::setIntersectionDistance(t t) {
    _intersection_distance = t;
    return *this;
  }

  template <typename t>
  gpuify Ray<t>& Ray<t>::triangleIntersection(Triangle<t> triangle) {
    Vector3<t> V0 = triangle.getOA();
    Vector3<t> E1 = triangle.getAB();
    Vector3<t> E2 = triangle.getCA().flip();

    Vector3<t> T = this->getOrigin() - V0;
    Vector3<t> D = this->getDirection();
    Vector3<t> P = D.cross(E2);
    Vector3<t> Q = T.cross(E1);

    t scalar_multiplier = 1 / P.dot(E1);
    t u = P.dot(T) * scalar_multiplier;
    if (u < 0.0 || u > 1.0) {
      return *this;
    }
    t v = Q.dot(D) * scalar_multiplier;
    if (v < 0.0 || v > 1.0) {
      return *this;
    }
   
    t t = Q.dot(E2) * scalar_multiplier;
    if (t > 0.0 && t < _intersection_distance) { this->setIntersectionDistance(t); }

    return *this;
  }

template class Ray<float>;
template class Ray<double>;
}