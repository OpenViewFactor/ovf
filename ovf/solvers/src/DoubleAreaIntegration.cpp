#include "DoubleAreaIntegration.hpp"

namespace openviewfactor {
  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE FLOAT_TYPE DoubleAreaIntegration<FLOAT_TYPE>::solveViewFactorBetweenElements(const Vector3<FLOAT_TYPE>& emitter_centroid, const Vector3<FLOAT_TYPE>& emitter_normal, const Triangle<FLOAT_TYPE>& receiver_element) const {
    auto ray_vector = receiver_element.getCentroid() - emitter_centroid;
    auto scale_factor = -1.0 / (ray_vector.dot(ray_vector) * ray_vector.dot(ray_vector) * OVF_PI);
    FLOAT_TYPE receiver_area;
    if (std::isnan(receiver_element.getArea())) { receiver_area = 0.0; } else { receiver_area = receiver_element.getArea(); }
    auto fij = ((emitter_normal).dot(ray_vector) * (receiver_element.getNormal()).dot(ray_vector) * receiver_area * scale_factor);
    // if (std::isnan(fij)) {
    //   return 0.0;
    // } else {
    //   return fij;
    // }
    return fij;
  }

  template class DoubleAreaIntegration<float>;
  template class DoubleAreaIntegration<double>;
}