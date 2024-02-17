#ifndef OVF_TRIANGLE_H_
#define OVF_TRIANGLE_H_

#include "ovf/core/config.hpp"
#include "ovf/core/Vector3.hpp"

namespace openviewfactor {

template <typename FLOAT_TYPE> class Triangle {

  using V3 = Vector3<FLOAT_TYPE>;
  using T = Triangle<FLOAT_TYPE>;

  private:
    V3 _OA;
    V3 _OB;
    V3 _OC;
    V3 _n;
  public:
    //* ----- CLASS CONSTRUCTORS ----- *//
    OVF_HOST_DEVICE Triangle();
    OVF_HOST_DEVICE Triangle(V3 OA, V3 OB, V3 OC);    

    //* ----- ACCESSOR METHODS ----- *//
    OVF_HOST_DEVICE const V3& getOA() const;
    OVF_HOST_DEVICE const V3& getOB() const;
    OVF_HOST_DEVICE const V3& getOC() const;
    OVF_HOST_DEVICE const V3& normal() const;
    OVF_HOST_DEVICE const V3& centroid() const;
    OVF_HOST_DEVICE const V3& getAB() const;
    OVF_HOST_DEVICE const V3& getBC() const;
    OVF_HOST_DEVICE const V3& getCA() const;
    OVF_HOST_DEVICE const FLOAT_TYPE area() const;
    OVF_HOST_DEVICE T flip() const; //TODO [] NOT SURE THIS IS NECESSARY

    //* ----- MUTATOR METHODS ----- *//
    OVF_HOST_DEVICE T& flipInPlace(); //TODO [] NOT SURE THIS IS NECESSARY
    OVF_HOST_DEVICE V3& setOA(FLOAT_TYPE x, FLOAT_TYPE y, FLOAT_TYPE z);
    OVF_HOST_DEVICE V3& setOB(FLOAT_TYPE x, FLOAT_TYPE y, FLOAT_TYPE z);
    OVF_HOST_DEVICE V3& setOC(FLOAT_TYPE x, FLOAT_TYPE y, FLOAT_TYPE z);

    //* ----- OPERATOR OVERLOADS ----- *//
    OVF_HOST_DEVICE bool operator==(const T &rhs) const;
    OVF_HOST_DEVICE bool operator!=(const T &rhs) const;
};
}

#endif