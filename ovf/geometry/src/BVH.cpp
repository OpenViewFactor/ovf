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
    std::cout << "Mesh Num Elements: " << triangulation->getNumElements() << " ... Internal Mesh Num Elements: " << _triangulation->getNumElements() << std::endl;
    _mesh_element_indices = std::vector<unsigned int>((_triangulation)->getNumElements());
    std::cout << "Mesh element indices size: " << _mesh_element_indices.size() << std::endl;
    std::iota(_mesh_element_indices.begin(), _mesh_element_indices.end(), 0);
    _nodes = std::vector<BVHNode<FLOAT_TYPE>>(this->getMaxNumNodes());
    std::cout << "BVH _nodes vector size: " << _nodes.size() << std::endl;
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
    return *this;
  }

  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE void BVH<FLOAT_TYPE>::intersectRayWithBVH(Ray<FLOAT_TYPE> ray) {
    this->intersectRayWithBVHNode(ray, 1);
  }

  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE unsigned int BVH<FLOAT_TYPE>::getNumNodesUsed() { return _nodes_used; }

  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE void BVH<FLOAT_TYPE>::writeToFile(const std::string& filename) const {
    for (unsigned int i = 0; i < _nodes_used; i++) {
      std::ofstream outfile(filename + std::to_string(i) + ".txt");
      outfile << "Encoding,X,Y,Z\n";
      (_nodes[i]).writeToFile(outfile);
    }
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

    std::cout << ">> Node Subdivision Initiated for BVHNode: " << node_index << std::endl;

    //* --------------- [1] --------------- *//
    BVHNode<FLOAT_TYPE> current_node = _nodes[node_index];
    if (current_node.getNumTriangles() <= _minimum_triangle_threshold) { return *this; }
    
    std::cout << ">>> BVHNode " << node_index << " contains enough triangles to proceed" << std::endl;

    //* --------------- [2] --------------- *//
    unsigned int axis_index = current_node.getSplitLocationAxis();
    std::pair<FLOAT_TYPE, FLOAT_TYPE> location_and_cost = current_node.getBestSplitLocationAndCost(_triangulation, axis_index, _num_cost_evaluation_points);
    FLOAT_TYPE best_location = location_and_cost.first;
    FLOAT_TYPE best_cost = location_and_cost.second;
    //* --------------- [3] --------------- *//
    if (current_node.getNodeCost() <= best_cost) { return *this; }

    std::cout << ">>> BVHNode " << node_index << " cost analysis is favorable to proceed" << std::endl;

    //* --------------- [4] --------------- *//
    unsigned int split_index = this->splitPrimitives(node_index, axis_index, best_location);
    //* --------------- [5] --------------- *//
    unsigned int num_triangles_on_left = split_index - current_node.getFirstTriangleIndex();
    if (num_triangles_on_left == 0 || num_triangles_on_left == current_node.getNumTriangles()) { return *this; }

    std::cout << ">>> BVHNode " << node_index << " actually gets split at index " << split_index << " with " << num_triangles_on_left << " triangles on the 'left'" << std::endl;

    //* --------------- [6] --------------- *//
    unsigned int left_child_index = this->createChildNodes(node_index, split_index, num_triangles_on_left);
    //* --------------- [7] --------------- *//
    this->subdivideNode(left_child_index);
    this->subdivideNode(left_child_index + 1);

    return *this;
  }

  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE unsigned int BVH<FLOAT_TYPE>::splitPrimitives(unsigned int node_index, unsigned int axis_index, FLOAT_TYPE split_location) {

    std::cout << ">>>> splitPrimitives Initiated for BVHNode: " << node_index << " -- Split Axis: " << axis_index << " -- Split Location: " << split_location << std::endl;

    BVHNode<FLOAT_TYPE> current_node = _nodes[node_index];
    
    unsigned int index_at_which_to_split = current_node.getFirstTriangleIndex();
    unsigned int index_of_last_unsorted_element = index_at_which_to_split + current_node.getNumTriangles() - 1;

    while (index_at_which_to_split <= index_of_last_unsorted_element) {

      // std::cout << ">>>>> Sorting primitives. Current index_at_which_to_split: " << index_at_which_to_split << " ... Current index_of_last_unsorted_element: " << index_of_last_unsorted_element << ". ";

      // if (index_at_which_to_split == index_of_last_unsorted_element) {
      //   std::cout << "Indices are EQUAL" << std::endl;
      // } else {
      //   std::cout << "Indices are NOT EQUAL" << std::endl;
      // }

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
    BVHNode<FLOAT_TYPE> current_node = _nodes[node_index];

    unsigned int left_child_index = this->getNumNodesUsed();
    
    BVHNode<FLOAT_TYPE> left_child_node;
    BVHNode<FLOAT_TYPE> right_child_node;
    _nodes.push_back(left_child_node);
    _nodes.push_back(right_child_node);
    this->setNumNodesUsed(this->getNumNodesUsed() + 2);

    current_node.setChildOneIndex(left_child_index);

    (_nodes[left_child_index]).setFirstTriangleIndex(current_node.getFirstTriangleIndex());
    (_nodes[left_child_index]).setNumTriangles(num_triangles_on_left);

    (_nodes[left_child_index + 1]).setFirstTriangleIndex(split_index);
    (_nodes[left_child_index + 1]).setNumTriangles(current_node.getNumTriangles() - num_triangles_on_left);

    current_node.setNumTriangles(0);

    (this->constructNewNode(left_child_index)).constructNewNode(left_child_index + 1);

    return left_child_index;
  }

  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE BVH<FLOAT_TYPE>& BVH<FLOAT_TYPE>::constructNewNode(unsigned int node_index) {
    (_nodes[node_index]).growToIncludeTriangulation( _triangulation->getSubMesh( this->getSubMeshIndices(node_index) ) );
    return *this;
  }

  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE BVH<FLOAT_TYPE>& BVH<FLOAT_TYPE>::intersectRayWithBVHNode(Ray<FLOAT_TYPE> ray, unsigned int node_index) {
    BVHNode<FLOAT_TYPE> current_node = _nodes[node_index];
    if (!(current_node.intersectRayWithNodeBoundingBox(ray))) { return *this; }
    if (current_node.isLeaf()) {
      for (unsigned int i = current_node.getFirstTriangleIndex(); i < current_node.getFirstTriangleIndex() + current_node.getNumTriangles(); i++) {
        ray.triangleIntersection((*_triangulation)[_mesh_element_indices[i]]);
      }
    } else {
      (this->intersectRayWithBVHNode(ray, current_node.getChildOneIndex())).intersectRayWithBVHNode(ray, current_node.getChildTwoIndex());
    }
    return *this;
  }

  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE std::vector<unsigned int> BVH<FLOAT_TYPE>::getSubMeshIndices(unsigned int node_index) const {
    BVHNode<FLOAT_TYPE> current_node = _nodes[node_index];
    std::vector<unsigned int> submesh_indices = current_node.getElementArraySubindices();
    return submesh_indices;
  }

template class BVH<float>;
template class BVH<double>;
}