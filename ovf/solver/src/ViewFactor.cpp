#include "solver/include/ViewFactor.hpp"

namespace openviewfactor {
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
  FLOAT_TYPE *ViewFactor<FLOAT_TYPE>::getNonZeroViewFactorVectorPtr() const { return _nonzero_vf.data(); }

  template <typename FLOAT_TYPE>
  ViewFactor<FLOAT_TYPE>& ViewFactor<FLOAT_TYPE>::setElement(size_t emitter_index, size_t receiver_index, FLOAT_TYPE value) {
    auto full_matrix_index = (emitter_index * getNumReceiverElements()) + receiver_index;
    std::pair<size_t, FLOAT_TYPE> p;
    p.first = full_matrix_index;
    p.second = value;
    _nonzero_vf.push_back(p);
    return *this;
  }

  template <typename FLOAT_TYPE>
  FLOAT_TYPE ViewFactor<FLOAT_TYPE>::getSurfaceToSurfaceAverageVF() const {
    FLOAT_TYPE outer_sum = 0.0;
    if (_state == UNLINKED) {
      throw std::exception("Error getting surface-surface average: result not linked to mesh");
    }
    for (size_t emitter_index=0; emitter_index < getNumEmitterElements(); emitter_index++) {
      outer_sum += (*_emitter)[emitter_index].area() * getEmitterElementToReceiverSurfaceVF();
    }
    return (outer_sum / _emitter->area());
  }
  template <typename FLOAT_TYPE>
  FLOAT_TYPE ViewFactor<FLOAT_TYPE>::getEmitterElementToReceiverSurfaceVF(size_t emitter_index) const {
    FLOAT_TYPE sum = 0.0;
    if (_state == UNLINKED) {
      throw std::exception("Error getting emitting element-surface average: result not linked to mesh");
    }
    for (size_t receiver_index=0; receiver_index < getNumReceiverElements(); receiver_index++) {
      if (emitter_index == receiver_index) {
        continue;
      }
      sum += getMatrixElementVF(emitter_index, receiver_index);
    }
    return sum;
  }
  template <typename FLOAT_TYPE>
  FLOAT_TYPE ViewFactor<FLOAT_TYPE>::getReceiverElementFromEmitterSurfaceVF(size_t receiver_index) const {
    switch (_state) {
      case LINKED_ONE_MESH:
        return getEmitterElementToReceiverSurfaceVF(receiver_index);
      case LINKED_TWO_MESH:
        FLOAT_TYPE sum = 0.0;
        for (size_t emitter_index=0; emitter_index < getNumEmitterElements(); emitter_index++) {
          sum += getMatrixElementVF(emitter_index, receiver_index) * ((*_emitter)[emitter_index].area() / (*_receiver)[receiver_index].area());
        }
        return sum;
      default:
        throw std::exception("Error getting receiving element-surface average: result not linked to mesh");
    }
  }
  template <typename FLOAT_TYPE>
  FLOAT_TYPE ViewFactor<FLOAT_TYPE>::getMatrixElementVF(size_t emitter_index, size_t receiver_index) const {
    auto full_matrix_index = (emitter_index * getNumReceiverElements()) +  receiver_index;
    std::vector<std::pair<size_t, FLOAT_TYPE>>::iterator it = std::find_if(_nonzero_vf.begin(), _nonzero_vf.end(), (n=0)[]mutable{ return (full_matrix_index==_nonzero_vf[n++].first); });
    if (it != _nonzero_vf.end()) {
      size_t index = it - _nonzero_vf.begin();
      return _nonzero_vf[index].second;
    } else {
      throw std::exception("The element being accessed in this call to getMatrixElementVF does not exist in the nonzero vf vector.");
    }
  }

template class ViewFactor<float>;
template class ViewFactor<double>;
}