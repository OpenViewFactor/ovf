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
    auto ray = receiver_centroid - emitter_centroid;
    return (ray.dot(emitter_normal) <= 0 || ray.dot(receiver_normal) >= 0);
  }

  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE std::vector<unsigned int> Solver<FLOAT_TYPE>::backFaceCullMeshes(std::shared_ptr<Triangulation<FLOAT_TYPE>> emitter_mesh, std::shared_ptr<Triangulation<FLOAT_TYPE>> receiver_mesh) const {
    auto num_emitter_elements = emitter_mesh->getNumElements();
    auto num_receiver_elements = receiver_mesh->getNumElements();
    std::vector<unsigned int> unculled_indices(num_emitter_elements * num_receiver_elements, num_emitter_elements * num_receiver_elements);
    auto emitter_centroids = emitter_mesh->getCentroids();
    auto receiver_centroids = receiver_mesh->getCentroids();
    auto emitter_normals = emitter_mesh->getNormals();
    auto receiver_normals = receiver_mesh->getNormals();
    #pragma omp parallel for
    for (int emitter_index = 0; emitter_index < num_emitter_elements; emitter_index++) {
      for (int receiver_index = 0; receiver_index < num_receiver_elements; receiver_index++) {
        bool culled = this->backFaceCullElements(emitter_centroids[emitter_index], emitter_normals[emitter_index], receiver_centroids[receiver_index], receiver_normals[receiver_index]);
        if (!culled) {
          auto full_matrix_index = emitter_index * num_receiver_elements + receiver_index;
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
    for (int bvh_index = 0; bvh_index < blockers.size(); bvh_index++) {
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
    if (blockers.size() == 0) {
      return unculled_indices;
    }
    auto num_emitter_elements = emitter_mesh->getNumElements();
    auto num_receiver_elements = receiver_mesh->getNumElements();
    std::vector<unsigned int> unblocked_indices(unculled_indices.size(), (unsigned int)(num_emitter_elements * num_receiver_elements));
    auto emitter_elements = emitter_mesh->getTriangles();
    auto receiver_elements = receiver_mesh->getTriangles();
    #pragma omp parallel for
    for (int i = 0; i < unculled_indices.size(); i++) {
      auto index = unculled_indices[i];
      auto emitter_index = index / num_receiver_elements;
      auto receiver_index = index % num_receiver_elements;
      bool blocked = this->evaluateBlockingBetweenElements(emitter_elements[emitter_index], receiver_elements[receiver_index], blockers);
      if (!blocked) {
        unblocked_indices[index] = index;
      }
    }
    unblocked_indices.erase(std::remove(unblocked_indices.begin(), unblocked_indices.end(), (unsigned int)(num_emitter_elements * num_receiver_elements)), unblocked_indices.end());
    return unblocked_indices;
  }

  template <typename FLOAT_TYPE>
    OVF_HOST_DEVICE void Solver<FLOAT_TYPE>::solveViewFactorBetweenMeshes(std::shared_ptr<Triangulation<FLOAT_TYPE>> emitter_mesh, std::shared_ptr<Triangulation<FLOAT_TYPE>> receiver_mesh, std::vector<unsigned int> unblocked_indices, std::shared_ptr<ViewFactor<FLOAT_TYPE>> results) const {
    auto emitter_centroids = emitter_mesh->getCentroids();
    auto receiver_centroids = receiver_mesh->getCentroids();
    auto emitter_normals = emitter_mesh->getNormals();
    auto receiver_normals = receiver_mesh->getNormals();
    auto receiver_areas = receiver_mesh->getAreas();
    auto num_receiver_elements = receiver_mesh->getNumElements();
    std::vector<FLOAT_TYPE> view_factors(unblocked_indices.size());
    #pragma omp parallel for shared(num_receiver_elements, view_factors)
    for (int i = 0; i < unblocked_indices.size(); i++) {
      auto index = unblocked_indices[i];
      auto emitter_index = index / num_receiver_elements;
      auto receiver_index = index % num_receiver_elements;
      view_factors[i] = this->solveViewFactorBetweenElements(emitter_centroids[emitter_index], emitter_normals[emitter_index], receiver_centroids[receiver_index], receiver_normals[receiver_index], receiver_areas[receiver_index]);
    }
    results->setElements(unblocked_indices, view_factors);
  }

  template class Solver<float>;
  template class Solver<double>;
}