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

template <typename t> class BVH {
  private:
    //* ----- CLASS ATTRIBUTES ----- *//
    std::vector<BVHNode<t>> _nodes;
    std::vector<size_t> _triangulations;
    std::vector<t> _points;
    std::vector<size_t> _mesh_element_indices;
    size_t _nodes_used;
    //TODO could be interesting to study how these affect speed
    size_t _minimum_triangle_threshold; //? do i really care about this
    size_t _num_cost_evaluation_points; //? do i really care about this

    //* ----- PRIVATE METHODS ----- *//
    gpuify BVH<t>& setNumNodesUsed(size_t n);
    gpuify size_t getMaxNumNodes() const;
    gpuify BVH<t>& subdivideNode(size_t node_index);
    gpuify size_t splitPrimitives(size_t node_index, size_t axis_index, t split_location);
    gpuify BVH<t>& swapElements(size_t index_one, size_t index_two);
    gpuify size_t createChildNodes(size_t node_index, size_t split_index, size_t num_triangles_on_left);
    gpuify BVH<t>& constructNewNode(size_t node_index);
    gpuify BVH<t>& intersectRayWithBVHNode(std::shared_ptr<Ray<t>> ray, size_t node_index);
    gpuify std::vector<size_t> getSubMeshIndices(size_t node_index) const;
  protected:
  public:
    //* ----- PUBLIC METHODS ----- *//
    gpuify BVH();
    
    gpuify BVH<t>& linkToTriangulation(std::vector<size_t> &triangulations, std::vector<t> &points);

    gpuify bool isLinked() const;

    gpuify BVH<t>& setMinimumNumTriangles(size_t min_triangles);
    gpuify BVH<t>& setNumCostEvaluationPoints(size_t num_cost_evaluation_points);

    gpuify BVH<t>& constructBVH();

    gpuify void intersectRayWithBVH(std::shared_ptr<Ray<t>> ray);

    gpuify size_t getNumNodesUsed();

    gpuify void writeToFile(const std::string& filename) const;
};

}

#endif