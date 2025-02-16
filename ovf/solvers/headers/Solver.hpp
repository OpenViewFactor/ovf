#ifndef _OVF_SOLVER_H_
#define _OVF_SOLVER_H_

#include "../../config.hpp"

#include <vector>
#include <algorithm>
#include <iostream>

#include "Vector3.hpp"
#include "Triangle.hpp"
#include "Triangulation.hpp"
#include "BVH.hpp"
#include "Blockers.hpp"
#include "Ray.hpp"
#include "SolverOptions.hpp"
#include "ViewFactor.hpp"

#include <omp.h>

namespace openviewfactor {
  template <typename t> class Solver {
    private:
    protected:
      SolverOptions _options;
    public:
      gpuify Solver();
      
      gpuify SolverOptions getOptions() const;
      gpuify Solver<t>& setOptions(const SolverOptions &options);

      virtual ~Solver() {}

      //* ------------------------------ COMMON METHODS ------------------------------ *//

      gpuify bool backFaceCullElements(const Vector3<t>& emitter_centroid, const Vector3<t>& emitter_normal, const Vector3<t>& receiver_centroid, const Vector3<t> receiver_normal) const;

      gpuify std::vector<unsigned int> backFaceCullMeshes(unsigned int num_emitter_elements, unsigned int num_receiver_elements, std::vector<Vector3<t>>* emitter_centroids, std::vector<Vector3<t>>* receiver_centroids, std::vector<Vector3<t>>* emitter_normals, std::vector<Vector3<t>>* receiver_normals) const;
      
      gpuify bool evaluateBlockingBetweenElements(std::shared_ptr<Ray<t>> ray, t ray_magnitude, const Blockers<t>& blockers) const;

      gpuify std::vector<unsigned int> evaluateBlockingBetweenMeshes(unsigned int num_emitter_elements, unsigned int num_receiver_elements, std::vector<Vector3<t>> emitter_centroids, std::vector<Vector3<t>> receiver_centroids, Blockers<t> blockers, std::vector<unsigned int> unculled_indices) const;

      gpuify void solveViewFactorBetweenMeshes(unsigned int num_emitter_elements, unsigned int num_receiver_elements, std::vector<Vector3<t>>* emitter_centroids, std::vector<Vector3<t>>* emitter_normals, std::vector<Triangle<t>>* receiver_elements, std::vector<unsigned int>* unblocked_indices, std::shared_ptr<ViewFactor<t>> results) const;

      //* ------------------------------ VIRTUAL METHODS ------------------------------ *//

      virtual gpuify t solveViewFactorBetweenElements(const Vector3<t>& emitter_centroid, const Vector3<t>& emitter_normal, const Triangle<t>& receiver_element) const = 0;
  };
}

#endif