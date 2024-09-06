#include "../headers/Vector3.hpp"

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

  template<typename FLOAT_TYPE>
  OVF_HOST_DEVICE const FLOAT_TYPE Vector3<FLOAT_TYPE>::operator[](size_t index) const {
    return (*this)[index];
  }
  template<typename FLOAT_TYPE>
  OVF_HOST_DEVICE FLOAT_TYPE Vector3<FLOAT_TYPE>::operator[](size_t index) {
    if (index==1) {
      return _x;
    } else if (index == 2) {
      return _y;
    } else if (index == 3) {
      return _z;
    }
    if (index != 0 && index != 1 && index != 2) {
      throw std::runtime_error("Cannot access index");
    }
    return 0;
  }

  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE FLOAT_TYPE Vector3<FLOAT_TYPE>::getMagnitude() const {
    return sqrt(_x*_x + _y*_y + _z*_z);
  }

  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE unsigned int Vector3<FLOAT_TYPE>::getLongestDirection() const {
    if (_y >= _x && _y >= _z) { return 1; }
    if (_z >= _x && _z >= _y) { return 2; }
    return 0;
  }

  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE Vector3<FLOAT_TYPE> Vector3<FLOAT_TYPE>::normalize() const {
    return this->scale(1/this->getMagnitude());
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
  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE Vector3<FLOAT_TYPE> Vector3<FLOAT_TYPE>::flip() const {
    return this->scale(-1);
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
    return openviewfactor::dot<FLOAT_TYPE>(*this, rhs);
  }

  // ! openviewfactor namespace cross product function
  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE Vector3<FLOAT_TYPE> cross(const Vector3<FLOAT_TYPE> &lhs,
                                   const Vector3<FLOAT_TYPE> &rhs) {
    Vector3<FLOAT_TYPE> crossed_vector;
    crossed_vector.setX(lhs.getY() * rhs.getZ() - lhs.getZ() * rhs.getY());
    crossed_vector.setY(lhs.getZ() * rhs.getX() - lhs.getX() * rhs.getZ());
    crossed_vector.setZ(lhs.getX() * rhs.getY() - lhs.getY() * rhs.getX());
    return crossed_vector;
  }
  // ! Vector3 namespace cross product function
  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE Vector3<FLOAT_TYPE> Vector3<FLOAT_TYPE>::cross(const Vector3<FLOAT_TYPE> &rhs) const {
    return openviewfactor::cross<FLOAT_TYPE>(*this, rhs);
  }




  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE Vector3<FLOAT_TYPE> vectorMinima(const Vector3<FLOAT_TYPE> v1, const Vector3<FLOAT_TYPE> v2) {
    FLOAT_TYPE min_x = std::min(v1.getX(), v2.getX());
    FLOAT_TYPE min_y = std::min(v1.getY(), v2.getY());
    FLOAT_TYPE min_z = std::min(v1.getZ(), v2.getZ());
    return Vector3<FLOAT_TYPE>(min_x, min_y, min_z);
  }
  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE Vector3<FLOAT_TYPE> vectorMaxima(const Vector3<FLOAT_TYPE> v1, const Vector3<FLOAT_TYPE> v2) {
    FLOAT_TYPE max_x = std::max(v1.getX(), v2.getX());
    FLOAT_TYPE max_y = std::max(v1.getY(), v2.getY());
    FLOAT_TYPE max_z = std::max(v1.getZ(), v2.getZ());
    return Vector3<FLOAT_TYPE>(max_x, max_y, max_z);
  }



  //* ----- OPERATOR OVERLOADS ----- *//
  // ! openviewfactor namespace addition operator overload
  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE Vector3<FLOAT_TYPE> operator+(const Vector3<FLOAT_TYPE>& lhs, const Vector3<FLOAT_TYPE> &rhs) {
    Vector3<FLOAT_TYPE> vector_sum;
    vector_sum.setX(lhs.getX() + rhs.getX()).setY(lhs.getY() + rhs.getY()).setZ(lhs.getZ() + rhs.getZ());
    return vector_sum;
  }
  // ! Vector3 namespace addition operator overload
  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE Vector3<FLOAT_TYPE> Vector3<FLOAT_TYPE>::operator+(const Vector3<FLOAT_TYPE> &rhs) {
    Vector3<FLOAT_TYPE> vector_sum;
    vector_sum.setX(this->getX() + rhs.getX()).setY(this->getY() + rhs.getY()).setZ(this->getZ() + rhs.getZ());
    return vector_sum;
  }

  // ! openviewfactor namespace subtraction operator overload
  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE Vector3<FLOAT_TYPE> operator-(const Vector3<FLOAT_TYPE>& lhs, const Vector3<FLOAT_TYPE> &rhs) {
    Vector3<FLOAT_TYPE> vector_diff;
    vector_diff.setX(lhs.getX() - rhs.getX()).setY(lhs.getY() - rhs.getY()).setZ(lhs.getZ() - rhs.getZ());
    return vector_diff;
  }
  // ! Vector3 namespace subtraction operator overload
  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE Vector3<FLOAT_TYPE> Vector3<FLOAT_TYPE>::operator-(const Vector3<FLOAT_TYPE> &rhs) {
   Vector3<FLOAT_TYPE> vector_diff;
    vector_diff.setX(this->getX() - rhs.getX()).setY(this->getY() - rhs.getY()).setZ(this->getZ() - rhs.getZ());
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