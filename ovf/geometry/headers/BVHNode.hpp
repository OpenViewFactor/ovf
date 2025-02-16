#ifndef OVF_BVHNODE_H_
#define OVF_BVHNODE_H_

#include "../../config.hpp"
#include "Vector3.hpp"
#include "Triangle.hpp"
#include "Triangulation.hpp"
#include "Ray.hpp"
#include <cmath>
#include <algorithm>
#include <numeric>
#include <utility>
#include <memory>
#include <fstream>
#include <iostream>

namespace openviewfactor {

template <typename t> class BVHNode {
  private:
    Vector3<t> _bounding_box_min;
    Vector3<t> _bounding_box_max;
    unsigned int _child_one_index;
    unsigned int _first_triangle_index;
    unsigned int _num_triangles;
  protected:
  public:
    gpuify BVHNode();

    gpuify Vector3<t> getBoundingBoxMin() const;
    gpuify Vector3<t> getBoundingBoxMax() const;
    gpuify Vector3<t> getBoundingBoxSpan() const;

    gpuify unsigned int getChildOneIndex() const;
    gpuify unsigned int getChildTwoIndex() const;
    gpuify unsigned int getFirstTriangleIndex() const;
    gpuify unsigned int getNumTriangles() const;

    gpuify bool isLeaf() const;

    gpuify BVHNode<t>& setChildOneIndex(unsigned int const index);
    gpuify BVHNode<t>& setFirstTriangleIndex(unsigned int const index);
    gpuify BVHNode<t>& setNumTriangles(unsigned int const n);

    gpuify BVHNode<t>& growToIncludeTriangle(Triangle<t> triangle);
    gpuify BVHNode<t>& growToIncludeTriangulation(std::shared_ptr<Triangulation<t>> triangulation);

    gpuify bool intersectRayWithNodeBoundingBox(std::shared_ptr<Ray<t>> ray) const;

    gpuify t getNodeCost() const;
    gpuify t getSurfaceArea() const;

    gpuify unsigned int getSplitLocationAxis() const;
    gpuify t evaluateNodeChildrenSurfaceAreaHeuristic(std::shared_ptr<Triangulation<t>> submesh, unsigned int axis_index, t candidate_position) const;
    gpuify std::pair<t, t> getBestSplitLocationAndCost(std::shared_ptr<Triangulation<t>> triangulation, std::vector<unsigned int> mesh_element_indices, unsigned int axis_index, unsigned int num_evaluation_points) const;

    gpuify std::vector<unsigned int> getElementArraySubindices() const;

    gpuify void writeToFile(std::ofstream& outfile, unsigned int node_index) const;
};

}

#endif