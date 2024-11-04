#include "DoubleAreaIntegration.hpp"

namespace openviewfactor {
  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE std::vector<unsigned int> DoubleAreaIntegration<FLOAT_TYPE>::backFaceCull(std::shared_ptr<Triangulation<FLOAT_TYPE>> emitter_mesh, std::shared_ptr<Triangulation<FLOAT_TYPE>> receiver_mesh) const {
    auto num_emitter_elements = emitter_mesh->getNumElements();
    auto num_receiver_elements = receiver_mesh->getNumElements();
    std::vector<unsigned int> unculled_indices(num_emitter_elements * num_receiver_elements - 1);
    for (unsigned int emitter_index = 0; emitter_index < num_emitter_elements; emitter_index++) {
      for (unsigned int receiver_index = 0; receiver_index < num_receiver_elements; receiver_index++) {
        auto emitter_element = (*emitter_mesh)[emitter_index];
        auto receiver_element = (*receiver_mesh)[receiver_index];
        auto emitter_centroid = emitter_element.getCentroid();
        Ray<FLOAT_TYPE> ray;
        ray.setOrigin(emitter_centroid).setDirection((receiver_element.getCentroid() - emitter_centroid).normalize());
        bool culled = ray.backFaceCull(emitter_element, receiver_element);
        auto full_matrix_index = emitter_index * num_receiver_elements + receiver_index;
        if (culled) {
          unculled_indices[full_matrix_index] = (unsigned int)0;
        } else {
          unculled_indices[full_matrix_index] = full_matrix_index;
        }
      }
    }
    unculled_indices.erase(std::remove(unculled_indices.begin(), unculled_indices.end(), (unsigned int)0), unculled_indices.end());
    return unculled_indices;
  }

  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE std::vector<unsigned int> DoubleAreaIntegration<FLOAT_TYPE>::evaluateBlocking(std::shared_ptr<Triangulation<FLOAT_TYPE>> emitter_mesh, std::shared_ptr<Triangulation<FLOAT_TYPE>> receiver_mesh, std::vector<unsigned int> unculled_indices, Blockers<FLOAT_TYPE> blockers) const {
    std::vector<unsigned int> unblocked_indices;
    auto num_emitter_elements = emitter_mesh->getNumElements();
    auto num_receiver_elements = receiver_mesh->getNumElements();
    for (auto index : unculled_indices) {
      auto emitter_index = index / num_receiver_elements;
      auto receiver_index = index % num_receiver_elements;
      auto emitter_element = (*emitter_mesh)[emitter_index];
      auto receiver_element = (*receiver_mesh)[receiver_index];
      auto emitter_element_centroid = emitter_element.getCentroid();
      auto receiver_element_centroid = receiver_element.getCentroid();
      auto ray_vector = emitter_element_centroid - receiver_element_centroid;
      Ray<FLOAT_TYPE> ray;
      ray.setOrigin(emitter_element_centroid).setDirection(ray_vector.normalize());
      for (auto blocker : blockers) {
        blocker->intersectRayWithBVH(ray);
      }
      if (ray.getIntersectionDistance() < ray_vector.getMagnitude()) { unblocked_indices.push_back(index); }
    }
    return unblocked_indices;
  }

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