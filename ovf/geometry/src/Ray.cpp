#include "geometry/include/Ray.hpp"

namespace openviewfactor {
  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE Ray<FLOAT_TYPE>::Ray()
    : _origin(Vector3<FLOAT_TYPE>()), _dir(Vector3<FLOAT_TYPE>()), _intersection_distance(0);

  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE V3 Ray<FLOAT_TYPE>::getOrigin() const { return _origin; }
  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE V3 Ray<FLOAT_TYPE>::getDirection() const { return _dir; }
  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE double Ray<FLOAT_TYPE>::getIntersectionDistance() const { return _intersection_distance; }
  
  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE Ray& Ray<FLOAT_TYPE>::setOrigin(const Vector3<FLOAT_TYPE> &origin) { _origin = origin; return *this; }
  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE Ray& Ray<FLOAT_TYPE>::setDirection(const Vector3<FLOAT_TYPE> &dir) { _dir = dir; return *this; }
  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE Ray& Ray<FLOAT_TYPE>::setIntersectionDistance(const FLOAT_TYPE &intersection_distance) { _intersection_distance = intersection_distance; return *this; }

  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE bool Ray<FLOAT_TYPE>::backFaceCull(const Triangle<FLOAT_TYPE> &emitter, const Triangle<FLOAT_TYPE> &receiver) {
    //TODO write this method
  }
  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE void Ray<FLOAT_TYPE>::triangleIntersection(const Triangle<FLOAT_TYPE> &triangle) {
    Vector3<FLOAT_TYPE> V0 = triangle.getOA();
    Vector3<FLOAT_TYPE> E1 = triangle.getAB();
    Vector3<FLOAT_TYPE> E2 = triangle.getCA().flip;

    Vector3<FLOAT_TYPE> T = _origin - V0;
    Vector3<FLOAT_TYPE> P = _dir.cross(E2);
    Vector3<FLOAT_TYPE> Q = T.cross(E1);

    t = Q.dot(E2) * (1 / P.dot(E1));
    if (t > 0 && t < _intersection_distance) {
      setIntersectionDistance(t);
    }
  }
}