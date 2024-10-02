#ifndef OVF_TRIANGULATION_H_
#define OVF_TRIANGULATION_H_

#include <string>
#include <array>
#include <vector>
#include <algorithm>

#include "../../config.hpp"
#include "Triangle.hpp"

namespace openviewfactor {

enum MeshType { EMITTER, RECEIVER, BLOCKER, UNKNOWN };

template <typename FLOAT_TYPE> class Triangulation {

  private:
    std::vector<Vector3<FLOAT_TYPE>> _pts;  // points list in the triangulation
    std::vector<std::array<size_t, 3>> _con; // connectivity list in the triangulation
    MeshType _name;  // enumeration: EMITTER, RECEIVER, BLOCKER)

  public:
    //* ----- CLASS CONSTRUCTORS ----- *//
    OVF_HOST_DEVICE Triangulation();  // default constructor
    OVF_HOST_DEVICE Triangulation(MeshType name);  // name constructor

    //* ----- ACCESSOR METHODS ----- *//
    OVF_HOST_DEVICE MeshType getName() const;
    OVF_HOST_DEVICE size_t getNumElements() const;
    OVF_HOST_DEVICE size_t getNumPoints() const;
    OVF_HOST_DEVICE size_t getNumBytes() const;

    OVF_HOST_DEVICE const std::array<size_t,3>* getConPtr() const;
    OVF_HOST_DEVICE const Vector3<FLOAT_TYPE>* getPtsPtr() const;
    
    OVF_HOST_DEVICE const Triangle<FLOAT_TYPE> operator[](size_t index) const;
    OVF_HOST_DEVICE Triangle<FLOAT_TYPE> operator[](size_t index);
    
    OVF_HOST_DEVICE FLOAT_TYPE getMeshArea() const;

    OVF_HOST_DEVICE Triangulation getSubMesh(std::vector<unsigned int> indices) const;

    OVF_HOST_DEVICE std::vector<Vector3<FLOAT_TYPE>> getCentroids() const;
    OVF_HOST_DEVICE std::vector<Vector3<FLOAT_TYPE>> getNormals() const;
    OVF_HOST_DEVICE std::vector<Triangle<FLOAT_TYPE>> getTriangles() const;

    //* ----- MUTATOR METHODS ----- *//
    OVF_HOST_DEVICE Triangulation<FLOAT_TYPE>& setName(const MeshType &name);
    OVF_HOST_DEVICE Triangulation<FLOAT_TYPE>& clear();
    OVF_HOST_DEVICE Triangulation<FLOAT_TYPE>& addElement(Vector3<FLOAT_TYPE> OA, Vector3<FLOAT_TYPE> OB, Vector3<FLOAT_TYPE> OC);
    OVF_HOST_DEVICE Triangulation<FLOAT_TYPE>& addElement(Triangle<FLOAT_TYPE> tri);

    OVF_HOST_DEVICE Triangulation<FLOAT_TYPE>& setConnectivity(std::vector<std::array<size_t, 3>> con);
    OVF_HOST_DEVICE Triangulation<FLOAT_TYPE>& setPoints(std::vector<Vector3<FLOAT_TYPE>> pts);

    OVF_HOST_DEVICE bool operator==(const Triangulation<FLOAT_TYPE>& rhs) const;
};
}

#endif