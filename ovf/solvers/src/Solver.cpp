#include "Solver.hpp"

namespace openviewfactor {

  template <typename t>
  gpuify Solver<t>::Solver()
    : _options(SolverOptions()) {}

  template <typename t>
  gpuify SolverOptions Solver<t>::getOptions() const { return _options; }

  template <typename t>
  gpuify Solver<t>& Solver<t>::setOptions(const SolverOptions &options) { _options = options; return *this; }

  template <typename t>
  gpuify bool Solver<t>::backFaceCullElements(const Vector3<t>& emitter_centroid, const Vector3<t>& emitter_normal, const Vector3<t>& receiver_centroid, const Vector3<t> receiver_normal) const {
    auto ray = (receiver_centroid - emitter_centroid).normalize();
    return (ray.dot(emitter_normal) <= 0.0 || ray.dot(receiver_normal) >= 0.0);
  }

  template <typename t>
  gpuify std::vector<unsigned int> Solver<t>::backFaceCullMeshes(unsigned int num_emitter_elements, unsigned int num_receiver_elements, std::vector<Vector3<t>>* emitter_centroids, std::vector<Vector3<t>>* receiver_centroids, std::vector<Vector3<t>>* emitter_normals, std::vector<Vector3<t>>* receiver_normals) const {
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



  template <typename t>
  gpuify bool Solver<t>::evaluateBlockingBetweenElements(std::shared_ptr<Ray<t>> ray, t ray_magnitude, const Blockers<t>& blockers) const {
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

  template <typename t>
  gpuify std::vector<unsigned int> Solver<t>::evaluateBlockingBetweenMeshes(unsigned int num_emitter_elements, unsigned int num_receiver_elements, std::vector<Vector3<t>> emitter_centroids, std::vector<Vector3<t>> receiver_centroids, Blockers<t> blockers, std::vector<unsigned int> unculled_indices) const {
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
      auto ray = std::make_shared<Ray<t>>();
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

  template <typename t>
  gpuify void Solver<t>::solveViewFactorBetweenMeshes(unsigned int num_emitter_elements, unsigned int num_receiver_elements, std::vector<Vector3<t>>* emitter_centroids,std::vector<Vector3<t>>* emitter_normals, std::vector<Triangle<t>>* receiver_elements, std::vector<unsigned int>* unblocked_indices, std::shared_ptr<ViewFactor<t>> results) const {
    std::vector<t> view_factors(unblocked_indices->size());
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