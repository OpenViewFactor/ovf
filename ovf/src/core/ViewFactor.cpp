#include "ovf/core/ViewFactor.hpp"

namespace openviewfactor {
  template <typename FLOAT_TYPE>
  ViewFactor<FLOAT_TYPE>::ViewFactor(std::vector<size_t> full_matrix_indices_map) 
    : _nonzero_view_factors(std::vector<FLOAT_TYPE> v(full_matrix_indices_map.size())),
      _full_matrix_index_map(full_matrix_indices_map) {}
  
  template <typename FLOAT_TYPE>
  ViewFactor<FLOAT_TYPE>& ViewFactor<FLOAT_TYPE>::link(std::shared_ptr<Triangulation<FLOAT_TYPE>> mesh) {
    //TODO FILL THIS OUT
  }
  template <typename FLOAT_TYPE>
  ViewFactor<FLOAT_TYPE>& ViewFactor<FLOAT_TYPE>::link(std::shared_ptr<Triangulation<FLOAT_TYPE>> emitter_mesh,
                                                       std::shared_ptr<Triangulation<FLOAT_TYPE>> receiver_mesh) {
    //TODO FILL THIS OUT
  }
  template <typename FLOAT_TYPE>
  ViewFactor<FLOAT_TYPE>& ViewFactor<FLOAT_TYPE>::unlink() {
    //TODO FILL THIS OUT
  }
  template <typename FLOAT_TYPE>
  bool ViewFactor<FLOAT_TYPE>::isLinked() const { return _state != UNLINKED; }

  template <typename FLOAT_TYPE>
  size_t ViewFactor<FLOAT_TYPE>::getNumEmitterElements() const { return _emitter.size(); }
  template <typename FLOAT_TYPE>
  size_t ViewFactor<FLOAT_TYPE>::getNumReceiverElements() const { return _receiver.size(); }

  template <typename FLOAT_TYPE>
  size_t ViewFactor<FLOAT_TYPE>::getNumMeshes() const { return static_cast<size_t>(_state); }
  template <typename FLOAT_TYPE>
  std::shared_ptr<Triangulation<FLOAT_TYPE>> ViewFactor<FLOAT_TYPE>::getEmitterMesh() const {
    switch (_state) {
      case LINKED_ONE_MESH: //! fall through to LINKED_TWO_MESH
      case LINKED_TWO_MESH:
        return _emitter;
      default:
        throw std::exception("Cannot get mesh: No mesh linked to the result");
    }
  }
  template <typename FLOAT_TYPE>
  std::shared_ptr<Triangulation<FLOAT_TYPE>> ViewFactor<FLOAT_TYPE>::getReceiverMesh() const {
    switch (_state) {
      case LINKED_ONE_MESH:
        return _emitter;
      case LINKED_TWO_MESH:
        return _receiver;
      default:
        throw std::exception("Cannot get mesh: No mesh linked to the result");
    }
  }
  template <typename FLOAT_TYPE>
  FLOAT_TYPE *ViewFactor<FLOAT_TYPE>::getPointer() const { return _vf.data(); }
  template <typename FLOAT_TYPE>
  FLOAT_TYPE ViewFactor<FLOAT_TYPE>::getSurfacetoSurfaceAverageVF() const {
    //TODO FILL THIS OUT
  }
  template <typename FLOAT_TYPE>
  FLOAT_TYPE ViewFactor<FLOAT_TYPE>::getEmitterElementToReceiverSurfaceVF(size_t emitter_index) const {
    //TODO FILL THIS OUT
  }
  template <typename FLOAT_TYPE>
  FLOAT_TYPE getReceiverElementFromEmitterSurfaceVF(size_t receiver_index) const {
    //TODO FILL THIS OUT
  }
  template <typename FLOAT_TYPE>
  FLOAT_TYPE getMatrixElementVF(size_t emitter_index, size_t receiver_index) const {
    //TODO FILL THIS OUT
  }

  template <typename FLOAT_TYPE>
  ViewFactor<FLOAT_TYPE>& ViewFactor<FLOAT_TYPE>::setViewFactorVector(const std::vector<FLOAT_TYPE> &v) {
    _nonzero_view_factors = v;
  }
  template <typename FLOAT_TYPE>
  ViewFactor<FLOAT_TYPE>& ViewFactor<FLOAT_TYPE>::setElement(size_t emitter_index, size_t receiver_index, FLOAT_TYPE value) {
    //TODO FILL THIS OUT
  }
}