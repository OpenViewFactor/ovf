#ifndef OVF_RAY_H_
#define OVF_RAY_H_

#include "geometry/include/Vector3.hpp"
#include "geometry/include/Triangle.hpp"

namespace openviewfactor {

template <typename FLOAT_TYPE> class Ray {
  using V3 = Vector3<FLOAT_TYPE>;

  private:
    V3 _origin;
    V3 _dir;
    double _intersection_distance;
  
  public:
    OVF_HOST_DEVICE Ray();

    OVF_HOST_DEVICE V3 getOrigin() const;
    OVF_HOST_DEVICE V3 getDirection() const;
    OVF_HOST_DEVICE double getIntersectionDistance() const;

    OVF_HOST_DEVICE Ray& setOrigin(const V3 &origin);
    OVF_HOST_DEVICE Ray& setDirection(const V3 &dir);
    OVF_HOST_DEVICE Ray& setIntersectionDistance(const FLOAT_TYPE &intersection_distance);

    OVF_HOST_DEVICE bool backFaceCull(const T &emitter, const T &receiver); //? unclear whether this method is necessary
    OVF_HOST_DEVICE void triangleIntersection(const T &triangle);
};
}

#endif