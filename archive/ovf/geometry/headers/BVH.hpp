#ifndef OVF_BVH_H_
#define OVF_BVH_H_

#include "../../config.hpp"
#include "Vector3.hpp"
#include "Triangle.hpp"
#include "Triangulation.hpp"
#include "Ray.hpp"
#include "BVHNode.hpp"
#include "STLReader.hpp"
#include <memory>
#include <vector>
#include <numeric>
#include <fstream>
#include <iostream>
#include <stdexcept>

namespace openviewfactor {

template <typename FLOAT_TYPE> class BVH {
  private:
    //* ----- CLASS ATTRIBUTES ----- *//
    std::vector<BVHNode<FLOAT_TYPE>> _nodes;
    std::shared_ptr<Triangulation<FLOAT_TYPE>> _triangulation;
    std::vector<unsigned int> _mesh_element_indices;
    unsigned int _nodes_used;
    //TODO could be interesting to study how these affect speed
    unsigned int _minimum_triangle_threshold; //? do i really care about this
    unsigned int _num_cost_evaluation_points; //? do i really care about this

    //* ----- PRIVATE METHODS ----- *//
    OVF_HOST_DEVICE BVH<FLOAT_TYPE>& setNumNodesUsed(unsigned int n);
    OVF_HOST_DEVICE unsigned int getMaxNumNodes() const;
    OVF_HOST_DEVICE BVH<FLOAT_TYPE>& subdivideNode(unsigned int node_index);
    OVF_HOST_DEVICE unsigned int splitPrimitives(unsigned int node_index, unsigned int axis_index, FLOAT_TYPE split_location);
    OVF_HOST_DEVICE BVH<FLOAT_TYPE>& swapElements(unsigned int index_one, unsigned int index_two);
    OVF_HOST_DEVICE unsigned int createChildNodes(unsigned int node_index, unsigned int split_index, unsigned int num_triangles_on_left);
    OVF_HOST_DEVICE BVH<FLOAT_TYPE>& constructNewNode(unsigned int node_index);
    OVF_HOST_DEVICE BVH<FLOAT_TYPE>& intersectRayWithBVHNode(std::shared_ptr<Ray<FLOAT_TYPE>> ray, unsigned int node_index);
    OVF_HOST_DEVICE std::vector<unsigned int> getSubMeshIndices(unsigned int node_index) const;
  protected:
  public:
    //* ----- PUBLIC METHODS ----- *//
    OVF_HOST_DEVICE BVH();
    
    OVF_HOST_DEVICE BVH<FLOAT_TYPE>& linkToTriangulation(std::shared_ptr<Triangulation<FLOAT_TYPE>> triangulation);

    OVF_HOST_DEVICE bool isLinked() const;

    OVF_HOST_DEVICE BVH<FLOAT_TYPE>& setMinimumNumTriangles(unsigned int min_triangles);
    OVF_HOST_DEVICE BVH<FLOAT_TYPE>& setNumCostEvaluationPoints(unsigned int num_cost_evaluation_points);

    OVF_HOST_DEVICE BVH<FLOAT_TYPE>& constructBVH();

    OVF_HOST_DEVICE void intersectRayWithBVH(std::shared_ptr<Ray<FLOAT_TYPE>> ray, FLOAT_TYPE triangle_distance);

    OVF_HOST_DEVICE unsigned int getNumNodesUsed();

    OVF_HOST_DEVICE void writeToFile(const std::string& filename) const;
};

}

#endif