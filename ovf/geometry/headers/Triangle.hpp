#ifndef OVF_TRIANGLE_H_
#define OVF_TRIANGLE_H_

#include "../../config.hpp"
#include "Vector3.hpp"
#include <cmath>
#include <vector>
#include <algorithm>

namespace openviewfactor {

template <typename t> class Triangle {

  private:
    std::array<Vector3<t>,3> _pts;
  protected:public:
    //* ----- CLASS CONSTRUCTORS ----- *//
    gpuify Triangle();
    gpuify Triangle(std::array<Vector3<t>,3> pts);
    gpuify Triangle(std::array<std::array<t,3>,3> pts);
    gpuify Triangle(Vector3<t> OA, Vector3<t> OB, Vector3<t> OC);    

    //* ----- ACCESSOR METHODS ----- *//
    gpuify Vector3<t> getOA() const;
    gpuify Vector3<t> getOB() const;
    gpuify Vector3<t> getOC() const;
    gpuify Vector3<t> getAB() const;
    gpuify Vector3<t> getBC() const;
    gpuify Vector3<t> getCA() const;
    gpuify Vector3<t> getNormal() const;
    gpuify Vector3<t> getCentroid() const;
    gpuify t getArea() const;

    gpuify t evaluateAspectRatio() const;
    gpuify t evaluateSkewness() const;
    gpuify t evaluateElementQuality() const;

    //* ----- MUTATOR METHODS ----- *//
    gpuify Triangle<t>& setOA(t x, t y, t z);
    gpuify Triangle<t>& setOA(Vector3<t> v);
    gpuify Triangle<t>& setOB(t x, t y, t z);
    gpuify Triangle<t>& setOB(Vector3<t> v);
    gpuify Triangle<t>& setOC(t x, t y, t z);
    gpuify Triangle<t>& setOC(Vector3<t> v);

    //* ----- OPERATOR OVERLOADS ----- *//
    gpuify bool operator==(const Triangle<t> &rhs) const;
    gpuify bool operator!=(const Triangle<t> &rhs) const;
    gpuify const Vector3<t> operator[](size_t index) const;
    gpuify Vector3<t> operator[](size_t index);
};

template <typename t>
bool operator==(const Triangle<t> &lhs,
                const Triangle<t> &rhs);
template <typename t>
bool operator!=(const Triangle<t> &lhs,
                const Triangle<t> &rhs);
}

#endif