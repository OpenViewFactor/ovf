#include "DoubleAreaIntegration.hpp"

namespace openviewfactor {
  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE FLOAT_TYPE DoubleAreaIntegration<FLOAT_TYPE>::solveViewFactorBetweenElements(const Vector3<FLOAT_TYPE>& emitter_centroid, const Vector3<FLOAT_TYPE>& emitter_normal, const Triangle<FLOAT_TYPE>& receiver_element) const {
    auto ray_vector = receiver_element.getCentroid() - emitter_centroid;
    auto scale_factor = 1 / (ray_vector.dot(ray_vector) * ray_vector.dot(ray_vector) * OVF_PI);
    if (!std::isnan(scale_factor)) {
      return ((emitter_normal).dot(ray_vector) * (receiver_element.getNormal()).dot(ray_vector) * receiver_element.getArea() * scale_factor);
    } else {
      return 0.0;
    }
  }

  template class DoubleAreaIntegration<float>;
  template class DoubleAreaIntegration<double>;
}