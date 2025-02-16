#include "Vector3.hpp"
#include "Triangle.hpp"
#include "Triangulation.hpp"
#include "Ray.hpp"
#include "BVHNode.hpp"

namespace openviewfactor {
  template <typename t>
  gpuify BVHNode<t>::BVHNode()
    : _bounding_box_min(Vector3<t>(INFINITY,INFINITY,INFINITY)), _bounding_box_max(Vector3<t>(-INFINITY,-INFINITY,-INFINITY)), _child_one_index(0), _first_triangle_index(0), _num_triangles(0) {}
  
  template <typename t>
  gpuify Vector3<t> BVHNode<t>::getBoundingBoxMin() const { return _bounding_box_min; }
  template <typename t>
  gpuify Vector3<t> BVHNode<t>::getBoundingBoxMax() const { return _bounding_box_max; }
  template <typename t>
  gpuify Vector3<t> BVHNode<t>::getBoundingBoxSpan() const { return (_bounding_box_max - _bounding_box_min); }

  template <typename t>
  gpuify unsigned int BVHNode<t>::getChildOneIndex() const { return _child_one_index; }
  template <typename t>
  gpuify unsigned int BVHNode<t>::getChildTwoIndex() const { return (_child_one_index + 1); }
  template <typename t>
  gpuify unsigned int BVHNode<t>::getFirstTriangleIndex() const { return _first_triangle_index; }
  template <typename t>
  gpuify unsigned int BVHNode<t>::getNumTriangles() const { return _num_triangles; }

  template <typename t>
  gpuify bool BVHNode<t>::isLeaf() const { return !(_num_triangles == 0); }

  template <typename t>
  gpuify BVHNode<t>& BVHNode<t>::setChildOneIndex(unsigned int const index) { _child_one_index = index; return *this; }
  template <typename t>
  gpuify BVHNode<t>& BVHNode<t>::setFirstTriangleIndex(unsigned int const index) { _first_triangle_index = index; return *this; }
  template <typename t>
  gpuify BVHNode<t>& BVHNode<t>::setNumTriangles(unsigned int const n) { _num_triangles = n; return *this; }

  template <typename t>
  gpuify BVHNode<t>& BVHNode<t>::growToIncludeTriangle(Triangle<t> triangle) {
    _bounding_box_min = vectorMinima(_bounding_box_min, triangle.getOA());
    _bounding_box_min = vectorMinima(_bounding_box_min, triangle.getOB());
    _bounding_box_min = vectorMinima(_bounding_box_min, triangle.getOC());
    _bounding_box_max = vectorMaxima(_bounding_box_max, triangle.getOA());
    _bounding_box_max = vectorMaxima(_bounding_box_max, triangle.getOB());
    _bounding_box_max = vectorMaxima(_bounding_box_max, triangle.getOC());
    _num_triangles++;
    return *this;
  }
  template <typename t>
  gpuify BVHNode<t>& BVHNode<t>::growToIncludeTriangulation(std::shared_ptr<Triangulation<t>> triangulation) {
    for (unsigned int i = 0; i < triangulation->getNumElements(); i++) {
      this->growToIncludeTriangle((*triangulation)[i]);
    }
    this->setNumTriangles(triangulation->getNumElements());
    return *this;
  }

  template <typename t>
  gpuify t BVHNode<t>::getNodeCost() const {
    return (this->getSurfaceArea() * this->getNumTriangles());
  }
  template <typename t>
  gpuify t BVHNode<t>::getSurfaceArea() const {
    Vector3<t> span = this->getBoundingBoxSpan();
    return ((span.getX() * span.getY()) + (span.getX() * span.getZ()) + (span.getY() * span.getZ()));
  }

  template <typename t>
  gpuify unsigned int BVHNode<t>::getSplitLocationAxis() const {
    Vector3<t> span = this->getBoundingBoxSpan();
    unsigned int axis = span.getLongestDirection();
    return axis;
  }

  template <typename t>
  gpuify t BVHNode<t>::evaluateNodeChildrenSurfaceAreaHeuristic(std::shared_ptr<Triangulation<t>> submesh, unsigned int axis_index, t candidate_position) const {
    BVHNode<t> left_box, right_box;

    std::vector<unsigned int> left_side_indices;
    std::vector<unsigned int> right_side_indices;
    std::vector<Triangle<t>> triangles = submesh->getTriangles();
    for (unsigned int i = 0; i < this->getNumTriangles(); i++) {
      if ((triangles[i].getCentroid())[axis_index] < candidate_position) {
        left_side_indices.push_back(i);
      } else {
        right_side_indices.push_back(i);
      }
    }

    left_box.growToIncludeTriangulation(submesh->getSubMesh(left_side_indices));
    right_box.growToIncludeTriangulation(submesh->getSubMesh(right_side_indices));

    t left_cost = left_box.getSurfaceArea() * left_side_indices.size();
    t right_cost = right_box.getSurfaceArea() * right_side_indices.size();
    t node_cost = 0;
    if (left_cost != NAN) { node_cost += left_cost; }
    if (right_cost != NAN) { node_cost += right_cost; }
    if (node_cost == 0) { node_cost = INFINITY; }
    return node_cost;
  }
  
  template <typename t>
  gpuify std::pair<t, t> BVHNode<t>::getBestSplitLocationAndCost(std::shared_ptr<Triangulation<t>> triangulation, std::vector<unsigned int> mesh_element_indices, unsigned int axis_index, unsigned int num_evaluation_points) const {

    std::vector<unsigned int> submesh_indices(this->getNumTriangles());
    for (int i = 0; i < submesh_indices.size(); i++) {
      submesh_indices[i] = mesh_element_indices[(this->getElementArraySubindices())[i]];
    }
    auto submesh = triangulation->getSubMesh(submesh_indices);

    t axis_length = (this->getBoundingBoxSpan())[axis_index];
    t axis_min = (this->getBoundingBoxMin())[axis_index];

    std::vector<t> evaluation_point_costs(num_evaluation_points);
    std::vector<t> evaluation_point_positions(num_evaluation_points);
    for (unsigned int i = 1; i <= num_evaluation_points; i++) {
      t position = axis_min + (axis_length * (double(i) / (double(num_evaluation_points) + 1)));
      evaluation_point_positions[i-1] = position;

      t cost = this->evaluateNodeChildrenSurfaceAreaHeuristic(submesh, axis_index, position);
      evaluation_point_costs[i-1] = cost;
    }

    typename std::vector<t>::iterator best_cost_iterator = std::min_element(evaluation_point_costs.begin(), evaluation_point_costs.end(), [] (const t& a, const t& b) {
      if (std::isnan(a)) return false;
      if (std::isnan(b)) return true;
      return (a < b); });
    t best_cost = *best_cost_iterator;
    unsigned int best_cost_index = std::distance(evaluation_point_costs.begin(), best_cost_iterator);
    std::pair<t, t> split_location_and_cost;
    split_location_and_cost.first = evaluation_point_positions[best_cost_index];
    split_location_and_cost.second = best_cost;
    return split_location_and_cost;
  }

  template <typename t>
  gpuify std::vector<unsigned int> BVHNode<t>::getElementArraySubindices() const {
    std::vector<unsigned int> subindices(this->getNumTriangles());
    std::iota(subindices.begin(), subindices.end(), this->getFirstTriangleIndex());
    return subindices;
  }

  template <typename t>
  gpuify void BVHNode<t>::writeToFile(std::ofstream& outfile, unsigned int node_index) const {
    
    Vector3<t> min_min_min = this->getBoundingBoxMin();
    Vector3<t> max_min_min = (this->getBoundingBoxMin()).setX((this->getBoundingBoxMax())[0]);
    Vector3<t> min_max_min = (this->getBoundingBoxMin()).setY((this->getBoundingBoxMax())[1]);
    Vector3<t> min_min_max = (this->getBoundingBoxMin()).setZ((this->getBoundingBoxMax())[2]);
    Vector3<t> min_max_max = (this->getBoundingBoxMax()).setX((this->getBoundingBoxMin())[0]);
    Vector3<t> max_min_max = (this->getBoundingBoxMax()).setY((this->getBoundingBoxMin())[1]);
    Vector3<t> max_max_min = (this->getBoundingBoxMax()).setZ((this->getBoundingBoxMin())[2]);
    Vector3<t> max_max_max = this->getBoundingBoxMax();

    std::vector<Vector3<t>> points = {min_min_min,
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

  template <typename t>
  gpuify bool BVHNode<t>::intersectRayWithNodeBoundingBox(std::shared_ptr<Ray<t>> ray) const {
    t tx1 = (this->getBoundingBoxMin().getX() - ray->getOrigin().getX()) / ray->getDirection().getX();
    t tx2 = (this->getBoundingBoxMax().getX() - ray->getOrigin().getX()) / ray->getDirection().getX();

    t tmin = std::min(tx1, tx2);
    t tmax = std::max(tx1, tx2);

    t ty1 = (this->getBoundingBoxMin().getY() - ray->getOrigin().getY()) / ray->getDirection().getY();
    t ty2 = (this->getBoundingBoxMax().getY() - ray->getOrigin().getY()) / ray->getDirection().getY();

    tmin = std::max(tmin, std::min(ty1, ty2));
    tmax = std::min(tmax, std::max(ty1, ty2));

    t tz1 = (this->getBoundingBoxMin().getZ() - ray->getOrigin().getZ()) / ray->getDirection().getZ();
    t tz2 = (this->getBoundingBoxMax().getZ() - ray->getOrigin().getZ()) / ray->getDirection().getZ();

    tmin = std::max(tmin, std::min(tz1, tz2));
    tmax = std::min(tmax, std::max(tz1, tz2));

    return (tmax >= tmin && tmin < ray->getIntersectionDistance() && tmax > 0);
  }

template class BVHNode<float>;
template class BVHNode<double>;
}