#include "Vector3.hpp"

namespace openviewfactor {
  //* ----- CLASS CONSTRUCTORS ----- *//
  template <typename t>
  gpuify Vector3<t>::Vector3()
    : _xyz(std::array<t,3>({0,0,0})) {}
  
  template <typename t>
  gpuify Vector3<t>::Vector3(std::array<t,3> xyz)
    : _xyz(xyz) {}
  
  template <typename t>
  gpuify Vector3<t>::Vector3(t x, t y, t z)
    : _xyz(std::array<t,3>({x,y,z})) {}

  //* ----- ACCESSOR METHODS ----- *//
  template <typename t>
  gpuify t Vector3<t>::getX() const { return _xyz[0]; }
  template <typename t>
  gpuify t Vector3<t>::getY() const { return _xyz[1]; }
  template <typename t>
  gpuify t Vector3<t>::getZ() const { return _xyz[2]; }

  template<typename t>
  gpuify const t Vector3<t>::operator[](size_t index) const {
    if (index==0) {
      return _xyz[0];
    } else if (index == 1) {
      return _xyz[1];
    } else if (index == 2) {
      return _xyz[2];
    }
    if (index != 0 && index != 1 && index != 2) {
      throw std::runtime_error("Cannot access index");
    }
    return 0;
  }
  template<typename t>
  gpuify t Vector3<t>::operator[](size_t index) {
    if (index==0) {
      return _xyz[0];
    } else if (index == 1) {
      return _xyz[1];
    } else if (index == 2) {
      return _xyz[2];
    }
    if (index != 0 && index != 1 && index != 2) {
      throw std::runtime_error("Cannot access index");
    }
    return 0;
  }

  template <typename t>
  gpuify t Vector3<t>::getMagnitude() const {
    return sqrt(_xyz[0]*_xyz[0] + _xyz[1]*_xyz[1] + _xyz[2]*_xyz[2]);
  }

  template <typename t>
  gpuify unsigned int Vector3<t>::getLongestDirection() const {
    if (_xyz[1] >= _xyz[0] && _xyz[1] >= _xyz[2]) { return 1; }
    if (_xyz[2] >= _xyz[0] && _xyz[2] >= _xyz[1]) { return 2; }
    return 0;
  }

  template <typename t>
  gpuify Vector3<t> Vector3<t>::normalize() const {
    return this->scale(1/(this->getMagnitude()));
  }

  //* ----- MUTATOR METHODS ----- *//
  template <typename t>
  gpuify Vector3<t>& Vector3<t>::setX(t x) {
    _xyz[0] = x;
    return *this;
  }
  template <typename t>
  gpuify Vector3<t>& Vector3<t>::setY(t y) {
    _xyz[1] = y;
    return *this;
  }
  template <typename t>
  gpuify Vector3<t>& Vector3<t>::setZ(t z) {
    _xyz[2] = z;
    return *this;
  }
  template <typename t>
  gpuify Vector3<t> Vector3<t>::scale(t s) const {
    Vector3<t> scaled_vector;
    scaled_vector.setX(_xyz[0] * s).setY(_xyz[1] * s).setZ(_xyz[2] * s);
    return scaled_vector;
  }
  template <typename t>
  gpuify Vector3<t> Vector3<t>::flip() const {
    return this->scale(-1.0);
  }

  //* ----- VECTOR OPERATIONS ----- *//
  // ! openviewfactor namespace dot product function
  template <typename t>
  gpuify t dot(const Vector3<t> &lhs, const Vector3<t> &rhs) {
    t dot_product_value = (lhs.getX() * rhs.getX() +
                                    lhs.getY() * rhs.getY() +
                                    lhs.getZ() * rhs.getZ());
    return dot_product_value;
  }
  // ! Vector3 namespace dot product function
  template <typename t>
  gpuify t Vector3<t>::dot(const Vector3<t> &rhs) const {
    t dot_product_value = (this->getX() * rhs.getX() +
                                    this->getY() * rhs.getY() +
                                    this->getZ() * rhs.getZ());
    return dot_product_value;
  }

  // ! openviewfactor namespace cross product function
  template <typename t>
  gpuify Vector3<t> cross(const Vector3<t> &lhs, const Vector3<t> &rhs) {
    Vector3<t> crossed_vector;
    crossed_vector.setX(lhs.getY() * rhs.getZ() - lhs.getZ() * rhs.getY());
    crossed_vector.setY(lhs.getZ() * rhs.getX() - lhs.getX() * rhs.getZ());
    crossed_vector.setZ(lhs.getX() * rhs.getY() - lhs.getY() * rhs.getX());
    return crossed_vector;
  }
  // ! Vector3 namespace cross product function
  template <typename t>
  gpuify Vector3<t> Vector3<t>::cross(const Vector3<t> &rhs) const {
    Vector3<t> crossed_vector;
    crossed_vector.setX(this->getY() * rhs.getZ() - this->getZ() * rhs.getY());
    crossed_vector.setY(this->getZ() * rhs.getX() - this->getX() * rhs.getZ());
    crossed_vector.setZ(this->getX() * rhs.getY() - this->getY() * rhs.getX());
    return crossed_vector;
  }




  template <typename t>
  gpuify Vector3<t> vectorMinima(const Vector3<t> &v1, const Vector3<t> &v2) {
    t min_x = std::min(v1.getX(), v2.getX());
    t min_y = std::min(v1.getY(), v2.getY());
    t min_z = std::min(v1.getZ(), v2.getZ());
    return Vector3<t>(min_x, min_y, min_z);
  }
  template <typename t>
  gpuify Vector3<t> vectorMaxima(const Vector3<t> &v1, const Vector3<t> &v2) {
    t max_x = std::max(v1.getX(), v2.getX());
    t max_y = std::max(v1.getY(), v2.getY());
    t max_z = std::max(v1.getZ(), v2.getZ());
    return Vector3<t>(max_x, max_y, max_z);
  }



  //* ----- OPERATOR OVERLOADS ----- *//
  // ! openviewfactor namespace addition operator overload
  template <typename t>
  gpuify Vector3<t> operator+(const Vector3<t>& lhs, const Vector3<t> &rhs) {
    Vector3<t> vector_sum;
    vector_sum.setX(lhs.getX() + rhs.getX()).setY(lhs.getY() + rhs.getY()).setZ(lhs.getZ() + rhs.getZ());
    return vector_sum;
  }
  // ! Vector3 namespace addition operator overload
  template <typename t>
  gpuify Vector3<t> Vector3<t>::operator+(const Vector3<t> &rhs) {
    Vector3<t> vector_sum;
    vector_sum.setX(this->getX() + rhs.getX()).setY(this->getY() + rhs.getY()).setZ(this->getZ() + rhs.getZ());
    return vector_sum;
  }

  // ! openviewfactor namespace subtraction operator overload
  template <typename t>
  gpuify Vector3<t> operator-(const Vector3<t>& lhs, const Vector3<t> &rhs) {
    Vector3<t> vector_diff;
    vector_diff.setX(lhs.getX() - rhs.getX()).setY(lhs.getY() - rhs.getY()).setZ(lhs.getZ() - rhs.getZ());
    return vector_diff;
  }
  // ! Vector3 namespace subtraction operator overload
  template <typename t>
  gpuify Vector3<t> Vector3<t>::operator-(const Vector3<t> &rhs) {
   Vector3<t> vector_diff;
    vector_diff.setX(this->getX() - rhs.getX()).setY(this->getY() - rhs.getY()).setZ(this->getZ() - rhs.getZ());
    return vector_diff;
  }

  // ! openviewfactor namespace equality operator overload
  template <typename t>
  gpuify bool operator==(const Vector3<t> &lhs, const Vector3<t> &rhs) {
    bool x_equal = lhs.getX() == rhs.getX();
    bool y_equal = lhs.getY() == rhs.getY();
    bool z_equal = lhs.getZ() == rhs.getZ();
    return (x_equal && y_equal && z_equal);
  }
  // ! Vector3 namespace equality operator overload
  template <typename t>
  gpuify bool Vector3<t>::operator==(const Vector3<t> &rhs) {
    bool x_equal = this->getX() == rhs.getX();
    bool y_equal = this->getY() == rhs.getY();
    bool z_equal = this->getZ() == rhs.getZ();
    return (x_equal && y_equal && z_equal);
  }

  // ! openviewfactor namespace inequality operator overload
  template <typename t>
  gpuify bool operator!=(const Vector3<t> &lhs,
                                  const Vector3<t> &rhs) {
    return !(lhs == rhs);
  }
  // ! Vector3 namespace inequality operator overload
  template <typename t>
  gpuify bool Vector3<t>::operator!=(const Vector3<t> &rhs) {
    return !(*this == rhs);
  }

template class Vector3<float>;
template class Vector3<double>;

template Vector3<float> vectorMinima(const Vector3<float> &, const Vector3<float> &);
template Vector3<double> vectorMinima(const Vector3<double> &, const Vector3<double> &);

template Vector3<float> vectorMaxima(const Vector3<float> &, const Vector3<float> &);
template Vector3<double> vectorMaxima(const Vector3<double> &, const Vector3<double> &);

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