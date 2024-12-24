#include "DoubleAreaIntegration.hpp"

namespace openviewfactor {
  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE FLOAT_TYPE DoubleAreaIntegration<FLOAT_TYPE>::solveViewFactorBetweenElements(const Vector3<FLOAT_TYPE>& emitter_centroid, const Vector3<FLOAT_TYPE>& emitter_normal, const Vector3<FLOAT_TYPE>& receiver_centroid, const Vector3<FLOAT_TYPE>& receiver_normal, FLOAT_TYPE receiver_area) const {
    auto ray_vector = receiver_centroid - emitter_centroid;
    FLOAT_TYPE view_factor = (emitter_normal).dot(ray_vector) * (receiver_normal).dot(ray_vector) * receiver_area / (ray_vector.dot(ray_vector) * ray_vector.dot(ray_vector) * M_PI);
    return view_factor;
  }

  template class DoubleAreaIntegration<float>;
  template class DoubleAreaIntegration<double>;
}