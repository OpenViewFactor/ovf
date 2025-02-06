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
    if (indices_index != -1 && indices_index < _indices.size()) {
      //TODO THE SECOND ARGUMENT IN THIS IF STATEMENT IS BOGUS AND IT SHOULD NOT EXIST
      if (_indices[indices_index] >= 0 && _indices[indices_index] < _view_factors.size()) {
        //TODO THIS IF STATEMENT IS BOGUS AND IT SHOULD NOT EXIST
        FLOAT_TYPE fij = _view_factors[_indices[indices_index]];
        return fij;
      } else {
        return 0.0;
      }
    }
    return 0.0;
  }
  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE FLOAT_TYPE ViewFactor<FLOAT_TYPE>::getMatrixElementVF(unsigned int emitter_index, unsigned int receiver_index) const {
    unsigned int full_matrix_index = (emitter_index * _emitter->getNumElements()) + receiver_index;
    unsigned int maximum_index = _emitter->getNumElements() * _receiver->getNumElements() - 1;
    if (full_matrix_index > maximum_index) { throw std::runtime_error("Cannot access a value for this index, it exceeds the matrix size"); }

    return (this->getMatrixElementVF(full_matrix_index));
  }

  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE FLOAT_TYPE ViewFactor<FLOAT_TYPE>::getEmitterElementToReceiverSurfaceVF(unsigned int emitter_index) const {
    unsigned int num_receiver_elements = _receiver->getNumElements();
    std::vector<FLOAT_TYPE> individual_view_factors(num_receiver_elements);
    for (unsigned int i = 0; i < num_receiver_elements; i++) {
      individual_view_factors[i] = this->getMatrixElementVF(emitter_index, i);
    }
    return std::reduce(individual_view_factors.cbegin(), individual_view_factors.cend());
  }

  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE FLOAT_TYPE ViewFactor<FLOAT_TYPE>::getReceiverElementToEmitterSurfaceVF(unsigned int receiver_index) const {
    FLOAT_TYPE receiver_element_area = (*_receiver)[receiver_index].getArea();
    unsigned int num_emitter_elements = _emitter->getNumElements();
    std::vector<FLOAT_TYPE> individual_view_factors(num_emitter_elements);
    for (unsigned int i = 0; i < num_emitter_elements; i++) {
      individual_view_factors[i] = (this->getMatrixElementVF(i, receiver_index) * (*_emitter)[i].getArea() / receiver_element_area);
    }

    return std::reduce(individual_view_factors.cbegin(), individual_view_factors.cend());
  }

  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE FLOAT_TYPE ViewFactor<FLOAT_TYPE>::getSurfaceToSurfaceAverageVF() const {
    // std::cout << "test 1" << '\n';
    auto num_emitter_elements = _emitter->getNumElements();
    // std::cout << "test 2" << '\n';
    std::vector<FLOAT_TYPE> emitter_view_factors(num_emitter_elements);
    // std::cout << "test 3" << '\n';
    auto emitter_areas = _emitter->getAreas();
    // std::cout << "test 4" << '\n';
    // #pragma omp parallel for
    for (int i = 0; i < num_emitter_elements; i++) {
      // std::cout << i << " ";
      emitter_view_factors[i] = (this->getEmitterElementToReceiverSurfaceVF(i) * emitter_areas[i]);
    }
    // std::cout << "\ntest 5" << '\n';
    FLOAT_TYPE scale_factor = 1.0 / _emitter->getMeshArea();
    // std::cout << "test 6" << '\n';
    FLOAT_TYPE total_vf = std::reduce(std::execution::par, emitter_view_factors.cbegin(), emitter_view_factors.cend()) * scale_factor;
    // std::cout << "test 7" << '\n';
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