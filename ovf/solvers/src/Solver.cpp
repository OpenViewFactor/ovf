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
  OVF_HOST_DEVICE bool Solver<FLOAT_TYPE>::backFaceCullElements(const Vector3<FLOAT_TYPE>& emitter_centroid, const Vector3<FLOAT_TYPE>& emitter_normal, const Vector3<FLOAT_TYPE>& receiver_centroid, const Vector3<FLOAT_TYPE> receiver_normal) const {
    auto ray = (receiver_centroid - emitter_centroid).normalize();
    return (ray.dot(emitter_normal) <= 0.0001 || ray.dot(receiver_normal) >= -0.0001);
  }

  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE std::vector<unsigned int> Solver<FLOAT_TYPE>::backFaceCullMeshes(unsigned int num_emitter_elements, unsigned int num_receiver_elements, std::vector<Vector3<FLOAT_TYPE>>* emitter_centroids, std::vector<Vector3<FLOAT_TYPE>>* receiver_centroids, std::vector<Vector3<FLOAT_TYPE>>* emitter_normals, std::vector<Vector3<FLOAT_TYPE>>* receiver_normals) const {
    auto problem_size = num_emitter_elements * num_receiver_elements;
    std::vector<unsigned int> unculled_indices(problem_size);
    std::iota(unculled_indices.begin(), unculled_indices.end(), 0);
    #pragma omp parallel for
    for (int emitter_index = 0; emitter_index < num_emitter_elements; emitter_index++) {
      for (int receiver_index = 0; receiver_index < num_receiver_elements; receiver_index++) {
        bool culled = this->backFaceCullElements((*emitter_centroids)[emitter_index], (*emitter_normals)[emitter_index], (*receiver_centroids)[receiver_index], (*receiver_normals)[receiver_index]);
        if (culled) {
          auto full_matrix_index = emitter_index * num_receiver_elements + receiver_index;
          unculled_indices[full_matrix_index] = problem_size;
        }
      }
    }
    std::cout << "[TEMP LOG] Unculled Indices Size (Pre-Filter): " << unculled_indices.size() << '\n';
    auto end_iterator = std::remove(unculled_indices.begin(), unculled_indices.end(), problem_size);
    unculled_indices.erase(end_iterator, unculled_indices.end());
    std::cout << "[TEMP LOG] Unculled Indices Size (Post-Filter): " << unculled_indices.size() << '\n';
    return unculled_indices;
  }



  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE bool Solver<FLOAT_TYPE>::evaluateBlockingBetweenElements(std::shared_ptr<Ray<FLOAT_TYPE>> ray, FLOAT_TYPE ray_magnitude, const Blockers<FLOAT_TYPE>& blockers) const {
    bool blocked = false;
    for (int bvh_index = 0; bvh_index < blockers.size(); bvh_index++) {
      (blockers.getBVH(bvh_index))->intersectRayWithBVH(ray);
      if (ray->getIntersectionDistance() < ray_magnitude) {
        blocked = true;
        break;
      }
    }
    return blocked;
  }

  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE std::vector<unsigned int> Solver<FLOAT_TYPE>::evaluateBlockingBetweenMeshes(unsigned int num_emitter_elements, unsigned int num_receiver_elements, std::vector<Vector3<FLOAT_TYPE>> emitter_centroids, std::vector<Vector3<FLOAT_TYPE>> receiver_centroids, Blockers<FLOAT_TYPE> blockers, std::vector<unsigned int> unculled_indices) const {
    if (blockers.size() == 0) {
      return unculled_indices;
    }
    std::vector<unsigned int> unblocked_indices(unculled_indices.size(), (num_emitter_elements * num_receiver_elements));
    #pragma omp parallel for
    for (int i = 0; i < unculled_indices.size(); i++) {
      auto index = unculled_indices[i];
      auto emitter_index = index / num_receiver_elements;
      auto receiver_index = index % num_receiver_elements;
      auto emitter_element_centroid = emitter_centroids[emitter_index];
      auto receiver_element_centroid = receiver_centroids[receiver_index];
      auto ray_vector = receiver_element_centroid - emitter_element_centroid;
      auto ray = std::make_shared<Ray<FLOAT_TYPE>>();
      ray->setOrigin(emitter_element_centroid);
      ray->setDirection(ray_vector.normalize());
      bool blocked = this->evaluateBlockingBetweenElements(ray, ray_vector.getMagnitude(), blockers);
      if (!blocked) {
        unblocked_indices[index] = index;
      }
    }
    auto end_iterator = std::remove(unblocked_indices.begin(), unblocked_indices.end(), (num_emitter_elements * num_receiver_elements));
    unblocked_indices.erase(end_iterator, unblocked_indices.end());
    return unblocked_indices;
  }

  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE void Solver<FLOAT_TYPE>::solveViewFactorBetweenMeshes(unsigned int num_emitter_elements, unsigned int num_receiver_elements, std::vector<Vector3<FLOAT_TYPE>>* emitter_centroids,std::vector<Vector3<FLOAT_TYPE>>* emitter_normals, std::vector<Triangle<FLOAT_TYPE>>* receiver_elements, std::vector<unsigned int>* unblocked_indices, std::shared_ptr<ViewFactor<FLOAT_TYPE>> results) const {
    std::vector<FLOAT_TYPE> view_factors(unblocked_indices->size());
    #pragma omp parallel for
    for (int i = 0; i < unblocked_indices->size(); i++) {
      auto index = (*unblocked_indices)[i];
      auto emitter_index = index / num_receiver_elements;
      auto receiver_index = index % num_receiver_elements;
      view_factors[i] = this->solveViewFactorBetweenElements((*emitter_centroids)[emitter_index], (*emitter_normals)[emitter_index], (*receiver_elements)[receiver_index]);
    }
    results->setElements(*unblocked_indices, view_factors);
  }

  template class Solver<float>;
  template class Solver<double>;
}