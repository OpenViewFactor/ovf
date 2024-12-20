#include "Solver.hpp"

namespace openviewfactor {

  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE Solver<FLOAT_TYPE>::Solver()
    : _options(SolverOptions()) {}

  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE SolverOptions Solver<FLOAT_TYPE>::getOptions() const { return _options; }

  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE Solver<FLOAT_TYPE>& Solver<FLOAT_TYPE>::setOptions(const SolverOptions &options) { _options = options; return *this; }

  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE bool Solver<FLOAT_TYPE>::backFaceCullElements(const Triangle<FLOAT_TYPE>& emitter_element, const Triangle<FLOAT_TYPE>& receiver_element) const {
    auto emitter_centroid = emitter_element.getCentroid();
    Ray<FLOAT_TYPE> ray;
    ray.setOrigin(emitter_centroid);
    ray.setDirection((receiver_element.getCentroid() - emitter_centroid).normalize());
    bool culled = ray.backFaceCull(emitter_element, receiver_element);
    return culled;
  }

  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE bool Solver<FLOAT_TYPE>::backFaceCullElements(std::shared_ptr<Triangulation<FLOAT_TYPE>> emitter_mesh, std::shared_ptr<Triangulation<FLOAT_TYPE>> receiver_mesh, unsigned int emitter_index, unsigned int receiver_index) const {
    auto emitter_element = (*emitter_mesh)[emitter_index];
    auto receiver_element = (*receiver_mesh)[receiver_index];
    bool culled = this->backFaceCullElements(emitter_element, receiver_element);
    return culled;
  }

  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE std::vector<unsigned int> Solver<FLOAT_TYPE>::backFaceCullMeshes(std::shared_ptr<Triangulation<FLOAT_TYPE>> emitter_mesh, std::shared_ptr<Triangulation<FLOAT_TYPE>> receiver_mesh) const {
    auto num_emitter_elements = emitter_mesh->getNumElements();
    auto num_receiver_elements = receiver_mesh->getNumElements();
    std::vector<unsigned int> unculled_indices(num_emitter_elements * num_receiver_elements, num_emitter_elements * num_receiver_elements);
    for (unsigned int emitter_index = 0; emitter_index < num_emitter_elements; emitter_index++) {
      for (unsigned int receiver_index = 0; receiver_index < num_receiver_elements; receiver_index++) {
        bool culled = this->backFaceCullElements(emitter_mesh, receiver_mesh, emitter_index, receiver_index);
        auto full_matrix_index = emitter_index * num_receiver_elements + receiver_index;
        if (culled) {
          unculled_indices[full_matrix_index] = (unsigned int)0;
        } else {
          unculled_indices[full_matrix_index] = full_matrix_index;
        }
      }
    }
    unculled_indices.erase(std::remove(unculled_indices.begin(), unculled_indices.end(), (unsigned int)(num_emitter_elements * num_receiver_elements)), unculled_indices.end());
    return unculled_indices;
  }



  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE bool Solver<FLOAT_TYPE>::evaluateBlockingBetweenElements(Triangle<FLOAT_TYPE> emitter_element, Triangle<FLOAT_TYPE> receiver_element, const Blockers<FLOAT_TYPE>& blockers) const {
    auto emitter_element_centroid = emitter_element.getCentroid();
    auto receiver_element_centroid = receiver_element.getCentroid();
    auto ray_vector = receiver_element_centroid - emitter_element_centroid;
    auto ray = std::make_shared<Ray<FLOAT_TYPE>>();
    ray->setOrigin(emitter_element_centroid);
    ray->setDirection(ray_vector.normalize());
    bool blocked = false;
    for (unsigned int bvh_index = 0; bvh_index < blockers.size(); bvh_index++) {
      (blockers.getBVH(bvh_index))->intersectRayWithBVH(ray);
      if (ray->getIntersectionDistance() < ray_vector.getMagnitude()) {
        blocked = true;
        break;
      }
    }
    return blocked;
  }

  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE std::vector<unsigned int> Solver<FLOAT_TYPE>::evaluateBlockingBetweenMeshes(std::shared_ptr<Triangulation<FLOAT_TYPE>> emitter_mesh, std::shared_ptr<Triangulation<FLOAT_TYPE>> receiver_mesh, Blockers<FLOAT_TYPE> blockers, std::vector<unsigned int> unculled_indices) const {
    auto num_emitter_elements = emitter_mesh->getNumElements();
    auto num_receiver_elements = receiver_mesh->getNumElements();
    std::vector<unsigned int> unblocked_indices(num_emitter_elements * num_receiver_elements, (unsigned int)(num_emitter_elements * num_receiver_elements));
    for (auto index : unculled_indices) {
      auto emitter_index = index / num_receiver_elements;
      auto receiver_index = index % num_receiver_elements;
      auto emitter_element = (*emitter_mesh)[emitter_index];
      auto receiver_element = (*receiver_mesh)[receiver_index];
      bool blocked = this->evaluateBlockingBetweenElements(emitter_element, receiver_element, blockers);
      if (!blocked) { unblocked_indices[index] = index; }
    }
    unblocked_indices.erase(std::remove(unblocked_indices.begin(), unblocked_indices.end(), (unsigned int)(num_emitter_elements * num_receiver_elements)), unblocked_indices.end());
    return unblocked_indices;
  }

  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE std::unique_ptr<ViewFactor<FLOAT_TYPE>> Solver<FLOAT_TYPE>::solveViewFactorBetweenMeshes(std::shared_ptr<Triangulation<FLOAT_TYPE>> emitter_mesh, std::shared_ptr<Triangulation<FLOAT_TYPE>> receiver_mesh, std::vector<unsigned int> unblocked_indices) const {
    auto num_emitter_elements = emitter_mesh->getNumElements();
    auto num_receiver_elements = receiver_mesh->getNumElements();
    auto results = std::make_unique<ViewFactor<FLOAT_TYPE>>();
    results->linkTriangulations(emitter_mesh, receiver_mesh);
    for (auto index : unblocked_indices) {
      auto emitter_index = index / num_receiver_elements;
      auto receiver_index = index % num_receiver_elements;
      auto emitter_element = (*emitter_mesh)[emitter_index];
      auto receiver_element = (*receiver_mesh)[receiver_index];
      FLOAT_TYPE view_factor = this->solveViewFactorBetweenElements(emitter_element, receiver_element);
      results->setElement(index, view_factor);
    }
    return results;
  }

  template class Solver<float>;
  template class Solver<double>;
}