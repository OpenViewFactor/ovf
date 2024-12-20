#include "BVH.hpp"

namespace openviewfactor {

  //* ------------------------------ PUBLIC METHODS ------------------------------ *//

  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE BVH<FLOAT_TYPE>::BVH()
    : _nodes(std::vector<BVHNode<FLOAT_TYPE>>()), _triangulation(std::make_shared<Triangulation<FLOAT_TYPE>>()), _mesh_element_indices(std::vector<unsigned int>()), _nodes_used(0), _minimum_triangle_threshold(2), _num_cost_evaluation_points(4) {}

  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE BVH<FLOAT_TYPE>& BVH<FLOAT_TYPE>::linkToTriangulation(std::shared_ptr<Triangulation<FLOAT_TYPE>> triangulation) {
    if (this->isLinked()) {
      _triangulation->clear();
      _mesh_element_indices.clear();
      _nodes.clear();
      _nodes_used = 0;
    }

    _triangulation = triangulation;
    _mesh_element_indices = std::vector<unsigned int>((_triangulation)->getNumElements());
    std::iota(_mesh_element_indices.begin(), _mesh_element_indices.end(), 0);
    _nodes = std::vector<BVHNode<FLOAT_TYPE>>(this->getMaxNumNodes());
    return *this;
  }

  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE bool BVH<FLOAT_TYPE>::isLinked() const { return !(*_triangulation == Triangulation<FLOAT_TYPE>()); }

  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE BVH<FLOAT_TYPE>& BVH<FLOAT_TYPE>::setMinimumNumTriangles(unsigned int min_triangles) {
    _minimum_triangle_threshold = min_triangles;
    return *this;
  }
  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE BVH<FLOAT_TYPE>& BVH<FLOAT_TYPE>::setNumCostEvaluationPoints(unsigned int num_cost_evaluation_points) {
    _num_cost_evaluation_points = num_cost_evaluation_points;
    return *this;
  }

  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE BVH<FLOAT_TYPE>& BVH<FLOAT_TYPE>::constructBVH() {
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

  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE void BVH<FLOAT_TYPE>::intersectRayWithBVH(std::shared_ptr<Ray<FLOAT_TYPE>> ray) {
    this->intersectRayWithBVHNode(ray, 0);
  }

  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE unsigned int BVH<FLOAT_TYPE>::getNumNodesUsed() { return _nodes_used; }

  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE void BVH<FLOAT_TYPE>::writeToFile(const std::string& filename) const {
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

  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE BVH<FLOAT_TYPE>& BVH<FLOAT_TYPE>::setNumNodesUsed(unsigned int n) {
    _nodes_used = n;
    return *this;
  }

  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE unsigned int BVH<FLOAT_TYPE>::getMaxNumNodes() const {
    if (!(this->isLinked())) {
      throw std::runtime_error("BVH is not linked to a triangulation. Cannot evaluate the maximum number of elements");
    }
    return (2 * (_triangulation->getNumElements()) - 1);
  }

  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE BVH<FLOAT_TYPE>& BVH<FLOAT_TYPE>::subdivideNode(unsigned int node_index) {
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
    std::pair<FLOAT_TYPE, FLOAT_TYPE> location_and_cost = (_nodes[node_index]).getBestSplitLocationAndCost(_triangulation, _mesh_element_indices, axis_index, _num_cost_evaluation_points);
    FLOAT_TYPE best_location = location_and_cost.first;
    FLOAT_TYPE best_cost = location_and_cost.second;
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

  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE unsigned int BVH<FLOAT_TYPE>::splitPrimitives(unsigned int node_index, unsigned int axis_index, FLOAT_TYPE split_location) {
    
    unsigned int index_at_which_to_split = (_nodes[node_index]).getFirstTriangleIndex();
    unsigned int index_of_last_unsorted_element = index_at_which_to_split + (_nodes[node_index]).getNumTriangles() - 1;

    while (index_at_which_to_split <= index_of_last_unsorted_element) {
      Triangle<FLOAT_TYPE> current_element = (*_triangulation)[_mesh_element_indices[index_at_which_to_split]];
      if ((current_element.getCentroid())[axis_index] < split_location) {
        index_at_which_to_split++;
      } else {
        this->swapElements(index_at_which_to_split, index_of_last_unsorted_element);
        index_of_last_unsorted_element--;
      }
    }

    return index_at_which_to_split; //! this is the index of the first element on the side > split_location
  }

  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE BVH<FLOAT_TYPE>& BVH<FLOAT_TYPE>::swapElements(unsigned int index_one, unsigned int index_two) {
    auto temporary = _mesh_element_indices[index_one];
    _mesh_element_indices[index_one] = _mesh_element_indices[index_two];
    _mesh_element_indices[index_two] = temporary;
    return *this;
  }

  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE unsigned int BVH<FLOAT_TYPE>::createChildNodes(unsigned int node_index, unsigned int split_index, unsigned int num_triangles_on_left) {
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

  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE BVH<FLOAT_TYPE>& BVH<FLOAT_TYPE>::constructNewNode(unsigned int node_index) {
    std::vector<unsigned int> submesh_indices = this->getSubMeshIndices(node_index);
    auto submesh = _triangulation->getSubMesh(submesh_indices);
    (_nodes[node_index]).growToIncludeTriangulation( submesh );
    return *this;
  }

  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE std::vector<unsigned int> BVH<FLOAT_TYPE>::getSubMeshIndices(unsigned int node_index) const {
    std::vector<unsigned int> element_array_indices = (_nodes[node_index]).getElementArraySubindices();
    std::vector<unsigned int> submesh_indices(element_array_indices.size());
    for (int i = 0; i < element_array_indices.size(); i++) {
      submesh_indices[i] = _mesh_element_indices[element_array_indices[i]];
    }
    return submesh_indices;
  }

  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE BVH<FLOAT_TYPE>& BVH<FLOAT_TYPE>::intersectRayWithBVHNode(std::shared_ptr<Ray<FLOAT_TYPE>> ray, unsigned int node_index) {
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