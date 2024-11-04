#ifndef OVF_RAY_H_
#define OVF_RAY_H_

#include "../../config.hpp"
#include "Vector3.hpp"
#include "Triangle.hpp"
#include <cmath>
#include <memory>

namespace openviewfactor {

template <typename FLOAT_TYPE> class Ray {
  private:
    Vector3<FLOAT_TYPE> _origin;
    Vector3<FLOAT_TYPE> _direction;
    FLOAT_TYPE _intersection_distance;
  protected:
  public:
    OVF_HOST_DEVICE Ray();

    OVF_HOST_DEVICE Vector3<FLOAT_TYPE> getOrigin() const;
    OVF_HOST_DEVICE Vector3<FLOAT_TYPE> getDirection() const;
    OVF_HOST_DEVICE FLOAT_TYPE getIntersectionDistance() const;

    OVF_HOST_DEVICE Ray<FLOAT_TYPE>& setOrigin(Vector3<FLOAT_TYPE> origin);
    OVF_HOST_DEVICE Ray<FLOAT_TYPE>& setDirection(Vector3<FLOAT_TYPE> direction);
    OVF_HOST_DEVICE Ray<FLOAT_TYPE>& setIntersectionDistance(FLOAT_TYPE t);

    OVF_HOST_DEVICE bool backFaceCull(Triangle<FLOAT_TYPE> emitter, Triangle<FLOAT_TYPE> receiver) const;
    OVF_HOST_DEVICE Ray<FLOAT_TYPE>& triangleIntersection(Triangle<FLOAT_TYPE> triangle);
};

}

#endif