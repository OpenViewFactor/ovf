#ifndef OVF_RAY_H_
#define OVF_RAY_H_

#include "../../config.hpp"
#include "Vector3.hpp"
#include "Triangle.hpp"
#include <cmath>
#include <memory>
#include <iostream>

namespace openviewfactor {

template <typename t> class Ray {
  private:
    Vector3<t> _origin;
    Vector3<t> _direction;
    t _intersection_distance;
  protected:
  public:
    gpuify Ray();

    gpuify Vector3<t> getOrigin() const;
    gpuify Vector3<t> getDirection() const;
    gpuify t getIntersectionDistance() const;

    gpuify Ray<t>& setOrigin(Vector3<t> origin);
    gpuify Ray<t>& setDirection(Vector3<t> direction);
    gpuify Ray<t>& setIntersectionDistance(t t);

    gpuify Ray<t>& triangleIntersection(Triangle<t> triangle);
};

}

#endif