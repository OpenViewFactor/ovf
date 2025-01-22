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

#include <cuda.h>
#include <cuda_runtime_api.h>
#include <device_launch_parameters.h>

#include <thrust/host_vector.h>
#include <thrust/device_vector.h>
#include <thrust/copy.h>

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

      std::vector<unsigned int> backFaceCullMeshesCPU(std::shared_ptr<Triangulation<FLOAT_TYPE>> emitter_mesh, std::shared_ptr<Triangulation<FLOAT_TYPE>> receiver_mesh) const;
      __global__ std::vector<unsigned int> backFaceCullMeshesGPU(std::shared_ptr<Triangulation<FLOAT_TYPE>> emitter_mesh, std::shared_ptr<Triangulation<FLOAT_TYPE>> receiver_mesh) const;

      OVF_HOST_DEVICE bool evaluateBlockingBetweenElements(Triangle<FLOAT_TYPE> emitter_element, Triangle<FLOAT_TYPE> receiver_element, const Blockers<FLOAT_TYPE>& blockers) const;

      OVF_HOST_DEVICE std::vector<unsigned int> evaluateBlockingBetweenMeshes(std::shared_ptr<Triangulation<FLOAT_TYPE>> emitter_mesh, std::shared_ptr<Triangulation<FLOAT_TYPE>> receiver_mesh, Blockers<FLOAT_TYPE> blockers, std::vector<unsigned int> unculled_indices) const;

      OVF_HOST_DEVICE void solveViewFactorBetweenMeshes(std::shared_ptr<Triangulation<FLOAT_TYPE>> emitter_mesh, std::shared_ptr<Triangulation<FLOAT_TYPE>> receiver_mesh, std::vector<unsigned int> unblocked_indices, std::shared_ptr<ViewFactor<FLOAT_TYPE>> results) const;

      //* ------------------------------ VIRTUAL METHODS ------------------------------ *//

      virtual OVF_HOST_DEVICE FLOAT_TYPE solveViewFactorBetweenElements(const Vector3<FLOAT_TYPE>& emitter_centroid, const Vector3<FLOAT_TYPE>& emitter_normal, const Triangle<FLOAT_TYPE>& receiver_element) const = 0;
  };
}

#endif