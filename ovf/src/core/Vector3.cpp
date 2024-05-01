#include "ovf/core/Vector3.hpp"
#include <cmath>

namespace openviewfactor {
  //* ----- CLASS CONSTRUCTORS ----- *//
  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE Vector3<FLOAT_TYPE>::Vector3()
    : _x(0), _y(0), _z(0) {} // default to 0 vector
  
  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE Vector3<FLOAT_TYPE>::Vector3(FLOAT_TYPE x, FLOAT_TYPE y, FLOAT_TYPE z)
    : _x(x), _y(y), _z(z) {}

  //* ----- ACCESSOR METHODS ----- *//
  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE FLOAT_TYPE Vector3<FLOAT_TYPE>::getX() const { return _x; }
  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE FLOAT_TYPE Vector3<FLOAT_TYPE>::getY() const { return _y; }
  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE FLOAT_TYPE Vector3<FLOAT_TYPE>::getZ() const { return _z; }

  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE FLOAT_TYPE Vector3<FLOAT_TYPE>::getMagnitude() const {
    return sqrt(_x*_x + _y*_y + _z*_z);
  }

  //* ----- MUTATOR METHODS ----- *//
  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE Vector3<FLOAT_TYPE>& Vector3<FLOAT_TYPE>::setX(FLOAT_TYPE x) {
    _x = x;
    return *this;
  }
  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE Vector3<FLOAT_TYPE>& Vector3<FLOAT_TYPE>::setY(FLOAT_TYPE y) {
    _y = y;
    return *this;
  }
  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE Vector3<FLOAT_TYPE>& Vector3<FLOAT_TYPE>::setZ(FLOAT_TYPE z) {
    _z = z;
    return *this;
  }
  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE Vector3<FLOAT_TYPE> Vector3<FLOAT_TYPE>::scale(FLOAT_TYPE s) const {
    Vector3<FLOAT_TYPE> scaled_vector(_x * s, _y * s, _z * s);
    return scaled_vector;
  }

  //* ----- VECTOR OPERATIONS ----- *//
  // ! openviewfactor namespace dot product function
  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE FLOAT_TYPE dot(const Vector3<FLOAT_TYPE> &lhs,
                                 const Vector3<FLOAT_TYPE> &rhs) {
    return (lhs.getX() * rhs.getX() +
            lhs.getY() * rhs.getY() +
            lhs.getZ() * rhs.getZ());
  }
  // ! Vector3 namespace dot product function
  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE FLOAT_TYPE Vector3<FLOAT_TYPE>::dot(const Vector3<FLOAT_TYPE> &rhs) const {
    return openviewfactor::dot(*this, &rhs);
  }

  // ! openviewfactor namespace cross product function
  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE FLOAT_TYPE cross(const Vector3<FLOAT_TYPE> &lhs,
                                   const Vector3<FLOAT_TYPE> &rhs) {
    Vector3<FLOAT_TYPE> crossed_vector = {
      lhs.getY() * rhs.getZ() - lhs.getZ() * rhs.getY(),
      lhs.getZ() * rhs.getX() - lhs.getX() * rhs.getZ(),
      lhs.getX() * rhs.getY() - lhs.getY() * rhs.getX()
    };
    return crossed_vector;
  }
  // ! Vector3 namespace cross product function
  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE Vector3<FLOAT_TYPE> Vector3<FLOAT_TYPE>::cross(const Vector3<FLOAT_TYPE> &rhs) const {
    return openviewfactor::cross(*this, &rhs);
  }

  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE Vector3<FLOAT_TYPE> Vector3<FLOAT_TYPE>::normalize() const {
    FLOAT_TYPE magnitude = getMagnitude();
    Vector3<FLOAT_TYPE> normalized_vector = {
      _x / magnitude,
      _y / magnitude,
      _z / magnitude
    };
    return normalized_vector;
  }

  //* ----- OPERATOR OVERLOADS ----- *//
  // ! openviewfactor namespace addition operator overload
  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE Vector3<FLOAT_TYPE> operator+(const Vector3<FLOAT_TYPE> &rhs) {
    Vector3<FLOAT_TYPE> vector_sum = {
      lhs._x + rhs._x,
      lhs._y + rhs._y,
      lhs._z + rhs._z
    };
    return vector_sum;
  }
  // ! Vector3 namespace addition operator overload
  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE Vector3<FLOAT_TYPE> Vector3<FLOAT_TYPE>::operator+(const Vector3<FLOAT_TYPE> &rhs) {
    Vector3<FLOAT_TYPE> vector_sum = {
      _x + rhs._x,
      _y + rhs._y,
      _z + rhs._z
    };
    return vector_sum;
  }

  // ! openviewfactor namespace subtraction operator overload
  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE Vector3<FLOAT_TYPE> operator-(const Vector3<FLOAT_TYPE> &rhs) {
    Vector3<FLOAT_TYPE> vector_diff = {
      lhs._x + rhs._x,
      lhs._y + rhs._y,
      lhs._z + rhs._z
    };
    return vector_diff;
  }
  // ! Vector3 namespace subtraction operator overload
  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE Vector3<FLOAT_TYPE> Vector3<FLOAT_TYPE>::operator-(const Vector3<FLOAT_TYPE> &rhs) {
    Vector3<FLOAT_TYPE> vector_diff = {
      _x + rhs._x,
      _y + rhs._y,
      _z + rhs._z
    };
    return vector_diff;
  }

  // ! openviewfactor namespace equality operator overload
  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE bool operator==(const Vector3<FLOAT_TYPE> &lhs,
                                  const Vector3<FLOAT_TYPE> &rhs) {
    return lhs == rhs;
  }
  // ! Vector3 namespace equality operator overload
  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE bool Vector3<FLOAT_TYPE>::operator==(const Vector3<FLOAT_TYPE> &rhs) {
    return (*this == rhs);
  }

  // ! openviewfactor namespace inequality operator overload
  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE bool operator!=(const Vector3<FLOAT_TYPE> &lhs,
                                  const Vector3<FLOAT_TYPE> &rhs) {
    return !(lhs == rhs);
  }
  // ! Vector3 namespace inequality operator overload
  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE bool Vector3<FLOAT_TYPE>::operator!=(const Vector3<FLOAT_TYPE> &rhs) {
    return !(*this == rhs);
  }

template class Vector3<float>;
template class Vector3<double>;

template bool operator==(const Vector3<float> &, const Vector3<float> &);
template bool operator==(const Vector3<double> &, const Vector3<double> &);
template bool operator!=(const Vector3<float> &, const Vector3<float> &);
template bool operator!=(const Vector3<double> &, const Vector3<double> &);

template Vector3<float> operator+(const Vector3<float> &, const Vector3<float> &);
template Vector3<double> operator+(const Vector3<double> &, const Vector3<double> &);
template Vector3<float> operator-(const Vector3<float> &, const Vector3<float> &);
template Vector3<double> operator-(const Vector3<double> &, const Vector3<double> &);

template float dot(const Vector3<float> &, const Vector3<float> &);
template double dot(const Vector3<double> &, const Vector3<double> &);
template Vector3<float> cross(const Vector3<float> &, const Vector3<float> &);
template Vector3<double> cross(const Vector3<double> &, const Vector3<double> &);
}