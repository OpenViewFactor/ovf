#ifndef OVF_TRIANGULATION_H_
#define OVF_TRIANGULATION_H_

#include <string>
#include <array>
#include <vector>
#include <algorithm>

#include "geometry/include/Triangle.hpp"

namespace openviewfactor {

template <typename FLOAT_TYPE> class Triangulation {

  using V3 = Vector3<FLOAT_TYPE>;
  using T = Triangle<FLOAT_TYPE>;
  using TGLN = Triangulation<FLOAT_TYPE>;

  private:
    std::vector<V3> _pts;  // points list in the triangulation
    std::vector<std::array<size_t, 3>> _con; // connectivity list in the triangulation
    std::string _name;  // mesh name (i assume this was EMITTER, RECEIVER, BLOCKER)

  public:
    //* ----- CLASS CONSTRUCTORS ----- *//
    OVF_HOST_DEVICE Triangulation();  // default constructor
    OVF_HOST_DEVICE Triangulation(const std::string &name);  // name constructor

    //* ----- ACCESSOR METHODS ----- *//
    OVF_HOST_DEVICE std::string getName() const;
    OVF_HOST_DEVICE size_t numElem() const;
    OVF_HOST_DEVICE size_t numPts() const;
    OVF_HOST_DEVICE size_t numBytes() const;
    OVF_HOST_DEVICE std::vector<std::array<size_t, 3>>& getConPtr() const;
    OVF_HOST_DEVICE std::vector<V3>& getPtsPtr() const;
    OVF_HOST_DEVICE const T operator[](size_t index) const;
    OVF_HOST_DEVICE T operator[](size_t index);
    OVF_HOST_DEVICE FLOAT_TYPE area() const;

    //* ----- MUTATOR METHODS ----- *//
    OVF_HOST_DEVICE TGLN& setName(const std::string &name);
    OVF_HOST_DEVICE TGLN& clear();
    OVF_HOST_DEVICE TGLN& addElem(V3 OA, V3 OB, V3 OC);
    OVF_HOST_DEVICE TGLN& addElem(T tri);
};
}

#endif