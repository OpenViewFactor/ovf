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
      std::vector<FLOAT_TYPE> _nonzero_view_factors;
      std::vector<size_t> _full_matrix_indices_map;
    
    public:
      ViewFactor(std::vector<FLOAT_TYPE> full_matrix_indices_map);  //! initialize with filtered number from back-face culling and intersection
      // ViewFactor(size_t num_rows, size_t num_cols); //! initialize a full matrix

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
      FLOAT_TYPE *getPointer() const;

      FLOAT_TYPE getSurfaceToSurfaceAverageVF() const;
      FLOAT_TYPE getEmitterElementToReceiverSurfaceVF(size_t emitter_index) const;
      FLOAT_TYPE getReceiverElementFromEmitterSurfaceVF(size_t receiver_index) const;
      FLOAT_TYPE getMatrixElementVF(size_t emitter_index, size_t receiver_index) const;

      ViewFactor<FLOAT_TYPE>& setViewFactorVector(const std::vector<FLOAT_TYPE> &buffer);
      ViewFactor<FLOAT_TYPE>& setElement(size_t emitter_index, size_t receiver_index, FLOAT_TYPE value);
  };
}

#endif