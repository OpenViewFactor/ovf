#ifndef OVF_VIEW_FACTOR_H_
#define OVF_VIEW_FACTOR_H_

#include <vector>
#include <memory>
#include <execution>
#include <omp.h>

#include "../../config.hpp"
#include "Triangulation.hpp"
#include "IndexValuePair.hpp"

enum ViewFactorState { UNLINKED, LINKED_ONE_MESH, LINKED_TWO_MESH };

namespace openviewfactor {
  template <typename FLOAT_TYPE> class ViewFactor {
    private:
      ViewFactorState _state;
      // std::vector<IndexValuePair<FLOAT_TYPE>> _nonzero_vf;
      std::vector<unsigned int> _indices;
      std::vector<FLOAT_TYPE> _view_factors;
      std::shared_ptr<Triangulation<FLOAT_TYPE>> _emitter;
      std::shared_ptr<Triangulation<FLOAT_TYPE>> _receiver;
    protected:
    public:
      OVF_HOST_DEVICE ViewFactor();

      OVF_HOST_DEVICE ViewFactor<FLOAT_TYPE>& linkTriangulations(std::shared_ptr<Triangulation<FLOAT_TYPE>> emitter, std::shared_ptr<Triangulation<FLOAT_TYPE>> receiver, unsigned int size);

      OVF_HOST_DEVICE ViewFactorState getState() const;
      OVF_HOST_DEVICE bool isLinked() const;

      OVF_HOST_DEVICE unsigned int getNumEmitterElements() const;
      OVF_HOST_DEVICE unsigned int getNumReceiverElements() const;

      OVF_HOST_DEVICE unsigned int getNumTriangulations() const;
      OVF_HOST_DEVICE std::shared_ptr<Triangulation<FLOAT_TYPE>> getEmitterMesh() const;
      OVF_HOST_DEVICE std::shared_ptr<Triangulation<FLOAT_TYPE>> getReceiverMesh() const;

      OVF_HOST_DEVICE unsigned int binarySearch(unsigned int full_matrix_index, unsigned int inclusive_start, unsigned int uninclusive_end) const;
      OVF_HOST_DEVICE FLOAT_TYPE getMatrixElementVF(unsigned int full_matrix_index) const;
      OVF_HOST_DEVICE FLOAT_TYPE getMatrixElementVF(unsigned int emitter_index, unsigned int receiver_index) const;

      OVF_HOST_DEVICE FLOAT_TYPE getEmitterElementToReceiverSurfaceVF(unsigned int emitter_index) const;
      OVF_HOST_DEVICE FLOAT_TYPE getReceiverElementToEmitterSurfaceVF(unsigned int receiver_index) const;
      OVF_HOST_DEVICE FLOAT_TYPE getSurfaceToSurfaceAverageVF() const;

      OVF_HOST_DEVICE ViewFactor<FLOAT_TYPE>& setElements(std::vector<unsigned int> indices, std::vector<FLOAT_TYPE> view_factors);
      // OVF_HOST_DEVICE ViewFactor<FLOAT_TYPE>& setElement(unsigned int full_matrix_index, FLOAT_TYPE value);
      // OVF_HOST_DEVICE ViewFactor<FLOAT_TYPE>& setElement(unsigned int emitter_index, unsigned int receiver_index, FLOAT_TYPE value);
  };
}
#endif