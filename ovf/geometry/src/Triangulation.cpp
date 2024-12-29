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
  OVF_HOST_DEVICE std::shared_ptr<Triangulation<FLOAT_TYPE>> Triangulation<FLOAT_TYPE>::getSubMesh(std::vector<unsigned int> indices) const {    
    auto sub_mesh = std::make_shared<Triangulation<FLOAT_TYPE>>();
    std::vector<std::array<size_t, 3>> sub_mesh_connectivity(indices.size());
    std::generate(sub_mesh_connectivity.begin(), sub_mesh_connectivity.end(), [this, indices] (unsigned int i=0) mutable { return (this->_con)[indices[i++]]; }); 
    sub_mesh->setConnectivity(sub_mesh_connectivity);
    sub_mesh->setPoints(_pts);    
    return sub_mesh;
  }

  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE std::vector<FLOAT_TYPE> Triangulation<FLOAT_TYPE>::getAreas() const {
    std::vector<FLOAT_TYPE> mesh_areas(this->getNumElements());
    for (int i = 0; i < getNumElements(); i++) {
      mesh_areas[i] = (*this)[i].getArea();
    }
    return mesh_areas;
  }

  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE std::vector<Vector3<FLOAT_TYPE>> Triangulation<FLOAT_TYPE>::getCentroids() const {
    std::vector<Vector3<FLOAT_TYPE>> mesh_centroids(this->getNumElements());
    for (int i = 0; i < getNumElements(); i++) {
      mesh_centroids[i] = (*this)[i].getCentroid();
    }
    return mesh_centroids;
  }

  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE std::vector<Vector3<FLOAT_TYPE>> Triangulation<FLOAT_TYPE>::getNormals() const {
    std::vector<Vector3<FLOAT_TYPE>> mesh_normals(this->getNumElements());
    for (int i = 0; i < getNumElements(); i++) {
      mesh_normals[i] = (*this)[i].getNormal();
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
    this->addElement(tri.getOA(), tri.getOB(), tri.getOC());
    return *this;
  }

  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE Triangulation<FLOAT_TYPE>& Triangulation<FLOAT_TYPE>::setConnectivity(std::vector<std::array<size_t, 3>> con) {
    _con.clear();
    _con = std::move(con);
    return *this;
  }
  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE Triangulation<FLOAT_TYPE>& Triangulation<FLOAT_TYPE>::setPoints(std::vector<Vector3<FLOAT_TYPE>> pts) {
    _pts.clear();
    _pts = std::move(pts);
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