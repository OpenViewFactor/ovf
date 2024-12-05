#include "Vector3.hpp"
#include "Triangle.hpp"
#include "Triangulation.hpp"
#include "Ray.hpp"
#include "BVHNode.hpp"

namespace openviewfactor {
  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE BVHNode<FLOAT_TYPE>::BVHNode()
    : _bounding_box_min(Vector3<FLOAT_TYPE>(INFINITY,INFINITY,INFINITY)), _bounding_box_max(Vector3<FLOAT_TYPE>(-INFINITY,-INFINITY,-INFINITY)), _child_one_index(0), _first_triangle_index(0), _num_triangles(0) {}
  
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
  OVF_HOST_DEVICE bool BVHNode<FLOAT_TYPE>::isLeaf() const { return !(_num_triangles == 0); }

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
    _num_triangles++;
    return *this;
  }
  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE BVHNode<FLOAT_TYPE>& BVHNode<FLOAT_TYPE>::growToIncludeTriangulation(std::shared_ptr<Triangulation<FLOAT_TYPE>> triangulation) {
    for (unsigned int i = 0; i < triangulation->getNumElements(); i++) {
      this->growToIncludeTriangle((*triangulation)[i]);
    }
    this->setNumTriangles(triangulation->getNumElements());
    return *this;
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
  OVF_HOST_DEVICE unsigned int BVHNode<FLOAT_TYPE>::getSplitLocationAxis() const {
    Vector3<FLOAT_TYPE> span = this->getBoundingBoxSpan();
    unsigned int axis = span.getLongestDirection();
    return axis;
  }

  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE FLOAT_TYPE BVHNode<FLOAT_TYPE>::evaluateNodeChildrenSurfaceAreaHeuristic(std::shared_ptr<Triangulation<FLOAT_TYPE>> submesh, unsigned int axis_index, FLOAT_TYPE candidate_position) const {
    BVHNode<FLOAT_TYPE> left_box, right_box;

    std::vector<unsigned int> left_side_indices, right_side_indices;
    std::vector<Triangle<FLOAT_TYPE>> triangles = submesh->getTriangles();
    for (unsigned int i = 0; i < this->getNumTriangles(); i++) {
      if ((triangles[i].getCentroid())[axis_index] < candidate_position) {
        left_side_indices.push_back(i);
      } else {
        right_side_indices.push_back(i);
      }
    }

    left_box.growToIncludeTriangulation(submesh->getSubMesh(left_side_indices));
    right_box.growToIncludeTriangulation(submesh->getSubMesh(right_side_indices));

    FLOAT_TYPE left_cost = left_box.getSurfaceArea() * left_side_indices.size();
    FLOAT_TYPE right_cost = right_box.getSurfaceArea() * right_side_indices.size();
    FLOAT_TYPE node_cost = 0;
    if (left_cost != NAN) { node_cost += left_cost; }
    if (right_cost != NAN) { node_cost += right_cost; }
    if (node_cost == 0) { node_cost = INFINITY; }
    return node_cost;
  }
  
  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE std::pair<FLOAT_TYPE, FLOAT_TYPE> BVHNode<FLOAT_TYPE>::getBestSplitLocationAndCost(std::shared_ptr<Triangulation<FLOAT_TYPE>> triangulation, std::vector<unsigned int> mesh_element_indices, unsigned int axis_index, unsigned int num_evaluation_points) const {

    std::cout << ">>>>>> BVHNode getBestSplitLocationAndCost Initiated" << std::endl;

    std::vector<unsigned int> submesh_indices(this->getNumTriangles());
    for (int i = 0; i < submesh_indices.size(); i++) {
      submesh_indices[i] = mesh_element_indices[(this->getElementArraySubindices())[i]];
    }
    auto submesh = triangulation->getSubMesh(submesh_indices);

    FLOAT_TYPE axis_length = (this->getBoundingBoxSpan())[axis_index];
    FLOAT_TYPE axis_min = (this->getBoundingBoxMin())[axis_index];

    std::cout << ">>>>>>> Axis: " << axis_index << ". Axis Length: " << axis_length << ". Axis Minimum: " << axis_min << std::endl;

    std::vector<FLOAT_TYPE> evaluation_point_costs(num_evaluation_points);
    std::vector<FLOAT_TYPE> evaluation_point_positions(num_evaluation_points);
    for (unsigned int i = 1; i <= num_evaluation_points; i++) {
      FLOAT_TYPE position = axis_min + (axis_length * (double(i) / (double(num_evaluation_points) + 1)));
      evaluation_point_positions[i-1] = position;

      FLOAT_TYPE cost = this->evaluateNodeChildrenSurfaceAreaHeuristic(submesh, axis_index, position);
      evaluation_point_costs[i-1] = cost;
    }

    for (int i = 0; i < num_evaluation_points; i++) {
      std::cout << ">>>>>>>> Position: " << evaluation_point_positions[i] << ". Cost: " << evaluation_point_costs[i] << std::endl;
    }

    typename std::vector<FLOAT_TYPE>::iterator best_cost_iterator = std::min_element(evaluation_point_costs.begin(), evaluation_point_costs.end(), [] (const FLOAT_TYPE& a, const FLOAT_TYPE& b) {
      if (std::isnan(a)) return false;
      if (std::isnan(b)) return true;
      return (a < b); });
    FLOAT_TYPE best_cost = *best_cost_iterator;
    unsigned int best_cost_index = std::distance(evaluation_point_costs.begin(), best_cost_iterator);

    std::cout << ">>>>>>>> best_cost: " << best_cost << " -- best_cost_index: " << best_cost_index << std::endl;

    std::pair<FLOAT_TYPE, FLOAT_TYPE> split_location_and_cost;

    std::cout << ">>>>>>>> best evaluation point: " << evaluation_point_positions[best_cost_index] << std::endl;

    split_location_and_cost.first = evaluation_point_positions[best_cost_index];

    std::cout << ">>>>>>>> best evaluation point in the pair : " << split_location_and_cost.first << std::endl;
    split_location_and_cost.second = best_cost;
    return split_location_and_cost;
  }

  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE std::vector<unsigned int> BVHNode<FLOAT_TYPE>::getElementArraySubindices() const {
    std::vector<unsigned int> subindices(this->getNumTriangles());
    std::iota(subindices.begin(), subindices.end(), this->getFirstTriangleIndex());
    return subindices;
  }

  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE void BVHNode<FLOAT_TYPE>::writeToFile(std::ofstream& outfile, unsigned int node_index) const {
    
    Vector3<FLOAT_TYPE> min_min_min = this->getBoundingBoxMin();
    Vector3<FLOAT_TYPE> max_min_min = (this->getBoundingBoxMin()).setX((this->getBoundingBoxMax())[0]);
    Vector3<FLOAT_TYPE> min_max_min = (this->getBoundingBoxMin()).setY((this->getBoundingBoxMax())[1]);
    Vector3<FLOAT_TYPE> min_min_max = (this->getBoundingBoxMin()).setZ((this->getBoundingBoxMax())[2]);
    Vector3<FLOAT_TYPE> min_max_max = (this->getBoundingBoxMax()).setX((this->getBoundingBoxMin())[0]);
    Vector3<FLOAT_TYPE> max_min_max = (this->getBoundingBoxMax()).setY((this->getBoundingBoxMin())[1]);
    Vector3<FLOAT_TYPE> max_max_min = (this->getBoundingBoxMax()).setZ((this->getBoundingBoxMin())[2]);
    Vector3<FLOAT_TYPE> max_max_max = this->getBoundingBoxMax();

    std::vector<Vector3<FLOAT_TYPE>> points = {min_min_min,
                                               max_min_min,
                                               min_max_min,
                                               min_min_max,
                                               min_max_max,
                                               max_min_max,
                                               max_max_min,
                                               max_max_max};

    for (auto p : points) {
      outfile << "p," << p[0] << "," << p[1] << "," << p[2] << "\n";
    }

    std::vector<std::array<unsigned int, 3>> connections = {{1,2,6},{1,6,4},
                                                            {1,3,7},{1,7,2},
                                                            {1,4,5},{1,5,3},
                                                            {8,7,3},{8,3,5},
                                                            {8,5,4},{8,4,6},
                                                            {8,6,2},{8,2,7}};

    for (auto c : connections) {
      outfile << "c," << (8 * node_index) + c[0] << "," << (8 * node_index) + c[1] << "," << (8 * node_index) + c[2] << "\n";
    }
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

template class BVHNode<float>;
template class BVHNode<double>;
}