#ifndef OVF_VIEW_FACTOR_H_
#define OVF_VIEW_FACTOR_H_

#include <iostream>
#include <exception>
#include <vector>
#include <memory>
#include <execution>
#include <omp.h>

#include "../../config.hpp"
#include "Triangulation.hpp"

enum ViewFactorState { UNLINKED, LINKED_ONE_MESH, LINKED_TWO_MESH };

namespace openviewfactor {
  template <typename t> class ViewFactor {
    private:
      ViewFactorState _state;
      // std::vector<IndexValuePair<t>> _nonzero_vf;
      std::vector<unsigned int> _indices;
      std::vector<t> _view_factors;
      std::shared_ptr<Triangulation<t>> _emitter;
      std::shared_ptr<Triangulation<t>> _receiver;
    protected:
    public:
      gpuify ViewFactor();

      gpuify ViewFactor<t>& linkTriangulations(std::shared_ptr<Triangulation<t>> emitter, std::shared_ptr<Triangulation<t>> receiver, unsigned int size);

      gpuify ViewFactorState getState() const;
      gpuify bool isLinked() const;

      gpuify unsigned int getNumEmitterElements() const;
      gpuify unsigned int getNumReceiverElements() const;

      gpuify unsigned int getNumTriangulations() const;
      gpuify std::shared_ptr<Triangulation<t>> getEmitterMesh() const;
      gpuify std::shared_ptr<Triangulation<t>> getReceiverMesh() const;

      gpuify int binarySearch(unsigned int full_matrix_index, unsigned int inclusive_start, unsigned int uninclusive_end) const;
      gpuify t getMatrixElementVF(unsigned int full_matrix_index) const;
      gpuify t getMatrixElementVF(unsigned int emitter_index, unsigned int receiver_index) const;

      gpuify t getEmitterElementToReceiverSurfaceVF(unsigned int emitter_index) const;
      gpuify t getReceiverElementToEmitterSurfaceVF(unsigned int receiver_index) const;
      gpuify t getSurfaceToSurfaceAverageVF() const;

      gpuify ViewFactor<t>& setElements(std::vector<unsigned int> indices, std::vector<t> view_factors);
  };
}
#endif