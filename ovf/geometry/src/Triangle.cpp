#include "Vector3.hpp"
#include "Triangle.hpp"

#include <iostream>

namespace openviewfactor {
  //* ----- CLASS CONSTRUCTORS ----- *//
  template <typename t>
  gpuify Triangle<t>::Triangle()
    : _pts(std::array<Vector3<t>,3>({Vector3<t>(), Vector3<t>(), Vector3<t>()})) {}

  template <typename t>
  gpuify Triangle<t>::Triangle(std::array<Vector3<t>,3> pts)
    : _pts(pts) {}

  template <typename t>
  gpuify Triangle<t>::Triangle(std::array<std::array<t,3>,3> pts) 
    : _pts({Vector3<t>(pts[0]), Vector3<t>(pts[1]), Vector3<t>(pts[2])}) {}

  template <typename t>
  gpuify Triangle<t>::Triangle(Vector3<t> OA, Vector3<t> OB, Vector3<t> OC)
    : _pts(std::array<Vector3<t>,3>({OA,OB,OC})) {}
  
  //* ----- ACCESSOR METHODS ----- *//
  template <typename t>
  gpuify Vector3<t> Triangle<t>::getOA() const { return _pts[0]; }
  template <typename t>
  gpuify Vector3<t> Triangle<t>::getOB() const { return _pts[1]; }
  template <typename t>
  gpuify Vector3<t> Triangle<t>::getOC() const { return _pts[2]; }

  template <typename t>
  gpuify Vector3<t> Triangle<t>::getAB() const { return _pts[1] - _pts[0]; }
  template <typename t>
  gpuify Vector3<t> Triangle<t>::getBC() const { return _pts[2] - _pts[1]; }
  template <typename t>
  gpuify Vector3<t> Triangle<t>::getCA() const { return _pts[0] - _pts[2]; }

  template <typename t>
  gpuify Vector3<t> Triangle<t>::getNormal() const {
    Vector3<t> unnormalized_normal = cross(this->getAB(), (this->getCA()).flip());
    return unnormalized_normal.normalize();
  }
  template <typename t>
  gpuify Vector3<t> Triangle<t>::getCentroid() const {
    Vector3<t> centroid = (_pts[0]+_pts[1]+_pts[2]).scale(1.0/3.0);
    return centroid;
  }
  template <typename t>
  gpuify t Triangle<t>::getArea() const {
    Vector3<t> unnormalized_normal = cross(this->getAB(), this->getCA());
    return (unnormalized_normal.getMagnitude() / 2.0);
  }

  template <typename t>
  gpuify t Triangle<t>::evaluateAspectRatio() const {
    t aspect_ratio = 0.0;
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
      t current_bisector_aspect_ratio = std::max({bisector_side_length_1, bisector_side_length_2}) / std::min({bisector_side_length_1, bisector_side_length_2});

      auto opposite_edge_direction = (third_point - second_point).normalize();
      if (angle_bisector.normalize().dot(opposite_edge_direction) < 0) { opposite_edge_direction.flip(); }
      auto midpoint_side_length_1 = (angle_bisector - opposite_edge_direction.scale(angle_bisector.dot(opposite_edge_direction))).getMagnitude();
      auto midpoint_side_length_2 = midpoint_to_midpoint.getMagnitude();
      t current_midpoint_aspect_ratio = std::max({midpoint_side_length_1, midpoint_side_length_2}) / std::min({midpoint_side_length_1, midpoint_side_length_2});

      aspect_ratio = std::max({aspect_ratio, current_bisector_aspect_ratio, current_midpoint_aspect_ratio});
    }
    return aspect_ratio / std::sqrt(3.0);
  }

  template <typename t>
  gpuify t Triangle<t>::evaluateSkewness() const {
    auto angle_OA = std::acos( (this->getAB()).dot((this->getCA()).flip()) / ((this->getAB()).getMagnitude() * (this->getCA()).getMagnitude()) );
    auto circumscribed_diameter = (this->getBC()).getMagnitude() / std::sin(angle_OA);
    auto equilateral_side_length = (circumscribed_diameter * std::sqrt(3)*0.5);
    auto equilateral_area = 0.5 * (equilateral_side_length * equilateral_side_length*0.5*1.732050807568877);
    auto skewness = (equilateral_area - this->getArea()) / equilateral_area;
    return skewness;
  }

  template <typename t>
  gpuify t Triangle<t>::evaluateElementQuality() const {
    t triangle_constant = 6.92820323;
    std::array<Vector3<t>,3> edges = {this->getAB(), this->getBC(), this->getCA()};
    t square_sum = 0.0;
    for (int i = 0; i < 3; i++) {
      t edge_length = edges[i].getMagnitude();
      square_sum += (edge_length * edge_length);
    }
    t quality = triangle_constant * this->getArea() / square_sum;
    return quality;
  }

  //* ----- MUTATOR METHODS ----- *//
  template <typename t>
  gpuify Triangle<t>& Triangle<t>::setOA(t x, t y, t z) {
    _pts[0].setX(x).setY(y).setZ(z);
    return *this;
  }
  template <typename t>
  gpuify Triangle<t>& Triangle<t>::setOA(Vector3<t> v) {
    _pts[0] = v;
    return *this;
  }
  template <typename t>
  gpuify Triangle<t>& Triangle<t>::setOB(t x, t y, t z) {
    _pts[1].setX(x).setY(y).setZ(z);
    return *this;
  }
  template <typename t>
  gpuify Triangle<t>& Triangle<t>::setOB(Vector3<t> v) {
    _pts[1] = v;
    return *this;
  }
  template <typename t>
  gpuify Triangle<t>& Triangle<t>::setOC(t x, t y, t z) {
    _pts[2].setX(x).setY(y).setZ(z);
    return *this;
  }
  template <typename t>
  gpuify Triangle<t>& Triangle<t>::setOC(Vector3<t> v) {
    _pts[2] = v;
    return *this;
  }

  template <typename t>
  gpuify bool Triangle<t>::operator==(const Triangle<t> &rhs) const {
    return ((_pts[0] == rhs.getOA() && _pts[1] == rhs.getOB() && _pts[2] == rhs.getOC()) ||
            (_pts[1] == rhs.getOA() && _pts[2] == rhs.getOB() && _pts[0] == rhs.getOC()) ||
            (_pts[2] == rhs.getOA() && _pts[0] == rhs.getOB() && _pts[1] == rhs.getOC()));
  }
  template <typename t>
  gpuify bool Triangle<t>::operator!=(const Triangle<t> &rhs) const {
    return !(*this == rhs);
  }

  template <typename t>
  gpuify bool operator==(const Triangle<t> &lhs, const Triangle<t> &rhs) {
    return (lhs.getOA() == rhs.getOA() && lhs.getOB() == rhs.getOB() && lhs.getOC() == rhs.getOC());
  }
  template <typename t>
  gpuify bool operator!=(const Triangle<t> &lhs, const Triangle<t> &rhs) {
    return !(lhs == rhs);
  }
  
  template <typename t>
  gpuify const Vector3<t> Triangle<t>::operator[](size_t index) const {
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

  template <typename t>
  gpuify Vector3<t> Triangle<t>::operator[](size_t index) {
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