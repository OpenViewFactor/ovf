#include "BVH.hpp"

namespace openviewfactor {

  //* ------------------------------ PUBLIC METHODS ------------------------------ *//

  template <typename t>
  gpuify BVH<t>::BVH()
    : _nodes(std::vector<BVHNode<t>>()), _triangulations(std::vector<size_t>()), _points(std::vector<t>()), _mesh_element_indices(std::vector<size_t>()), _nodes_used(0), _minimum_triangle_threshold(2), _num_cost_evaluation_points(4) {}

  template <typename t>
  gpuify BVH<t>& BVH<t>::linkToTriangulation(std::vector<size_t> &triangulations, std::vector<t> &points) {
    if (this->isLinked()) {
      _triangulations.clear();
      _points.clear()
      _mesh_element_indices.clear();
      _nodes.clear();
      _nodes_used = 0;
    }
    _triangulations = triangulations;
    _points = points;
    _mesh_element_indices = std::vector<size_t>(triangulations.size()/3);
    std::iota(_mesh_element_indices.begin(), _mesh_element_indices.end(), 0);
    _nodes = std::vector<BVHNode<t>>(this->getMaxNumNodes());
    return *this;
  }

  template <typename t>
  gpuify bool BVH<t>::isLinked() const { return !(*_triangulation == Triangulation<t>()); }

  template <typename t>
  gpuify BVH<t>& BVH<t>::setMinimumNumTriangles(unsigned int min_triangles) {
    _minimum_triangle_threshold = min_triangles;
    return *this;
  }
  template <typename t>
  gpuify BVH<t>& BVH<t>::setNumCostEvaluationPoints(unsigned int num_cost_evaluation_points) {
    _num_cost_evaluation_points = num_cost_evaluation_points;
    return *this;
  }

  template <typename t>
  gpuify BVH<t>& BVH<t>::constructBVH() {
    if (!(this->isLinked())) {
      throw std::runtime_error("BVH is not linked to a triangulation. Cannot construct the BVH");
    }
    std::cout << "> BVH Construction Initiated" << std::endl;
    unsigned int root_node_index = 0;
    _nodes[root_node_index].growToIncludeTriangulation(_triangulation);
    this->setNumNodesUsed(1);
    this->subdivideNode(root_node_index);
    std::cout << "> BVH Construction Completed. " << this->getNumNodesUsed() << " Nodes used." << std::endl;
    return *this;
  }

  template <typename t>
  gpuify void BVH<t>::intersectRayWithBVH(std::shared_ptr<Ray<t>> ray) {
    this->intersectRayWithBVHNode(ray, 0);
  }

  template <typename t>
  gpuify unsigned int BVH<t>::getNumNodesUsed() { return _nodes_used; }

  template <typename t>
  gpuify void BVH<t>::writeToFile(const std::string& filename) const {
    std::ofstream outfile(filename + ".txt");
    outfile << "Encoding,X,Y,Z\n";
    unsigned int j = 0;
    for (unsigned int i = 0; i < _nodes_used; i++) {
      auto node = _nodes[i];
      if (node.isLeaf()) {
        node.writeToFile(outfile, j++);
      }
    }
    outfile.close();
  }

  //* ------------------------------ PRIVATE METHODS ------------------------------ *//

  template <typename t>
  gpuify BVH<t>& BVH<t>::setNumNodesUsed(unsigned int n) {
    _nodes_used = n;
    return *this;
  }

  template <typename t>
  gpuify unsigned int BVH<t>::getMaxNumNodes() const {
    if (!(this->isLinked())) {
      throw std::runtime_error("BVH is not linked to a triangulation. Cannot evaluate the maximum number of elements");
    }
    return (2 * (_triangulation->getNumElements()) - 1);
  }

  template <typename t>
  gpuify BVH<t>& BVH<t>::subdivideNode(unsigned int node_index) {
    //* --------------- FUNCTION ACTIONS --------------- *//
    //* [1] skip subdivision if current iteration contains too few triangles
    //* [2] determine cardinal axis, position along it, and cost for splitting the node
    //* [3] skip subdivision if current iteration is already lower cost than a possible subdivision
    //* [4] split the group of primitives in two halves using the split plane
    //* [5] skip subdivision if the "split" contains either no triangles or all the triangles
    //* [6] create child nodes for each half of the current node
    //* [7] recurse into each of the child nodes

    //* --------------- [1] --------------- *//
    if ((_nodes[node_index]).getNumTriangles() <= _minimum_triangle_threshold) { return *this; }

    //* --------------- [2] --------------- *//
    unsigned int axis_index = (_nodes[node_index]).getSplitLocationAxis();
    std::pair<t, t> location_and_cost = (_nodes[node_index]).getBestSplitLocationAndCost(_triangulation, _mesh_element_indices, axis_index, _num_cost_evaluation_points);
    t best_location = location_and_cost.first;
    t best_cost = location_and_cost.second;
    //* --------------- [3] --------------- *//
    if ((_nodes[node_index]).getNodeCost() <= best_cost) { return *this; }

    //* --------------- [4] --------------- *//
    unsigned int split_index = this->splitPrimitives(node_index, axis_index, best_location);
    //* --------------- [5] --------------- *//
    unsigned int num_triangles_on_left = split_index - (_nodes[node_index]).getFirstTriangleIndex();
    if (num_triangles_on_left == 0 || num_triangles_on_left == (_nodes[node_index]).getNumTriangles()) { return *this; }

    //* --------------- [6] --------------- *//
    unsigned int left_child_index = this->createChildNodes(node_index, split_index, num_triangles_on_left);
    //* --------------- [7] --------------- *//
    this->subdivideNode(left_child_index);
    this->subdivideNode(left_child_index + 1);

    return *this;
  }

  template <typename t>
  gpuify unsigned int BVH<t>::splitPrimitives(unsigned int node_index, unsigned int axis_index, t split_location) {
    
    unsigned int index_at_which_to_split = (_nodes[node_index]).getFirstTriangleIndex();
    unsigned int index_of_last_unsorted_element = index_at_which_to_split + (_nodes[node_index]).getNumTriangles() - 1;

    while (index_at_which_to_split <= index_of_last_unsorted_element) {
      Triangle<t> current_element = (*_triangulation)[_mesh_element_indices[index_at_which_to_split]];
      if ((current_element.getCentroid())[axis_index] < split_location) {
        index_at_which_to_split++;
      } else {
        this->swapElements(index_at_which_to_split, index_of_last_unsorted_element);
        index_of_last_unsorted_element--;
      }
    }

    return index_at_which_to_split; //! this is the index of the first element on the side > split_location
  }

  template <typename t>
  gpuify BVH<t>& BVH<t>::swapElements(unsigned int index_one, unsigned int index_two) {
    auto temporary = _mesh_element_indices[index_one];
    _mesh_element_indices[index_one] = _mesh_element_indices[index_two];
    _mesh_element_indices[index_two] = temporary;
    return *this;
  }

  template <typename t>
  gpuify unsigned int BVH<t>::createChildNodes(unsigned int node_index, unsigned int split_index, unsigned int num_triangles_on_left) {
    unsigned int left_child_index = this->getNumNodesUsed();

    this->setNumNodesUsed(left_child_index + 2);

    (_nodes[node_index]).setChildOneIndex(left_child_index);

    (_nodes[left_child_index]).setFirstTriangleIndex((_nodes[node_index]).getFirstTriangleIndex());
    (_nodes[left_child_index]).setNumTriangles(num_triangles_on_left);

    (_nodes[left_child_index + 1]).setFirstTriangleIndex(split_index);
    (_nodes[left_child_index + 1]).setNumTriangles((_nodes[node_index]).getNumTriangles() - num_triangles_on_left);

    (_nodes[node_index]).setNumTriangles(0);

    this->constructNewNode(left_child_index);
    this->constructNewNode(left_child_index + 1);

    return left_child_index;
  }

  template <typename t>
  gpuify BVH<t>& BVH<t>::constructNewNode(unsigned int node_index) {
    auto submesh = _triangulation->getSubMesh(this->getSubMeshIndices(node_index));
    (_nodes[node_index]).growToIncludeTriangulation( submesh );
    return *this;
  }

  template <typename t>
  gpuify std::vector<unsigned int> BVH<t>::getSubMeshIndices(unsigned int node_index) const {
    std::vector<unsigned int> element_array_indices = (_nodes[node_index]).getElementArraySubindices();
    std::vector<unsigned int> submesh_indices(element_array_indices.size());
    for (int i = 0; i < element_array_indices.size(); i++) {
      submesh_indices[i] = _mesh_element_indices[element_array_indices[i]];
    }
    return submesh_indices;
  }

  template <typename t>
  gpuify BVH<t>& BVH<t>::intersectRayWithBVHNode(std::shared_ptr<Ray<t>> ray, unsigned int node_index) {
    bool ray_intersects_with_node_bounding_box = (_nodes[node_index]).intersectRayWithNodeBoundingBox(ray);
    if (!ray_intersects_with_node_bounding_box) { return *this; }
    if ((_nodes[node_index]).isLeaf()) {
      for (unsigned int i = 0; i < (_nodes[node_index]).getNumTriangles(); i++) {
        ray->triangleIntersection((*_triangulation)[_mesh_element_indices[i + (_nodes[node_index]).getFirstTriangleIndex()]]);
      }
    } else {
      this->intersectRayWithBVHNode(ray, (_nodes[node_index]).getChildOneIndex());
      this->intersectRayWithBVHNode(ray, (_nodes[node_index]).getChildTwoIndex());
    }
    return *this;
  }

template class BVH<float>;
template class BVH<double>;
}