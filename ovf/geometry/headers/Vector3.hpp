#ifndef OVF_VECTOR3_H_
#define OVF_VECTOR3_H_

#include "config.hpp"
#include <exception>
#include <cmath>

namespace openviewfactor {
  template <typename FLOAT_TYPE> class Vector3 {

    private:  //* Private Class Attributes (x,y,z)
      FLOAT_TYPE _x;
      FLOAT_TYPE _y;
      FLOAT_TYPE _z;
      
    public:
      //* ----- CLASS CONSTRUCTORS ----- *//
      OVF_HOST_DEVICE Vector3();  // default constructor
      OVF_HOST_DEVICE Vector3(FLOAT_TYPE x, FLOAT_TYPE y, FLOAT_TYPE z);  // (x,y,z) constructor

      //* ----- ACCESSOR METHODS ----- *//
      OVF_HOST_DEVICE FLOAT_TYPE getX() const;  // const method to access _x
      OVF_HOST_DEVICE FLOAT_TYPE getY() const;  // const method to access _y
      OVF_HOST_DEVICE FLOAT_TYPE getZ() const;  // const method to access _z

      OVF_HOST_DEVICE const FLOAT_TYPE operator[](size_t index) const;
      OVF_HOST_DEVICE FLOAT_TYPE operator[](size_t index);

      OVF_HOST_DEVICE unsigned int getLongestDirection() const;

      OVF_HOST_DEVICE Vector3<FLOAT_TYPE> normalize() const;  // const method to return the direction vector
      
      OVF_HOST_DEVICE FLOAT_TYPE getMagnitude() const; // const method to calculate norm
      
      //* ----- MUTATOR METHODS ----- *//
      OVF_HOST_DEVICE Vector3<FLOAT_TYPE>& setX(FLOAT_TYPE x);  // set _x and return it
      OVF_HOST_DEVICE Vector3<FLOAT_TYPE>& setY(FLOAT_TYPE y);  // set _y and return it
      OVF_HOST_DEVICE Vector3<FLOAT_TYPE>& setZ(FLOAT_TYPE z);  // set _z and return it
      OVF_HOST_DEVICE Vector3<FLOAT_TYPE> scale(FLOAT_TYPE s) const;  // scale by a constant
      OVF_HOST_DEVICE Vector3<FLOAT_TYPE> flip() const;

      //* ----- VECTOR OPERATIONS ----- *//
      OVF_HOST_DEVICE FLOAT_TYPE dot(const Vector3<FLOAT_TYPE> &rhs) const; // const method to dot two vectors
      OVF_HOST_DEVICE Vector3<FLOAT_TYPE> cross(const Vector3<FLOAT_TYPE> &rhs) const;  // const method to cross two vectors

      //* ----- OPERATOR OVERLOADS ----- *//
      OVF_HOST_DEVICE Vector3<FLOAT_TYPE> operator+(const Vector3<FLOAT_TYPE> &rhs);  // enable vector addition
      OVF_HOST_DEVICE Vector3<FLOAT_TYPE> operator-(const Vector3<FLOAT_TYPE> &rhs);  // enable vector subtraction
      OVF_HOST_DEVICE bool operator==(const Vector3<FLOAT_TYPE> &rhs);  // enable vector equality comparison
      OVF_HOST_DEVICE bool operator!=(const Vector3<FLOAT_TYPE> &rhs);  // enable vector inequality comparison
  };

  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE Vector3<FLOAT_TYPE> vectorMinima(const Vector3<FLOAT_TYPE> v1, const Vector3<FLOAT_TYPE> v2);
  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE Vector3<FLOAT_TYPE> vectorMaxima(const Vector3<FLOAT_TYPE> v1, const Vector3<FLOAT_TYPE> v2);

  //TODO [] SHANE AND I HAVE NO IDEA WHY THESE EXPLICIT INSTANTIATIONS ARE NECESSARY, BUT THE COMPILER THROWS A TANTRUM IF THEY'RE NOT HERE
  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE Vector3<FLOAT_TYPE> operator+(const Vector3<FLOAT_TYPE> &lhs,
                                                const Vector3<FLOAT_TYPE> &rhs);  // explicit instantiation of addition
  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE Vector3<FLOAT_TYPE> operator-(const Vector3<FLOAT_TYPE> &lhs,
                                                const Vector3<FLOAT_TYPE> &rhs);  // explicit instantiation of subtraction
  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE bool operator==(const Vector3<FLOAT_TYPE> &lhs,
                                  const Vector3<FLOAT_TYPE> &rhs);  // explicit instantiation of equality comparison
  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE bool operator!=(const Vector3<FLOAT_TYPE> &lhs,
                                  const Vector3<FLOAT_TYPE> &rhs);  // explicit instantiation of inequality comparison

  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE FLOAT_TYPE dot(const Vector3<FLOAT_TYPE> &lhs,
                                 const Vector3<FLOAT_TYPE> &rhs);
  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE Vector3<FLOAT_TYPE> cross(const Vector3<FLOAT_TYPE> &lhs,
                                 const Vector3<FLOAT_TYPE> &rhs);
}

#endif