#include "Vector3.hpp"
#include "Triangle.hpp"
#include "Triangulation.hpp"

#include <iostream>

namespace openviewfactor {
  //* ----- CLASS CONSTRUCTORS ----- *//
  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE Triangulation<FLOAT_TYPE>::Triangulation()
    : _pts(std::vector<Vector3<FLOAT_TYPE>>()), _con(std::vector<std::array<size_t, 3>>()), _name(UNKNOWN) {}

  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE Triangulation<FLOAT_TYPE>::Triangulation(MeshType name)
    : _pts(std::vector<Vector3<FLOAT_TYPE>>()), _con(std::vector<std::array<size_t, 3>>()), _name(name) {}

  //* ----- ACCESSOR METHODS ----- *//
  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE MeshType Triangulation<FLOAT_TYPE>::getName() const { return _name; }
  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE size_t Triangulation<FLOAT_TYPE>::getNumElements() const { return _con.size(); }
  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE size_t Triangulation<FLOAT_TYPE>::getNumPoints() const { return _pts.size(); }
  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE size_t Triangulation<FLOAT_TYPE>::getNumBytes() const { return (_con.size() * sizeof(size_t) * 3) + (_pts.size() * sizeof(Vector3<FLOAT_TYPE>)); }

  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE const std::array<size_t,3>* Triangulation<FLOAT_TYPE>::getConPtr() const { return _con.data(); }

  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE const Vector3<FLOAT_TYPE>* Triangulation<FLOAT_TYPE>::getPtsPtr() const { return _pts.data(); }

  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE const Triangle<FLOAT_TYPE> Triangulation<FLOAT_TYPE>::operator[](size_t index) const {
    Triangle<FLOAT_TYPE> T(_pts[ (_con[index])[0] ], _pts[ (_con[index])[1] ], _pts[ (_con[index])[2] ]);
    return T;
  }
  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE Triangle<FLOAT_TYPE> Triangulation<FLOAT_TYPE>::operator[](size_t index) {
    Triangle<FLOAT_TYPE> T(_pts[ (_con[index])[0] ], _pts[ (_con[index])[1] ], _pts[ (_con[index])[2] ]);
    return T;
  }

  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE FLOAT_TYPE Triangulation<FLOAT_TYPE>::getMeshArea() const {
    FLOAT_TYPE total_area = 0;
    for (size_t i = 0; i < this->getNumElements(); i++) {
      total_area += ((*this)[i]).getArea();
    }
    return total_area;
  }

  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE Triangulation<FLOAT_TYPE> Triangulation<FLOAT_TYPE>::getSubMesh(std::vector<unsigned int> indices) const {
    Triangulation sub_mesh;
    std::vector<std::array<size_t, 3>> sub_connectivity(indices.size());
    for (unsigned int i = 0; i < indices.size(); i++) {
      unsigned int index = indices[i];
      sub_connectivity[i] = {_con[index][0], _con[index][1], _con[index][2]};
    }
    sub_mesh.setConnectivity(sub_connectivity);
    sub_mesh.setPoints(_pts);
    return sub_mesh;
  }

  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE std::vector<Vector3<FLOAT_TYPE>> Triangulation<FLOAT_TYPE>::getCentroids() const {
    std::vector<Vector3<FLOAT_TYPE>> mesh_centroids;
    for (int i = 0; i < getNumElements(); i++) {
      mesh_centroids.push_back((*this)[i].getCentroid());
    }
    return mesh_centroids;
  }

  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE std::vector<Vector3<FLOAT_TYPE>> Triangulation<FLOAT_TYPE>::getNormals() const {
    std::vector<Vector3<FLOAT_TYPE>> mesh_normals;
    for (int i = 0; i < getNumElements(); i++) {
      mesh_normals.push_back((*this)[i].getNormal());
    }
    return mesh_normals;
  }

  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE std::vector<Triangle<FLOAT_TYPE>> Triangulation<FLOAT_TYPE>::getTriangles() const {
    std::vector<Triangle<FLOAT_TYPE>> mesh_triangles(this->getNumElements());
    for (unsigned int i = 0; i < this->getNumElements(); i++) {
      std::array<size_t, 3> connections = _con[i];
      mesh_triangles[i].setOA(_pts[connections[0]]).setOB(_pts[connections[1]]).setOC(_pts[connections[2]]);
    }
    return mesh_triangles;
  }

  //* ----- MUTATOR METHODS ----- *//
  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE Triangulation<FLOAT_TYPE>& Triangulation<FLOAT_TYPE>::setName(const MeshType &name) {
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
  OVF_HOST_DEVICE Triangulation<FLOAT_TYPE>& Triangulation<FLOAT_TYPE>::addElement(Vector3<FLOAT_TYPE> OA, Vector3<FLOAT_TYPE> OB, Vector3<FLOAT_TYPE> OC) {
    std::array<Vector3<FLOAT_TYPE>, 3> new_points = {OA, OB, OC};
    std::array<size_t, 3> new_connectivity;
    for (size_t i = 0; i < 3; i++) {
      auto point_location_iterator = std::find(_pts.begin(), _pts.end(), new_points[i]);
      if (point_location_iterator != _pts.end()) {
        new_connectivity[i] = std::distance(_pts.begin(), point_location_iterator);
      } else {
        _pts.push_back(new_points[i]);
        new_connectivity[i] = _pts.size() - 1;
      }
    }
    _con.push_back(new_connectivity);
    return *this;
  }
  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE Triangulation<FLOAT_TYPE>& Triangulation<FLOAT_TYPE>::addElement(Triangle<FLOAT_TYPE> tri) {
    addElement(tri.getOA(), tri.getOB(), tri.getOC());
    return *this;
  }

  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE Triangulation<FLOAT_TYPE>& Triangulation<FLOAT_TYPE>::setConnectivity(std::vector<std::array<size_t, 3>> con) {
    _con.clear();
    for (std::array<size_t, 3> c : con) {
      _con.push_back(c);
    }
    return *this;
  }
  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE Triangulation<FLOAT_TYPE>& Triangulation<FLOAT_TYPE>::setPoints(std::vector<Vector3<FLOAT_TYPE>> pts) {
    _pts.clear();
    for (Vector3<FLOAT_TYPE> pt : pts) {
      _pts.push_back(pt);
    }
    return *this;
  }

  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE bool Triangulation<FLOAT_TYPE>::operator==(const Triangulation<FLOAT_TYPE>& rhs) const {
    if (this->getNumElements() != rhs.getNumElements()) { return false; }
    if (this->getNumPoints() != rhs.getNumPoints()) { return false; }
    if (this->getMeshArea() != rhs.getMeshArea()) { return false; }
    for (size_t i = 0; i < this->getNumElements(); i++) {
      if ((*this)[i] != rhs[i]) { return false; }
    }
    return true;
  }

template class Triangulation<float>;
template class Triangulation<double>;
}