#ifndef OVF_TRIANGLE_H_
#define OVF_TRIANGLE_H_

#include "../../config.hpp"
#include "Vector3.hpp"
#include <cmath>

namespace openviewfactor {

template <typename FLOAT_TYPE> class Triangle {

  private:
    std::array<Vector3<FLOAT_TYPE>,3> _pts;
  protected:
  public:
    //* ----- CLASS CONSTRUCTORS ----- *//
    OVF_HOST_DEVICE Triangle();
    OVF_HOST_DEVICE Triangle(std::array<Vector3<FLOAT_TYPE>,3> pts);
    OVF_HOST_DEVICE Triangle(Vector3<FLOAT_TYPE> OA, Vector3<FLOAT_TYPE> OB, Vector3<FLOAT_TYPE> OC);    

    //* ----- ACCESSOR METHODS ----- *//
    OVF_HOST_DEVICE const Vector3<FLOAT_TYPE> getOA() const;
    OVF_HOST_DEVICE const Vector3<FLOAT_TYPE> getOB() const;
    OVF_HOST_DEVICE const Vector3<FLOAT_TYPE> getOC() const;
    OVF_HOST_DEVICE const Vector3<FLOAT_TYPE> getAB() const;
    OVF_HOST_DEVICE const Vector3<FLOAT_TYPE> getBC() const;
    OVF_HOST_DEVICE const Vector3<FLOAT_TYPE> getCA() const;
    OVF_HOST_DEVICE const Vector3<FLOAT_TYPE> getNormal() const;
    OVF_HOST_DEVICE const Vector3<FLOAT_TYPE> getCentroid() const;
    OVF_HOST_DEVICE const FLOAT_TYPE getArea() const;

    //* ----- MUTATOR METHODS ----- *//
    OVF_HOST_DEVICE Triangle<FLOAT_TYPE>& setOA(FLOAT_TYPE x, FLOAT_TYPE y, FLOAT_TYPE z);
    OVF_HOST_DEVICE Triangle<FLOAT_TYPE>& setOA(Vector3<FLOAT_TYPE> v);
    OVF_HOST_DEVICE Triangle<FLOAT_TYPE>& setOB(FLOAT_TYPE x, FLOAT_TYPE y, FLOAT_TYPE z);
    OVF_HOST_DEVICE Triangle<FLOAT_TYPE>& setOB(Vector3<FLOAT_TYPE> v);
    OVF_HOST_DEVICE Triangle<FLOAT_TYPE>& setOC(FLOAT_TYPE x, FLOAT_TYPE y, FLOAT_TYPE z);
    OVF_HOST_DEVICE Triangle<FLOAT_TYPE>& setOC(Vector3<FLOAT_TYPE> v);

    //* ----- OPERATOR OVERLOADS ----- *//
    OVF_HOST_DEVICE bool operator==(const Triangle<FLOAT_TYPE> &rhs) const;
    OVF_HOST_DEVICE bool operator!=(const Triangle<FLOAT_TYPE> &rhs) const;
};

template <typename FLOAT_TYPE>
bool operator==(const Triangle<FLOAT_TYPE> &lhs,
                const Triangle<FLOAT_TYPE> &rhs);
template <typename FLOAT_TYPE>
bool operator!=(const Triangle<FLOAT_TYPE> &lhs,
                const Triangle<FLOAT_TYPE> &rhs);
}

#endif