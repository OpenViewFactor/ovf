#include "geometry/include/Triangulation.hpp"

namespace openviewfactor {
  //* ----- CLASS CONSTRUCTORS ----- *//
  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE Triangulation<FLOAT_TYPE>::Triangulation()
    : _pts(std::vector<Vector3<FLOAT_TYPE>>()), _con(std::vector<std::array<size_t, 3>>>()), _name(std::string("UNKNOWN")) {}

  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE Triangulation<FLOAT_TYPE>::Triangulation(const std::string &name)
    : _pts(std::vector<Vector3<FLOAT_TYPE>>()), _con(std::vector<std::array<size_t, 3>>>()), _name(name) {}

  //* ----- ACCESSOR METHODS ----- *//
  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE std::string Triangulation<FLOAT_TYPE>::getName() const { return _name; }
  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE size_t Triangulation<FLOAT_TYPE>::getElem() const { return _con.size(); }
  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE size_t Triangulation<FLOAT_TYPE>::numPts() const { return _pts.size(); }
  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE size_t Triangulation<FLOAT_TYPE>::numBytes() const { return (_con.size() * sizeof(size_t) * 3) + (_pts.size() * sizeof(FLOAT_TYPE) * 9); }
  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE std::vector<std::array<size_t, 3>>& Triangulation<FLOAT_TYPE>::getConPtr() const { return _con.data(); }
  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE std::vector<Vector3<FLOAT_TYPE>>& Triangulation<FLOAT_TYPE>::getPtsPtr() const { return _pts.data(); }
  
  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE const Triangle<FLOAT_TYPE> Triangulation<FLOAT_TYPE>::operator[](size_t index) const {
    Triangle<FLOAT_TYPE> T(_pos[_con[index][0]], _pos[_con[index][1]], _pos[_con[index][2]]);
    return T;
  }
  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE Triangle<FLOAT_TYPE> Triangulation<FLOAT_TYPE>::operator[](size_t index) {
    Triangle<FLOAT_TYPE> T(_pos[_con[index][0]], _pos[_con[index][1]], _pos[_con[index][2]]);
    return T;
  }

  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE FLOAT_TYPE Triangulation<FLOAT_TYPE>::area() const {
    FLOAT_TYPE total_area = 0;
    for (int i = 0; i < numElem(); i++) {
      total_area += (*this)[i].area();
    }
    return total_area;
  }

  //* ----- MUTATOR METHODS ----- *//
  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE Triangulation<FLOAT_TYPE>& Triangulation<FLOAT_TYPE>::setName(const std::string &name) {
    _name = name;
    return *this;
  }
  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE Triangulation<FLOAT_TYPE>& Triangulation<FLOAT_TYPE>::clear() {
    _pts.clear();
    _con.clear();
    return *this;
  }
  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE Triangulation<FLOAT_TYPE>& Triangulation<FLOAT_TYPE>::addElem(Vector3<FLOAT_TYPE> OA, Vector3<FLOAT_TYPE> OB, Vector3<FLOAT_TYPE> OC) {
    std::vector<Vector3<FLOAT_TYPE>> pos = {OA, OB, OC};
    std::array<size_t, 3> con;
    for (size_t i = 0; i < pos.size(); i++) {
      auto pos_location_iterator = std::find(_pos.begin(), _pos.end(), pos[i]);
      if (pos_location_iterator != _pos.end()) {
        con[i] = pos_location_iterator - _pos.begin();
      } else {
        _pos.push_back(p[i]);
        con[i] = _pos[_pos.size() - 1];
      }
    }
    _con.push_back(con);
  }
  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE Triangulation<FLOAT_TYPE>& Triangulation<FLOAT_TYPE>::addElem(Triangle<FLOAT_TYPE> tri) {
    addElem(tri.getOA(), tri.getOB(), tri.getOC());
    return *this;
  }

template class Triangulation<float>;
template class Triangulation<double>;
}