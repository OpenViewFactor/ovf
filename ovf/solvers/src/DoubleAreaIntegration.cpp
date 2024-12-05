#include "DoubleAreaIntegration.hpp"

namespace openviewfactor {
  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE std::unique_ptr<ViewFactor<FLOAT_TYPE>> DoubleAreaIntegration<FLOAT_TYPE>::solveViewFactor(std::shared_ptr<Triangulation<FLOAT_TYPE>> emitter_mesh, std::shared_ptr<Triangulation<FLOAT_TYPE>> receiver_mesh, std::vector<unsigned int> unblocked_indices) const {
    auto num_emitter_elements = emitter_mesh->getNumElements();
    auto num_receiver_elements = receiver_mesh->getNumElements();
    auto results = std::make_unique<ViewFactor<FLOAT_TYPE>>();
    for (auto index : unblocked_indices) {
      auto emitter_index = index / num_receiver_elements;
      auto receiver_index = index % num_receiver_elements;
      auto emitter_element = (*emitter_mesh)[emitter_index];
      auto receiver_element = (*receiver_mesh)[receiver_index];
      auto emitter_element_centroid = emitter_element.getCentroid();
      auto receiver_element_centroid = receiver_element.getCentroid();
      auto ray_vector = emitter_element_centroid - receiver_element_centroid;
      FLOAT_TYPE view_factor = (emitter_element.getNormal()).dot(ray_vector) * (receiver_element.getNormal()).dot(ray_vector) * receiver_element.getArea() / (ray_vector.dot(ray_vector) * M_PI);
      results->setElement(index, view_factor);
    }
    return results;
  }

  template class DoubleAreaIntegration<float>;
  template class DoubleAreaIntegration<double>;
}