#ifndef OVF_VIEWFACTOR_H_
#define OVF_VIEWFACTOR_H_

#include "ovf/core/Triangulation.hpp"
#include <exception>
#include <memory>
#include <stdexcept>

namespace openviewfactor {
  enum ViewFactorState {
    UNLINKED = 0,
    LINKED_ONE_MESH = 1,
    LINKED_TWO_MESH = 2
  };

  template <typename FLOAT_TYPE> class ViewFactor {
    private:
      ViewFactorState _state = UNLINKED;
      std::shared_ptr<Triangulation<FLOAT_TYPE>> _emitter = nullptr;
      std::shared_ptr<Triangulation<FLOAT_TYPE>> _receiver = nullptr;
      std::vector<std::pair<size_t, FLOAT_TYPE>> _nonzero_vf;
    
    public:
      ViewFactor(std::vector<size_t> full_matrix_indices_map);

      ViewFactor<FLOAT_TYPE>& link(std::shared_ptr<Triangulation<FLOAT_TYPE>> mesh);  //* 1-mesh implementation
      ViewFactor<FLOAT_TYPE>& link(std::shared_ptr<Triangulation<FLOAT_TYPE>> emitter_mesh,
                                   std::shared_ptr<Triangulation<FLOAT_TYPE>> receiver_mesh); //* 2-mesh implementation
      ViewFactor<FLOAT_TYPE>& unlink();
      bool isLinked() const;

      size_t getNumEmitterElements() const;
      size_t getNumReceiverElements() const;
      size_t getNumMeshes() const;
      std::shared_ptr<Triangulation<FLOAT_TYPE>> getEmitterMesh() const;
      std::shared_ptr<Triangulation<FLOAT_TYPE>> getReceiverMesh() const;
      FLOAT_TYPE *getViewFactorVector() const;

      ViewFactor<FLOAT_TYPE>& sortIndicesMap();
      ViewFactor<FLOAT_TYPE>& setViewFactorVector(const std::vector<FLOAT_TYPE> &buffer);
      ViewFactor<FLOAT_TYPE>& setElement(size_t emitter_index, size_t receiver_index, FLOAT_TYPE value);

      FLOAT_TYPE getSurfaceToSurfaceAverageVF() const;
      FLOAT_TYPE getEmitterElementToReceiverSurfaceVF(size_t emitter_index) const;
      FLOAT_TYPE getReceiverElementFromEmitterSurfaceVF(size_t receiver_index) const;
      FLOAT_TYPE getMatrixElementVF(size_t emitter_index, size_t receiver_index) const;
  };
}

#endif