#define _USE_MATH_DEFINES
#include <cmath>
#include <numeric>

#include "ViewFactor.hpp"

namespace openviewfactor {
  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE ViewFactor<FLOAT_TYPE>::ViewFactor()
    : _state(UNLINKED), _indices(std::vector<unsigned int>()), _view_factors(std::vector<FLOAT_TYPE>()), _emitter(nullptr), _receiver(nullptr) {}
  
  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE ViewFactor<FLOAT_TYPE>& ViewFactor<FLOAT_TYPE>::linkTriangulations(std::shared_ptr<Triangulation<FLOAT_TYPE>> emitter, std::shared_ptr<Triangulation<FLOAT_TYPE>> receiver, unsigned int size) {
    _indices.clear();
    _view_factors.clear();
    bool same_triangulation = (emitter == receiver);
    if (same_triangulation) { _state = LINKED_ONE_MESH; } else { _state = LINKED_TWO_MESH; }
    _emitter = emitter;
    _receiver = receiver;
    _indices = std::vector<unsigned int>(size);
    _view_factors = std::vector<FLOAT_TYPE>(size);
    return *this;
  }

  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE ViewFactorState ViewFactor<FLOAT_TYPE>::getState() const { return _state; }

  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE bool ViewFactor<FLOAT_TYPE>::isLinked() const { return (_emitter != nullptr); }

  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE unsigned int ViewFactor<FLOAT_TYPE>::getNumEmitterElements() const {
    if (!(this->isLinked())) { throw std::runtime_error("Cannot evaluate the size of the emitting Triangulation for the ViewFactor object. No Triangulation is linked."); }
    return _emitter->getNumElements();
  }
  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE unsigned int ViewFactor<FLOAT_TYPE>::getNumReceiverElements() const {
    if (!(this->isLinked())) { throw std::runtime_error("Cannot evaluate the size of the receiving Triangulation for the ViewFactor object. No Triangulation is linked."); }
    return _receiver->getNumElements();
  }

  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE unsigned int ViewFactor<FLOAT_TYPE>::getNumTriangulations() const {
    if (!(this->isLinked())) { throw std::runtime_error("No Triangulations are linked to the ViewFactor object."); }
    if (_state == LINKED_ONE_MESH) { return 1; }
    return 2;
  }

  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE std::shared_ptr<Triangulation<FLOAT_TYPE>> ViewFactor<FLOAT_TYPE>::getEmitterMesh() const { return _emitter; }
  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE std::shared_ptr<Triangulation<FLOAT_TYPE>> ViewFactor<FLOAT_TYPE>::getReceiverMesh() const { return _receiver; }

  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE unsigned int ViewFactor<FLOAT_TYPE>::binarySearch(unsigned int full_matrix_index, unsigned int inclusive_start, unsigned int uninclusive_end) const {
    if (uninclusive_end > inclusive_start) {
      auto midpoint = (inclusive_start + uninclusive_end) / 2;
      if (_indices[midpoint] == full_matrix_index) { return midpoint; }
      if (_indices[midpoint] > full_matrix_index) { return binarySearch(full_matrix_index, inclusive_start, midpoint); }
      return binarySearch(full_matrix_index, midpoint+1, uninclusive_end);
    }
    return -1;
  }
  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE FLOAT_TYPE ViewFactor<FLOAT_TYPE>::getMatrixElementVF(unsigned int full_matrix_index) const {
    if (!(this->isLinked())) { throw std::runtime_error("Cannot access a value in this object, there are no linked triangulations"); }
    auto indices_index = this->binarySearch(full_matrix_index, 0, _indices.size());
    if (indices_index != -1) { return _view_factors[_indices[indices_index]]; }
    return 0.0;
  }
  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE FLOAT_TYPE ViewFactor<FLOAT_TYPE>::getMatrixElementVF(unsigned int emitter_index, unsigned int receiver_index) const {
    if (!(this->isLinked())) { throw std::runtime_error("Cannot access a value in this object, there are no linked triangulations"); }
    if (emitter_index > (_emitter->getNumElements() - 1)) { throw std::runtime_error("Cannot evaluate the VF for an emitting element index which exceeds the number of elements in the emitting surface"); }
    if (receiver_index > (_receiver->getNumElements() - 1)) { throw std::runtime_error("Cannot evaluate the VF for a receiving element index which exceeds the number of elements in the receiving surface"); }

    unsigned int full_matrix_index = (emitter_index * _emitter->getNumElements()) + receiver_index;
    unsigned int maximum_index = _emitter->getNumElements() * _receiver->getNumElements() - 1;
    if (full_matrix_index > maximum_index) { throw std::runtime_error("Cannot access a value for this index, it exceeds the matrix size"); }

    return (this->getMatrixElementVF(full_matrix_index));
  }

  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE FLOAT_TYPE ViewFactor<FLOAT_TYPE>::getEmitterElementToReceiverSurfaceVF(unsigned int emitter_index) const {
    if (!(this->isLinked())) { throw std::runtime_error("Cannot evaluate emitter element to receiver surface, this object is not linked to any triangulations"); }
    if (emitter_index > (_emitter->getNumElements() - 1)) { throw std::runtime_error("Cannot evaluate the VF for an emitting element index which exceeds the number of elements in the emitting surface"); }
    
    FLOAT_TYPE sum = 0.0;

    unsigned int num_receiver_elements = _receiver->getNumElements();
    for (unsigned int i = 0; i < _receiver->getNumElements(); i++) {
      sum += this->getMatrixElementVF(emitter_index, i);
    }

    return sum;
  }

  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE FLOAT_TYPE ViewFactor<FLOAT_TYPE>::getReceiverElementToEmitterSurfaceVF(unsigned int receiver_index) const {
    if (!(this->isLinked())) { throw std::runtime_error("Cannot evaluate receiver element to emitter surface, this object is not linked to any triangulations"); }
    if (receiver_index > (_receiver->getNumElements() - 1)) { throw std::runtime_error("Cannot evaluate the VF for a receiving element index which exceeds the number of elements in the receiving surface"); }

    FLOAT_TYPE receiver_element_area = (*_receiver)[receiver_index].getArea();
    FLOAT_TYPE sum = 0.0;
    for (unsigned int i = 0; i < _emitter->getNumElements(); i++) {
      sum += (this->getMatrixElementVF(i, receiver_index) * (*_emitter)[i].getArea() / receiver_element_area);
    }

    return sum;
  }

  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE FLOAT_TYPE ViewFactor<FLOAT_TYPE>::getSurfaceToSurfaceAverageVF() const {
    if (!(this->isLinked())) { throw std::runtime_error("Cannot evaluate surface to surface, this object is not linked to any triangulations"); }
    auto num_emitter_elements = _emitter->getNumElements();
    std::vector<FLOAT_TYPE> emitter_view_factors(num_emitter_elements);
    auto emitter_triangles = _emitter->getTriangles();
    #pragma omp parallel for
    for (unsigned int i = 0; i < num_emitter_elements; i++) {
      emitter_view_factors[i] = (this->getEmitterElementToReceiverSurfaceVF(i) * emitter_triangles[i].getArea());
    }
    FLOAT_TYPE total_vf = -1.0 * std::accumulate(emitter_view_factors.begin(), emitter_view_factors.end(), 0.0) / _emitter->getMeshArea();
    return total_vf;
  }

  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE ViewFactor<FLOAT_TYPE>& ViewFactor<FLOAT_TYPE>::setElements(std::vector<unsigned int> indices, std::vector<FLOAT_TYPE> view_factors) {
    std::copy(indices.begin(), indices.end(), _indices.begin());
    std::copy(view_factors.begin(), view_factors.end(), _view_factors.begin());
    return *this;
  }

  template class ViewFactor<float>;
  template class ViewFactor<double>;
}