#include "Vector3.hpp"
#include "Triangle.hpp"

#include <iostream>

namespace openviewfactor {
  //* ----- CLASS CONSTRUCTORS ----- *//
  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE Triangle<FLOAT_TYPE>::Triangle()
    : _pts(std::array<Vector3<FLOAT_TYPE>,3>({Vector3<FLOAT_TYPE>(), Vector3<FLOAT_TYPE>(), Vector3<FLOAT_TYPE>()})) {}

  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE Triangle<FLOAT_TYPE>::Triangle(std::array<Vector3<FLOAT_TYPE>,3> pts)
    : _pts(pts) {}

  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE Triangle<FLOAT_TYPE>::Triangle(std::array<std::array<FLOAT_TYPE,3>,3> pts) 
    : _pts({Vector3<FLOAT_TYPE>(pts[0]), Vector3<FLOAT_TYPE>(pts[1]), Vector3<FLOAT_TYPE>(pts[2])}) {}

  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE Triangle<FLOAT_TYPE>::Triangle(Vector3<FLOAT_TYPE> OA, Vector3<FLOAT_TYPE> OB, Vector3<FLOAT_TYPE> OC)
    : _pts(std::array<Vector3<FLOAT_TYPE>,3>({OA,OB,OC})) {}
  
  //* ----- ACCESSOR METHODS ----- *//
  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE Vector3<FLOAT_TYPE> Triangle<FLOAT_TYPE>::getOA() const { return _pts[0]; }
  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE Vector3<FLOAT_TYPE> Triangle<FLOAT_TYPE>::getOB() const { return _pts[1]; }
  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE Vector3<FLOAT_TYPE> Triangle<FLOAT_TYPE>::getOC() const { return _pts[2]; }

  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE Vector3<FLOAT_TYPE> Triangle<FLOAT_TYPE>::getAB() const { return _pts[1] - _pts[0]; }
  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE Vector3<FLOAT_TYPE> Triangle<FLOAT_TYPE>::getBC() const { return _pts[2] - _pts[1]; }
  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE Vector3<FLOAT_TYPE> Triangle<FLOAT_TYPE>::getCA() const { return _pts[0] - _pts[2]; }

  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE Vector3<FLOAT_TYPE> Triangle<FLOAT_TYPE>::getNormal() const {
    Vector3<FLOAT_TYPE> unnormalized_normal = cross(this->getAB(), (this->getCA()).flip());
    return unnormalized_normal.normalize();
  }
  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE Vector3<FLOAT_TYPE> Triangle<FLOAT_TYPE>::getCentroid() const {
    Vector3<FLOAT_TYPE> centroid = (_pts[0]+_pts[1]+_pts[2]).scale(1.0/3.0);
    return centroid;
  }
  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE FLOAT_TYPE Triangle<FLOAT_TYPE>::getArea() const {
    Vector3<FLOAT_TYPE> unnormalized_normal = cross(this->getAB(), this->getCA());
    return (unnormalized_normal.getMagnitude() / 2.0);
  }

  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE FLOAT_TYPE Triangle<FLOAT_TYPE>::evaluateAspectRatio() const {
    FLOAT_TYPE aspect_ratio = 0.0;
    for (size_t point_index = 0; point_index < 3; point_index++) {
      auto current_point = (*this)[point_index];
      auto second_point = (*this)[point_index + 1];
      auto third_point = (*this)[point_index + 2];
      auto angle_bisector = (second_point + (third_point - second_point).scale(0.5)) - current_point;
      auto angle_bisector_direction = angle_bisector.normalize();
      auto bisector_side_length_1 = angle_bisector.getMagnitude();
      auto midpoint_to_midpoint = ((second_point - current_point).scale(0.5)) - ((third_point - current_point).scale(0.5));
      if (midpoint_to_midpoint.normalize().dot(angle_bisector_direction) < 0) { angle_bisector_direction.flip(); }
      auto bisector_side_length_2 = (midpoint_to_midpoint - angle_bisector_direction.scale(midpoint_to_midpoint.dot(angle_bisector_direction))).getMagnitude();
      FLOAT_TYPE current_bisector_aspect_ratio = std::max({bisector_side_length_1, bisector_side_length_2}) / std::min({bisector_side_length_1, bisector_side_length_2});

      auto opposite_edge_direction = (third_point - second_point).normalize();
      if (angle_bisector.normalize().dot(opposite_edge_direction) < 0) { opposite_edge_direction.flip(); }
      auto midpoint_side_length_1 = (angle_bisector - opposite_edge_direction.scale(angle_bisector.dot(opposite_edge_direction))).getMagnitude();
      auto midpoint_side_length_2 = midpoint_to_midpoint.getMagnitude();
      FLOAT_TYPE current_midpoint_aspect_ratio = std::max({midpoint_side_length_1, midpoint_side_length_2}) / std::min({midpoint_side_length_1, midpoint_side_length_2});

      aspect_ratio = std::max({aspect_ratio, current_bisector_aspect_ratio, current_midpoint_aspect_ratio});
    }
    return aspect_ratio / std::sqrt(3.0);
  }

  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE FLOAT_TYPE Triangle<FLOAT_TYPE>::evaluateSkewness() const {
    auto angle_OA = std::acos( (this->getAB()).dot((this->getCA()).flip()) / ((this->getAB()).getMagnitude() * (this->getCA()).getMagnitude()) );
    auto circumscribed_diameter = (this->getBC()).getMagnitude() / std::sin(angle_OA);
    auto equilateral_side_length = (circumscribed_diameter * std::sqrt(3)*0.5);
    auto equilateral_area = 0.5 * (equilateral_side_length * equilateral_side_length*0.5*1.732050807568877);
    auto skewness = (equilateral_area - this->getArea()) / equilateral_area;
    return skewness;
  }

  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE FLOAT_TYPE Triangle<FLOAT_TYPE>::evaluateElementQuality() const {
    FLOAT_TYPE triangle_constant = 6.92820323;
    std::array<Vector3<FLOAT_TYPE>,3> edges = {this->getAB(), this->getBC(), this->getCA()};
    FLOAT_TYPE square_sum = 0.0;
    for (int i = 0; i < 3; i++) {
      FLOAT_TYPE edge_length = edges[i].getMagnitude();
      square_sum += (edge_length * edge_length);
    }
    FLOAT_TYPE quality = triangle_constant * this->getArea() / square_sum;
    return quality;
  }

  //* ----- MUTATOR METHODS ----- *//
  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE Triangle<FLOAT_TYPE>& Triangle<FLOAT_TYPE>::setOA(FLOAT_TYPE x, FLOAT_TYPE y, FLOAT_TYPE z) {
    _pts[0].setX(x).setY(y).setZ(z);
    return *this;
  }
  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE Triangle<FLOAT_TYPE>& Triangle<FLOAT_TYPE>::setOA(Vector3<FLOAT_TYPE> v) {
    _pts[0] = v;
    return *this;
  }
  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE Triangle<FLOAT_TYPE>& Triangle<FLOAT_TYPE>::setOB(FLOAT_TYPE x, FLOAT_TYPE y, FLOAT_TYPE z) {
    _pts[1].setX(x).setY(y).setZ(z);
    return *this;
  }
  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE Triangle<FLOAT_TYPE>& Triangle<FLOAT_TYPE>::setOB(Vector3<FLOAT_TYPE> v) {
    _pts[1] = v;
    return *this;
  }
  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE Triangle<FLOAT_TYPE>& Triangle<FLOAT_TYPE>::setOC(FLOAT_TYPE x, FLOAT_TYPE y, FLOAT_TYPE z) {
    _pts[2].setX(x).setY(y).setZ(z);
    return *this;
  }
  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE Triangle<FLOAT_TYPE>& Triangle<FLOAT_TYPE>::setOC(Vector3<FLOAT_TYPE> v) {
    _pts[2] = v;
    return *this;
  }

  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE bool Triangle<FLOAT_TYPE>::operator==(const Triangle<FLOAT_TYPE> &rhs) const {
    return ((_pts[0] == rhs.getOA() && _pts[1] == rhs.getOB() && _pts[2] == rhs.getOC()) ||
            (_pts[1] == rhs.getOA() && _pts[2] == rhs.getOB() && _pts[0] == rhs.getOC()) ||
            (_pts[2] == rhs.getOA() && _pts[0] == rhs.getOB() && _pts[1] == rhs.getOC()));
  }
  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE bool Triangle<FLOAT_TYPE>::operator!=(const Triangle<FLOAT_TYPE> &rhs) const {
    return !(*this == rhs);
  }

  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE bool operator==(const Triangle<FLOAT_TYPE> &lhs, const Triangle<FLOAT_TYPE> &rhs) {
    return (lhs.getOA() == rhs.getOA() && lhs.getOB() == rhs.getOB() && lhs.getOC() == rhs.getOC());
  }
  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE bool operator!=(const Triangle<FLOAT_TYPE> &lhs, const Triangle<FLOAT_TYPE> &rhs) {
    return !(lhs == rhs);
  }
  
  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE const Vector3<FLOAT_TYPE> Triangle<FLOAT_TYPE>::operator[](size_t index) const {
    switch (index % 3) {
      case 0:
        return _pts[0];
        break;
      case 1:
        return _pts[1];
        break;
      case 2:
        return _pts[2];
        break;
      default:
        throw std::runtime_error("Could not access a point of the triangle with that index for some reason.");
    }
  }

  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE Vector3<FLOAT_TYPE> Triangle<FLOAT_TYPE>::operator[](size_t index) {
    switch (index % 3) {
      case 0:
        return _pts[0];
        break;
      case 1:
        return _pts[1];
        break;
      case 2:
        return _pts[2];
        break;
      default:
        throw std::runtime_error("Could not access a point of the triangle with that index for some reason.");
    }
  }

template class Triangle<float>;
template class Triangle<double>;
template bool operator==(const Triangle<float> &, const Triangle<float> &);
template bool operator==(const Triangle<double> &, const Triangle<double> &);
template bool operator!=(const Triangle<float> &, const Triangle<float> &);
template bool operator!=(const Triangle<double> &, const Triangle<double> &);
}