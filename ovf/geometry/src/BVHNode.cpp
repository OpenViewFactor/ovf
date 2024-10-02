#include "BVHNode.hpp"

namespace openviewfactor {
  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE BVHNode<FLOAT_TYPE>::BVHNode()
    : _bounding_box_min(Vector3<FLOAT_TYPE>()), _bounding_box_max(Vector3<FLOAT_TYPE>()), _child_one_index(0), _first_triangle_index(0), _num_triangles(0) {}
  
  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE Vector3<FLOAT_TYPE> BVHNode<FLOAT_TYPE>::getBoundingBoxMin() const { return _bounding_box_min; }
  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE Vector3<FLOAT_TYPE> BVHNode<FLOAT_TYPE>::getBoundingBoxMax() const { return _bounding_box_max; }
  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE Vector3<FLOAT_TYPE> BVHNode<FLOAT_TYPE>::getBoundingBoxSpan() const { return (_bounding_box_max - _bounding_box_min); }

  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE unsigned int BVHNode<FLOAT_TYPE>::getChildOneIndex() const { return _child_one_index; }
  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE unsigned int BVHNode<FLOAT_TYPE>::getChildTwoIndex() const { return (_child_one_index + 1); }
  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE unsigned int BVHNode<FLOAT_TYPE>::getFirstTriangleIndex() const { return _first_triangle_index; }
  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE unsigned int BVHNode<FLOAT_TYPE>::getNumTriangles() const { return _num_triangles; }

  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE BVHNode<FLOAT_TYPE>& BVHNode<FLOAT_TYPE>::setChildOneIndex(unsigned int const index) { _child_one_index = index; return *this; }
  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE BVHNode<FLOAT_TYPE>& BVHNode<FLOAT_TYPE>::setFirstTriangleIndex(unsigned int const index) { _first_triangle_index = index; return *this; }
  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE BVHNode<FLOAT_TYPE>& BVHNode<FLOAT_TYPE>::setNumTriangles(unsigned int const n) { _num_triangles = n; return *this; }

  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE BVHNode<FLOAT_TYPE>& BVHNode<FLOAT_TYPE>::growToIncludeTriangle(Triangle<FLOAT_TYPE> triangle) {
    _bounding_box_min = vectorMinima(_bounding_box_min, triangle.getOA());
    _bounding_box_min = vectorMinima(_bounding_box_min, triangle.getOB());
    _bounding_box_min = vectorMinima(_bounding_box_min, triangle.getOC());
    _bounding_box_max = vectorMaxima(_bounding_box_max, triangle.getOA());
    _bounding_box_max = vectorMaxima(_bounding_box_max, triangle.getOB());
    _bounding_box_max = vectorMaxima(_bounding_box_max, triangle.getOC());
    return *this;
  }
  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE BVHNode<FLOAT_TYPE>& BVHNode<FLOAT_TYPE>::growToIncludeTriangulation(const Triangulation<FLOAT_TYPE> &triangulation) {
    for (unsigned int i = 0; i < triangulation.getNumElements(); i++) {
      this->growToIncludeTriangle(triangulation[i]);
    }
    return *this;
  }
  
  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE bool BVHNode<FLOAT_TYPE>::intersectRayWithNodeBoundingBox(Ray<FLOAT_TYPE> ray) const {
    FLOAT_TYPE tx1 = (this->getBoundingBoxMin().getX() - ray.getOrigin().getX()) / ray.getDirection().getX();
    FLOAT_TYPE tx2 = (this->getBoundingBoxMax().getX() - ray.getOrigin().getX()) / ray.getDirection().getX();

    FLOAT_TYPE tmin = std::min(tx1, tx2);
    FLOAT_TYPE tmax = std::max(tx1, tx2);

    FLOAT_TYPE ty1 = (this->getBoundingBoxMin().getY() - ray.getOrigin().getY()) / ray.getDirection().getY();
    FLOAT_TYPE ty2 = (this->getBoundingBoxMax().getY() - ray.getOrigin().getY()) / ray.getDirection().getY();

    tmin = std::min(tmin, std::min(ty1, ty2));
    tmax = std::max(tmax, std::max(ty1, ty2));

    FLOAT_TYPE tz1 = (this->getBoundingBoxMin().getZ() - ray.getOrigin().getZ()) / ray.getDirection().getZ();
    FLOAT_TYPE tz2 = (this->getBoundingBoxMax().getZ() - ray.getOrigin().getZ()) / ray.getDirection().getZ();

    tmin = std::min(tmin, std::min(tz1, tz2));
    tmax = std::max(tmax, std::max(tz1, tz2));

    return (tmax >= tmin && tmin < ray.getIntersectionDistance() && tmax > 0);
  }

  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE FLOAT_TYPE BVHNode<FLOAT_TYPE>::getNodeCost() const {
    return (this->getSurfaceArea() * this->getNumTriangles());
  }
  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE FLOAT_TYPE BVHNode<FLOAT_TYPE>::getSurfaceArea() const {
    Vector3<FLOAT_TYPE> span = this->getBoundingBoxSpan();
    return ((span.getX() * span.getY()) + (span.getX() * span.getZ()) + (span.getY() * span.getZ()));
  }

  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE unsigned int BVHNode<FLOAT_TYPE>::getSplitLocationAxis() const { return this->getBoundingBoxSpan().getLongestDirection(); }

  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE FLOAT_TYPE BVHNode<FLOAT_TYPE>::evaluateNodeChildrenSurfaceAreaHeuristic(const Triangulation<FLOAT_TYPE> &submesh, unsigned int axis_index, FLOAT_TYPE candidate_position) const {
    BVHNode left_box, right_box;

    std::vector<unsigned int> left_side_indices, right_side_indices;
    std::vector<Triangle<FLOAT_TYPE>> triangles = submesh.getTriangles();
    for (unsigned int i = 0; i < this->getNumTriangles(); i++) {
      if ((triangles[i].getCentroid())[i] < candidate_position) {
        left_side_indices.push_back(i);
      } else {
        right_side_indices.push_back(i);
      }
    }

    left_box.growToIncludeTriangulation(submesh.getSubMesh(left_side_indices));
    right_box.growToIncludeTriangulation(submesh.getSubMesh(right_side_indices));

    FLOAT_TYPE left_cost = left_box.getSurfaceArea() * left_side_indices.size();
    FLOAT_TYPE right_cost = right_box.getSurfaceArea() * right_side_indices.size();
    FLOAT_TYPE node_cost = 0;
    if (left_cost != NAN) { node_cost += left_cost; }
    if (right_cost != NAN) { node_cost += right_cost; }
    if (node_cost == 0) { node_cost = INFINITY; }
    return node_cost;
  }
  
  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE std::pair<FLOAT_TYPE, FLOAT_TYPE> BVHNode<FLOAT_TYPE>::getBestSplitLocationAndCost(const Triangulation<FLOAT_TYPE> &triangulation, unsigned int num_evaluation_points) const {
    unsigned int axis_index = this->getSplitLocationAxis();

    Triangulation<FLOAT_TYPE> submesh = triangulation.getSubMesh(this->getElementArraySubindices());

    FLOAT_TYPE axis_length = (this->getBoundingBoxSpan())[axis_index];
    FLOAT_TYPE axis_min = (this->getBoundingBoxMin())[axis_index];

    std::vector<FLOAT_TYPE> evaluation_point_costs;
    std::vector<FLOAT_TYPE> evaluation_point_positions;
    for (unsigned int i = 1; i <= num_evaluation_points; i++) {
      evaluation_point_positions.push_back(axis_min + (axis_length * (double(i) / (double(num_evaluation_points) + 1))));
      evaluation_point_costs.push_back(this->evaluateNodeChildrenSurfaceAreaHeuristic(submesh, axis_index, evaluation_point_positions[i]));
    }

    typename std::vector<FLOAT_TYPE>::iterator best_cost_iterator = std::min_element(evaluation_point_costs.begin(), evaluation_point_costs.end());
    FLOAT_TYPE best_cost = *best_cost_iterator;
    unsigned int best_cost_index = std::distance(evaluation_point_costs.begin(), best_cost_iterator);

    std::pair<FLOAT_TYPE, FLOAT_TYPE> split_location_and_cost;
    split_location_and_cost.first = evaluation_point_positions[best_cost_index];
    split_location_and_cost.second = best_cost;
    return split_location_and_cost;
  }

  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE std::vector<unsigned int> BVHNode<FLOAT_TYPE>::getElementArraySubindices() const {
    std::vector<unsigned int> subindices(this->getNumTriangles());
    std::generate(subindices.begin(), subindices.end(), [this, n=0] () mutable { return (this->getFirstTriangleIndex() + n++); });
    return subindices;
  }

template class BVHNode<float>;
template class BVHNode<double>;
}