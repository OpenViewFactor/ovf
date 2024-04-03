#include "ovf/core/ViewFactor.hpp"

namespace openviewfactor {
  template <typename FLOAT_TYPE>
  ViewFactor<FLOAT_TYPE>::ViewFactor(std::vector<size_t> full_matrix_indices_map) {
    std::vector<FLOAT_TYPE> values;
    auto indices = full_matrix_indices_map;
    std::sort(indices.begin(), indices.end());
    for (int i=0; i < indices.size(); i++) {
      _nonzero_vf.push_back(std::make_pair(indices[i], values[i]));
    }
  }
  
  template <typename FLOAT_TYPE>
  ViewFactor<FLOAT_TYPE>& ViewFactor<FLOAT_TYPE>::link(std::shared_ptr<Triangulation<FLOAT_TYPE>> mesh) {
    if (_state != UNLINKED) {
      unlink();
    }
    _emitter = mesh;
    _state = LINKED_ONE_MESH;
    return *this;
  }
  template <typename FLOAT_TYPE>
  ViewFactor<FLOAT_TYPE>& ViewFactor<FLOAT_TYPE>::link(std::shared_ptr<Triangulation<FLOAT_TYPE>> emitter_mesh,
                                                       std::shared_ptr<Triangulation<FLOAT_TYPE>> receiver_mesh) {
    if (_state != UNLINKED) {
      unlink();
    }
    _emitter = emitter_mesh;
    _receiver = receiver_mesh;
    _state = LINKED_TWO_MESH;
    return *this;
  }
  template <typename FLOAT_TYPE>
  ViewFactor<FLOAT_TYPE>& ViewFactor<FLOAT_TYPE>::unlink() {
    if (_state != UNLINKED) {
      _emitter = nullptr;
      _receiver = nullptr;
      _state = UNLINKED;
    }
    return *this;
  }
  template <typename FLOAT_TYPE>
  bool ViewFactor<FLOAT_TYPE>::isLinked() const { return _state != UNLINKED; }

  template <typename FLOAT_TYPE>
  size_t ViewFactor<FLOAT_TYPE>::getNumEmitterElements() const {
    switch (_state) {
      case LINKED_ONE_MESH: //! fall through to LINKED_TWO_MESH
      case LINKED_TWO_MESH:
        return _emitter.size();
      default:
        throw std::exception("Cannot get mesh: No mesh linked to the result");
    }
  }
  template <typename FLOAT_TYPE>
  size_t ViewFactor<FLOAT_TYPE>::getNumReceiverElements() const {
    switch (_state) {
      case LINKED_ONE_MESH:
        return _emitter.size();
      case LINKED_TWO_MESH:
        return _receiver.size();
      default:
        throw std::exception("Cannot get mesh: No mesh linked to the result");
    }
  }

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
  FLOAT_TYPE *ViewFactor<FLOAT_TYPE>::getViewFactorVector() const { return _nonzero_vf.data(); }

  template <typename FLOAT_TYPE>
  ViewFactor<FLOAT_TYPE>& ViewFactor<FLOAT_TYPE>::sortIndicesMap() {
    std::sort(_full_matrix_indices_map.begin(), _full_matrix_indices_map.end());
    std::sort(_nonzero_view_factors.begin(), _nonzero_view_factors.end(), _full_matrix_indices_map);
    return *this;
  }
  template <typename FLOAT_TYPE>
  ViewFactor<FLOAT_TYPE>& ViewFactor<FLOAT_TYPE>::setViewFactorVector(const std::vector<FLOAT_TYPE> &v) {
    _nonzero_view_factors = v;
    return *this;
  }
  template <typename FLOAT_TYPE>
  ViewFactor<FLOAT_TYPE>& ViewFactor<FLOAT_TYPE>::setElement(size_t emitter_index, size_t receiver_index, FLOAT_TYPE value) {
    auto full_matrix_index = (emitter_index * _receiver.size()) + receiver_index;
    std::vector<size_t>::iterator it = std::find(_full_matrix_indices_map.begin(), _full_matrix_indices_map.end(), full_matrix_index);
    if (it != _full_matrix_indices_map.end()) {
      size_t index_in_mapping_vector = it - _full_matrix_indices_map.begin();
      _nonzero_view_factors[index_in_mapping_vector] = value;
      return *this;
    } else {
      throw std::exception("Element attempting to be populated in ViewFactor::setElement was already determined to be zero and is therefore not present in the ViewFactor object.");
    }
  }

  template <typename FLOAT_TYPE>
  FLOAT_TYPE ViewFactor<FLOAT_TYPE>::getSurfaceToSurfaceAverageVF() const {
    //TODO FILL THIS OUT
    FLOAT_TYPE sum = 0.0;
    switch (_state) {
      case LINKED_ONE_MESH:
      case LINKED_TWO_MESH:
      default:
        throw std::exception("Cannot perform transformation: Result not linked to mesh");
    }
  }
  template <typename FLOAT_TYPE>
  FLOAT_TYPE ViewFactor<FLOAT_TYPE>::getEmitterElementToReceiverSurfaceVF(size_t emitter_index) const {
    //TODO FILL THIS OUT
    FLOAT_TYPE sum = 0.0;
    switch (_state) {
      case LINKED_ONE_MESH:
        break;
      case LINKED_TWO_MESH:
        break;
      default:
        throw std::exception("Cannot perform transformation: Result not linked to mesh");
    }
  }
  template <typename FLOAT_TYPE>
  FLOAT_TYPE ViewFactor<FLOAT_TYPE>::getReceiverElementFromEmitterSurfaceVF(size_t receiver_index) const {
    //TODO FILL THIS OUT
    FLOAT_TYPE sum = 0.0;
    switch (_state) {
      case LINKED_ONE_MESH:
        break;
      case LINKED_TWO_MESH:
        break;
      default:
        throw std::exception("Cannot perform transformation: Result not linked to mesh");
    }
  }
  template <typename FLOAT_TYPE>
  FLOAT_TYPE ViewFactor<FLOAT_TYPE>::getMatrixElementVF(size_t emitter_index, size_t receiver_index) const {
    return _nonzero_vf[(emitter_index * _receiver.size()) + receiver_index];
  }
}