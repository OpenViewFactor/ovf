#include "all_headers.hpp"

#include "geometry.hpp"
#include "solver.hpp"

#pragma once

//! ----- HANDLING RESULTS ----- !//

namespace results {

  template <typename T> class solution {
    public:
    std::vector<std::vector<unsigned int>*>* _e_indices;
    std::vector<std::vector<T>*>* _vf;
    unsigned int _N_e;
    unsigned int _N_r;
  
    //TODO rewrite constructors for new vector formats
    solution() {
      std::vector<std::vector<unsigned int>*> indices;
      _e_indices = &indices;
      std::vector<std::vector<T>*> results;
      _vf = &results;
      _N_e = 0;
      _N_r = 0;
    }
    solution(std::vector<std::vector<unsigned int>*>* e_indices, std::vector<std::vector<T>*>* vf, unsigned int N_e, unsigned int N_r) {
      _e_indices = e_indices;
      _vf = vf;
      _N_e = N_e;
      _N_r = N_r;
    }
  
    T operator[](unsigned int i) {
      unsigned int e_index = i / _N_r;
      unsigned int r_index = i % _N_r;
      std::vector<unsigned int>* e_indices = (*_e_indices)[e_index];
      int index_in_e_indices = binarySearch(e_indices, r_index, 0, e_indices->size());
      if (index_in_e_indices != -1) {
        std::vector<T>* sub_results = (*_vf)[e_index];
        T result = (*sub_results)[index_in_e_indices];
        return ( result );
      }
      return 0.0;
    }

    int binarySearch(std::vector<unsigned int>* v, unsigned int i, unsigned int inclusive_start, unsigned int uninclusive_end) {
      if (uninclusive_end > inclusive_start) {
        int midpoint = (int)((inclusive_start + uninclusive_end) / 2);
        if ( (*v)[midpoint] == i ) { return midpoint; }
        if ( (*v)[midpoint] > i ) { return binarySearch(v, i, inclusive_start, midpoint); }
        return binarySearch(v, i, midpoint + 1, uninclusive_end);
      }
      return -1;
    }
  };
  
  
  template <typename T> T vfElement(solution<T>* s, unsigned int i) {
    return ( (*s)[i] );
  }
  
  template <typename T> T vfElement(solution<T>* s, unsigned int e, unsigned int r) {
    unsigned int i = e * s->_N_r + r;
    return ( (*s)[i] );
  }
  
  template <typename T> T emitterElementVF(solution<T>* s, unsigned int e) {
    T total_vf = 0.0;
    for (int i = 0; i < s->_N_r; i++) {
      total_vf += vfElement(s, e, i);
    }
    return total_vf;
  }

  template <typename T> T receiverElementVF(solution<T>* s, unsigned int r, std::vector<T>* e_areas, T e_area) {
    T total_vf = 0.0;
    for (int i = 0; i < s->_N_e; i++) {
      total_vf += vfElement(s, i, r);
    }
    return total_vf;
  }
  
  template <typename T> T surfaceVF(solution<T>* s, std::vector<T>* e_areas) {
    std::vector<T> element_vf(s->_N_e);
    #pragma omp parallel for
    for (int i = 0; i < s->_N_e; i++) {
      element_vf[i] = emitterElementVF(s, i) * (*e_areas)[i];
    }
    T sum = std::reduce(std::execution::par, element_vf.cbegin(), element_vf.cend());
    T e_area = std::reduce(std::execution::par, (*e_areas).cbegin(), (*e_areas).cend());
    T total_vf = sum / e_area;
    return total_vf;
  }
  
}