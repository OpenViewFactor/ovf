#include "geometry/include/Triangulation.hpp"

namespace openviewfactor {
  //* ----- CLASS CONSTRUCTORS ----- *//
  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE Triangulation<FLOAT_TYPE>::Triangulation()
    : _pts(std::vector<Vector3<FLOAT_TYPE>>()), _con(std::vector<std::array<size_t, 3>>>()), _name(UNKNOWN) {}

  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE Triangulation<FLOAT_TYPE>::Triangulation(MeshType name)
    : _pts(std::vector<Vector3<FLOAT_TYPE>>()), _con(std::vector<std::array<size_t, 3>>>()), _name(name) {}

  //* ----- ACCESSOR METHODS ----- *//
  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE MeshType Triangulation<FLOAT_TYPE>::getName() const { return _name; }
  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE size_t Triangulation<FLOAT_TYPE>::getNumElements() const { return _con.size(); }
  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE size_t Triangulation<FLOAT_TYPE>::getNumPoints() const { return _pts.size(); }
  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE size_t Triangulation<FLOAT_TYPE>::getNumBytes() const { return (_con.size() * sizeof(size_t) * 3) + (_pts.size() * sizeof(FLOAT_TYPE) * 9); }

  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE std::vector<std::array<size_t, 3>>& Triangulation<FLOAT_TYPE>::getConPtr() const { return _con.data(); }

  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE std::vector<Vector3<FLOAT_TYPE>>& Triangulation<FLOAT_TYPE>::getPtsPtr() const { return _pts.data(); }

  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE const Triangle<FLOAT_TYPE> Triangulation<FLOAT_TYPE>::operator[](size_t index) const {
    return (*this)[index];
  }
  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE Triangle<FLOAT_TYPE> Triangulation<FLOAT_TYPE>::operator[](size_t index) {
    Triangle<FLOAT_TYPE> T(_pos[_con[index][0]], _pos[_con[index][1]], _pos[_con[index][2]]);
    return T;
  }

  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE FLOAT_TYPE Triangulation<FLOAT_TYPE>::getMeshArea() const {
    FLOAT_TYPE total_area = 0;
    for (int i = 0; i < getNumElements(); i++) {
      total_area += (*this)[i].area();
    }
    return total_area;
  }

  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE Triangulation<FLOAT_TYPE> Triangulation<FLOAT_TYPE>::getSubMesh(std::vector<unsigned int> indices) const {
    Triangulation sub_mesh;
    std::vector<std::array<size_t, 3>> sub_connectivity;
    for (unsigned int index : indices) {
      sub_connectivity.push_back(_con[index]);
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
    std::vector<triangle<FLOAT_TYPE>> mesh_triangles;
    for (std::array<size_t, 3> connections : _con) {
      mesh_triangles.push_back(Triangle<FLOAT_TYPE>(_pts[connections[0]], _pts[connections[1]], _pts[connections[2]]));
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
    return *this;
  }
  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE Triangulation<FLOAT_TYPE>& Triangulation<FLOAT_TYPE>::addElement(Triangle<FLOAT_TYPE> tri) {
    addElement(tri.getOA(), tri.getOB(), tri.getOC());
    return *this;
  }

  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE Triangulation<FLOAT_TYPE>& Triangulation<FLOAT_TYPE>::setConnectivity(std::vector<std::array<size_t, 3>> con) {
    _con = con;
    return *this;
  }
  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE Triangulation<FLOAT_TYPE>& Triangulation<FLOAT_TYPE>::setPoints(std::vector<Vector3<FLOAT_TYPE>> pts) {
    _pts = pts;
    return *this;
  }

template class Triangulation<float>;
template class Triangulation<double>;
}