#include "all_headers.hpp"

#include "geometry.hpp"
#include "solver.hpp"

#pragma once

//! ----- HANDLING RESULTS ----- !//

namespace results {

  template <typename T> class solution {
    public:
    std::vector<uint>* _i;
    std::vector<T>* _vf;
    uint _N_e;
    uint _N_r;
  
    solution() : _i(std::vector<uint>()), _vf(std::vector<T>()), _N_e(0), _N_r(0) {}
    solution(std::vector<uint>* i, std::vector<T>* vf, uint N_e, uint N_r) {
      _i = i;
      _vf = vf;
      _N_e = N_e;
      _N_r = N_r;
    }
  
    T operator[](uint i) {
      uint index_in_i = binarySearch(i, 0, _i->size());
      if (index_in_i != -1) {
        return ( (*_vf)[ index_in_i ] );
      }
      return 0.0;
    }
  
    int binarySearch(uint i, uint inclusive_start, uint uninclusive_end) {
      if (uninclusive_end > inclusive_start) {
        int midpoint = (int)((inclusive_start + uninclusive_end) / 2);
        if ( (*_i)[midpoint] == i ) { return midpoint; }
        if ( (*_i)[midpoint] > i ) { return binarySearch(i, inclusive_start, midpoint); }
        return binarySearch(i, midpoint + 1, uninclusive_end);
      }
      return -1;
    }
  };
  
  
  template <typename T> T vfElement(solution<T>* s, uint i) {
    return ( (*s)[i] );
  }
  
  template <typename T> T vfElement(solution<T>* s, uint e, uint r, uint N_r) {
    uint i = e * N_r + r;
    return ( (*s)[i] );
  }
  
  template <typename T> T emitterElementVF(solution<T>* s, uint e, uint N_r) {
    T total_vf = 0.0;
    for (int i = 0; i < N_r; i++) {
      total_vf += vfElement(s, e, i, N_r);
    }
    return total_vf;
  }

  template <typename T> T receiverElementVF(solution<T>* s, uint r, uint N_e, uint N_r, std::vector<T>* e_areas, T e_area) {
    T total_vf = 0.0;
    for (int i = 0; i < N_e; i++) {
      // total_vf += vfElement(s, i, r, N_r) * (*e_areas)[i];
      total_vf += vfElement(s, i, r, N_r);
    }
    // total_vf = total_vf / e_area;
    return total_vf;
  }
  
  template <typename T> T surfaceVF(solution<T>* s, std::vector<T>* e_areas, uint N_e, uint N_r) {
    std::vector<T> element_vf(N_e);
    #pragma omp parallel for
    for (int i = 0; i < N_e; i++) {
      element_vf[i] = emitterElementVF(s, i, N_r) * (*e_areas)[i];
    }
    T sum = std::reduce(std::execution::par, element_vf.cbegin(), element_vf.cend());
    T e_area = std::reduce(std::execution::par, (*e_areas).cbegin(), (*e_areas).cend());
    T total_vf = sum / e_area;
    return total_vf;
  }
  
  
  
}