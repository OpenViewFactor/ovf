#include "DoubleAreaIntegration.hpp"

namespace openviewfactor {
  template <typename t>
  gpuify t DoubleAreaIntegration<t>::solveViewFactorBetweenElements(const Vector3<t>& emitter_centroid, const Vector3<t>& emitter_normal, const Triangle<t>& receiver_element) const {
    auto ray_vector = receiver_element.getCentroid() - emitter_centroid;
    auto scale_factor = -1.0 / (ray_vector.dot(ray_vector) * ray_vector.dot(ray_vector) * OVF_PI);
    auto fij = ((emitter_normal).dot(ray_vector) * (receiver_element.getNormal()).dot(ray_vector) * receiver_element.getArea() * scale_factor);
    if (std::isnan(fij)) {
      return 0.0;
    } else {
      return fij;
    }
  }

  template class DoubleAreaIntegration<float>;
  template class DoubleAreaIntegration<double>;
}