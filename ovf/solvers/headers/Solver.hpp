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

#define OVF_PI 3.1415926535897932

namespace openviewfactor {
  template <typename FLOAT_TYPE> class Solver {
    private:
    protected:
      SolverOptions _options;
    public:
      OVF_HOST_DEVICE Solver();
      
      OVF_HOST_DEVICE SolverOptions getOptions() const;
      OVF_HOST_DEVICE Solver<FLOAT_TYPE>& setOptions(const SolverOptions &options);

      virtual ~Solver() {}

      //* ------------------------------ COMMON METHODS ------------------------------ *//

      OVF_HOST_DEVICE bool backFaceCullElements(const Vector3<FLOAT_TYPE>& emitter_centroid, const Vector3<FLOAT_TYPE>& emitter_normal, const Vector3<FLOAT_TYPE>& receiver_centroid, const Vector3<FLOAT_TYPE> receiver_normal) const;

      OVF_HOST_DEVICE std::vector<unsigned int> backFaceCullMeshes(unsigned int num_emitter_elements, unsigned int num_receiver_elements, std::vector<Vector3<FLOAT_TYPE>>* emitter_centroids, std::vector<Vector3<FLOAT_TYPE>>* receiver_centroids, std::vector<Vector3<FLOAT_TYPE>>* emitter_normals, std::vector<Vector3<FLOAT_TYPE>>* receiver_normals) const;
      
      OVF_HOST_DEVICE bool evaluateBlockingBetweenElements(std::shared_ptr<Ray<FLOAT_TYPE>> ray, FLOAT_TYPE ray_magnitude, const Blockers<FLOAT_TYPE>& blockers) const;

      OVF_HOST_DEVICE std::vector<unsigned int> evaluateBlockingBetweenMeshes(unsigned int num_emitter_elements, unsigned int num_receiver_elements, std::vector<Vector3<FLOAT_TYPE>> emitter_centroids, std::vector<Vector3<FLOAT_TYPE>> receiver_centroids, Blockers<FLOAT_TYPE> blockers, std::vector<unsigned int> unculled_indices) const;

      OVF_HOST_DEVICE void solveViewFactorBetweenMeshes(unsigned int num_emitter_elements, unsigned int num_receiver_elements, std::vector<Vector3<FLOAT_TYPE>>* emitter_centroids, std::vector<Vector3<FLOAT_TYPE>>* emitter_normals, std::vector<Triangle<FLOAT_TYPE>>* receiver_elements, std::vector<unsigned int>* unblocked_indices, std::shared_ptr<ViewFactor<FLOAT_TYPE>> results) const;

      //* ------------------------------ VIRTUAL METHODS ------------------------------ *//

      virtual OVF_HOST_DEVICE FLOAT_TYPE solveViewFactorBetweenElements(const Vector3<FLOAT_TYPE>& emitter_centroid, const Vector3<FLOAT_TYPE>& emitter_normal, const Triangle<FLOAT_TYPE>& receiver_element) const = 0;
  };
}

#endif