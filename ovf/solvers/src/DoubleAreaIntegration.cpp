#include "DoubleAreaIntegration.hpp"

namespace openviewfactor {
  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE FLOAT_TYPE DoubleAreaIntegration<FLOAT_TYPE>::solveViewFactorBetweenElements(const Triangle<FLOAT_TYPE>& emitter_element, const Triangle<FLOAT_TYPE>& receiver_element) const {
    auto emitter_element_centroid = emitter_element.getCentroid();
    auto receiver_element_centroid = receiver_element.getCentroid();
    auto ray_vector = receiver_element_centroid - emitter_element_centroid;
    FLOAT_TYPE view_factor = (emitter_element.getNormal()).dot(ray_vector) * (receiver_element.getNormal()).dot(ray_vector) * receiver_element.getArea() / (ray_vector.dot(ray_vector) * ray_vector.dot(ray_vector) * M_PI);
    return view_factor;
  }

  template class DoubleAreaIntegration<float>;
  template class DoubleAreaIntegration<double>;
}