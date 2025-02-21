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
  OVF_HOST_DEVICE int ViewFactor<FLOAT_TYPE>::binarySearch(unsigned int full_matrix_index, unsigned int inclusive_start, unsigned int uninclusive_end) const {
    if (uninclusive_end > inclusive_start) {
      int midpoint = ((int)inclusive_start + (int)uninclusive_end) / 2;
      if (_indices[midpoint] == full_matrix_index) { return midpoint; }
      if (_indices[midpoint] > full_matrix_index) { return binarySearch(full_matrix_index, inclusive_start, midpoint); }
      return binarySearch(full_matrix_index, midpoint+1, uninclusive_end);
    }
    return -1;
  }
  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE FLOAT_TYPE ViewFactor<FLOAT_TYPE>::getMatrixElementVF(unsigned int full_matrix_index) const {
    auto indices_index = this->binarySearch(full_matrix_index, 0, _indices.size());
    if (indices_index != -1) { return _view_factors[indices_index];
    return 0.0;
  }
  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE FLOAT_TYPE ViewFactor<FLOAT_TYPE>::getMatrixElementVF(unsigned int emitter_index, unsigned int receiver_index, unsigned int num_receiver_elements) const {
    unsigned int full_matrix_index = (emitter_index * num_receiver_elements) + receiver_index;
    return (this->getMatrixElementVF(full_matrix_index, num_receiver_elements));
  }

  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE FLOAT_TYPE ViewFactor<FLOAT_TYPE>::getEmitterElementToReceiverSurfaceVF(unsigned int emitter_index, unsigned int num_receiver_elements) const {
    FLOAT_TYPE total_view_factor = 0.0;
    for (unsigned int i = 0; i < num_receiver_elements; i++) {
      auto element_view_factor = this->getMatrixElementVF(emitter_index, i, num_receiver_elements);
      total_view_factor += element_view_factor;
    }
    return total_view_factor;
  }

  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE FLOAT_TYPE ViewFactor<FLOAT_TYPE>::getReceiverElementToEmitterSurfaceVF(unsigned int receiver_index, unsigned int num_receiver_elements) const {
    FLOAT_TYPE receiver_element_area = (*_receiver)[receiver_index].getArea();
    FLOAT_TYPE inverse_receiver_element_area = 1.0 / receiver_element_area;
    unsigned int num_emitter_elements = _emitter->getNumElements();
    FLOAT_TYPE total_view_factor = 0.0;
    for (unsigned int i = 0; i < num_emitter_elements; i++) {
      total_view_factor += (this->getMatrixElementVF(i, receiver_index, num_receiver_elements) * (*_emitter)[i].getArea() * inverse_receiver_element_area);
    }
    return total_view_factor;
  }

  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE FLOAT_TYPE ViewFactor<FLOAT_TYPE>::getSurfaceToSurfaceAverageVF() const {
    auto emitter_areas = _emitter->getAreas();
    auto num_emitter_elements = emitter_areas.size();
    auto num_receiver_elements = _receiver->getNumElements();
    std::vector<FLOAT_TYPE> emitter_view_factors(num_emitter_elements);

    #pragma omp parallel for
    for (int i = 0; i < num_emitter_elements; i++) {
      emitter_view_factors[i] = (this->getEmitterElementToReceiverSurfaceVF(i, num_receiver_elements) * (*_emitter)[i].getArea());
    }
    FLOAT_TYPE scale_factor = 1.0 / std::reduce(std::execution::par, emitter_areas.cbegin(), emitter_areas.cend());
    FLOAT_TYPE total_vf = std::reduce(std::execution::par, emitter_view_factors.cbegin(), emitter_view_factors.cend()) * scale_factor;
    return total_vf;
  }

  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE ViewFactor<FLOAT_TYPE>& ViewFactor<FLOAT_TYPE>::setElements(std::vector<unsigned int> indices, std::vector<FLOAT_TYPE> view_factors) {
    _indices = indices;
    _view_factors = view_factors;
    return *this;
  }

  template class ViewFactor<float>;
  template class ViewFactor<double>;
}