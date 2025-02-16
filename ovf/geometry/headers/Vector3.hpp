#ifndef OVF_VECTOR3_H_
#define OVF_VECTOR3_H_

#include "../../config.hpp"
#include <stdexcept>
#include <array>
#include <cmath>
#include <algorithm>

namespace openviewfactor {
  template <typename t> class Vector3 {

    private:  //* Private Class Attributes (x,y,z)
      std::array<t, 3> _xyz;
    protected:
    public:
      //* ----- CLASS CONSTRUCTORS ----- *//
      gpuify Vector3();  // default constructor
      gpuify Vector3(std::array<t,3> xyz);
      gpuify Vector3(t x, t y, t z);  // (x,y,z) constructor

      //* ----- ACCESSOR METHODS ----- *//
      gpuify t getX() const;  // const method to access _x
      gpuify t getY() const;  // const method to access _y
      gpuify t getZ() const;  // const method to access _z

      gpuify const t operator[](size_t index) const;
      gpuify t operator[](size_t index);

      gpuify unsigned int getLongestDirection() const;

      gpuify Vector3<t> normalize() const;  // const method to return the direction vector
      
      gpuify t getMagnitude() const; // const method to calculate norm
      
      //* ----- MUTATOR METHODS ----- *//
      gpuify Vector3<t>& setX(t x);  // set _x and return it
      gpuify Vector3<t>& setY(t y);  // set _y and return it
      gpuify Vector3<t>& setZ(t z);  // set _z and return it
      gpuify Vector3<t> scale(t s) const;  // scale by a constant
      gpuify Vector3<t> flip() const;

      //* ----- VECTOR OPERATIONS ----- *//
      gpuify t dot(const Vector3<t> &rhs) const; // const method to dot two vectors
      gpuify Vector3<t> cross(const Vector3<t> &rhs) const;  // const method to cross two vectors

      //* ----- OPERATOR OVERLOADS ----- *//
      gpuify Vector3<t> operator+(const Vector3<t> &rhs);  // enable vector addition
      gpuify Vector3<t> operator-(const Vector3<t> &rhs);  // enable vector subtraction
      gpuify bool operator==(const Vector3<t> &rhs);  // enable vector equality comparison
      gpuify bool operator!=(const Vector3<t> &rhs);  // enable vector inequality comparison
  };

  template <typename t>
  gpuify Vector3<t> vectorMinima(const Vector3<t> &v1, const Vector3<t> &v2);
  template <typename t>
  gpuify Vector3<t> vectorMaxima(const Vector3<t> &v1, const Vector3<t> &v2);

  template <typename t>
  gpuify Vector3<t> operator+(const Vector3<t> &lhs,
                                                const Vector3<t> &rhs);  // explicit instantiation of addition
  template <typename t>
  gpuify Vector3<t> operator-(const Vector3<t> &lhs,
                                                const Vector3<t> &rhs);  // explicit instantiation of subtraction
  template <typename t>
  gpuify bool operator==(const Vector3<t> &lhs,
                                  const Vector3<t> &rhs);  // explicit instantiation of equality comparison
  template <typename t>
  gpuify bool operator!=(const Vector3<t> &lhs,
                                  const Vector3<t> &rhs);  // explicit instantiation of inequality comparison

  template <typename t>
  gpuify t dot(const Vector3<t> &lhs,
                                 const Vector3<t> &rhs);
  template <typename t>
  gpuify Vector3<t> cross(const Vector3<t> &lhs,
                                 const Vector3<t> &rhs);
}

#endif