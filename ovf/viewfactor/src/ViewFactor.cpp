#define _USE_MATH_DEFINES
#include <cmath>
#include <numeric>

#include "ViewFactor.hpp"

namespace openviewfactor {
  template <typename t>
  gpuify ViewFactor<t>::ViewFactor()
    : _state(UNLINKED), _indices(std::vector<unsigned int>()), _view_factors(std::vector<t>()), _emitter(nullptr), _receiver(nullptr) {}
  
  template <typename t>
  gpuify ViewFactor<t>& ViewFactor<t>::linkTriangulations(std::shared_ptr<Triangulation<t>> emitter, std::shared_ptr<Triangulation<t>> receiver, unsigned int size) {
    _indices.clear();
    _view_factors.clear();
    bool same_triangulation = (emitter == receiver);
    if (same_triangulation) { _state = LINKED_ONE_MESH; } else { _state = LINKED_TWO_MESH; }
    _emitter = emitter;
    _receiver = receiver;
    _indices = std::vector<unsigned int>(size);
    _view_factors = std::vector<t>(size);
    return *this;
  }

  template <typename t>
  gpuify ViewFactorState ViewFactor<t>::getState() const { return _state; }

  template <typename t>
  gpuify bool ViewFactor<t>::isLinked() const { return (_emitter != nullptr); }

  template <typename t>
  gpuify unsigned int ViewFactor<t>::getNumEmitterElements() const {
    if (!(this->isLinked())) { throw std::runtime_error("Cannot evaluate the size of the emitting Triangulation for the ViewFactor object. No Triangulation is linked."); }
    return _emitter->getNumElements();
  }
  template <typename t>
  gpuify unsigned int ViewFactor<t>::getNumReceiverElements() const {
    if (!(this->isLinked())) { throw std::runtime_error("Cannot evaluate the size of the receiving Triangulation for the ViewFactor object. No Triangulation is linked."); }
    return _receiver->getNumElements();
  }

  template <typename t>
  gpuify unsigned int ViewFactor<t>::getNumTriangulations() const {
    if (!(this->isLinked())) { throw std::runtime_error("No Triangulations are linked to the ViewFactor object."); }
    if (_state == LINKED_ONE_MESH) { return 1; }
    return 2;
  }

  template <typename t>
  gpuify std::shared_ptr<Triangulation<t>> ViewFactor<t>::getEmitterMesh() const { return _emitter; }
  template <typename t>
  gpuify std::shared_ptr<Triangulation<t>> ViewFactor<t>::getReceiverMesh() const { return _receiver; }

  template <typename t>
  gpuify int ViewFactor<t>::binarySearch(unsigned int full_matrix_index, unsigned int inclusive_start, unsigned int uninclusive_end) const {
    if (uninclusive_end > inclusive_start) {
      int midpoint = ((int)inclusive_start + (int)uninclusive_end) / 2;
      if (_indices[midpoint] == full_matrix_index) { return midpoint; }
      if (_indices[midpoint] > full_matrix_index) { return binarySearch(full_matrix_index, inclusive_start, midpoint); }
      return binarySearch(full_matrix_index, midpoint+1, uninclusive_end);
    }
    return -1;
  }
  template <typename t>
  gpuify t ViewFactor<t>::getMatrixElementVF(unsigned int full_matrix_index) const {
    auto indices_index = this->binarySearch(full_matrix_index, 0, _indices.size());
    if (indices_index != -1) { return _view_factors[indices_index]; }
    return 0.0;
  }
  template <typename t>
  gpuify t ViewFactor<t>::getMatrixElementVF(unsigned int emitter_index, unsigned int receiver_index) const {
    unsigned int full_matrix_index = (emitter_index * _receiver->getNumElements()) + receiver_index;
    unsigned int maximum_index = _emitter->getNumElements() * _receiver->getNumElements() - 1;
    if (full_matrix_index > maximum_index) { throw std::runtime_error("Cannot access a value for this index, it exceeds the matrix size"); }

    return (this->getMatrixElementVF(full_matrix_index));
  }

  template <typename t>
  gpuify t ViewFactor<t>::getEmitterElementToReceiverSurfaceVF(unsigned int emitter_index) const {
    unsigned int num_receiver_elements = _receiver->getNumElements();
    t total_view_factor = 0.0;
    for (unsigned int i = 0; i < num_receiver_elements; i++) {
      total_view_factor += this->getMatrixElementVF(emitter_index, i);
    }
    return total_view_factor;
  }

  template <typename t>
  gpuify t ViewFactor<t>::getReceiverElementToEmitterSurfaceVF(unsigned int receiver_index) const {
    t receiver_element_area = (*_receiver)[receiver_index].getArea();
    t inverse_receiver_element_area = 1.0 / receiver_element_area;
    unsigned int num_emitter_elements = _emitter->getNumElements();
    t total_view_factor = 0.0;
    for (unsigned int i = 0; i < num_emitter_elements; i++) {
      total_view_factor += (this->getMatrixElementVF(i, receiver_index) * (*_emitter)[i].getArea() * inverse_receiver_element_area);
    }
    return total_view_factor;
  }

  template <typename t>
  gpuify t ViewFactor<t>::getSurfaceToSurfaceAverageVF() const {
    auto num_emitter_elements = _emitter->getNumElements();
    std::vector<t> emitter_view_factors(num_emitter_elements);
    auto emitter_areas = _emitter->getAreas();
    #pragma omp parallel for
    for (int i = 0; i < num_emitter_elements; i++) {
      emitter_view_factors[i] = (this->getEmitterElementToReceiverSurfaceVF(i) * emitter_areas[i]);
    }
    t scale_factor = 1.0 / std::reduce(std::execution::par, emitter_areas.cbegin(), emitter_areas.cend());
    t total_vf = std::reduce(std::execution::par, emitter_view_factors.cbegin(), emitter_view_factors.cend()) * scale_factor;
    return total_vf;
  }

  template <typename t>
  gpuify ViewFactor<t>& ViewFactor<t>::setElements(std::vector<unsigned int> indices, std::vector<t> view_factors) {
    _indices = indices;
    _view_factors = view_factors;
    return *this;
  }

  template class ViewFactor<float>;
  template class ViewFactor<double>;
}