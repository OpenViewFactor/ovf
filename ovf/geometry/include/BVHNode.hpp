#ifndef OVF_BVHNODE_H_
#define OVF_BVHNODE_H_

#include "config.hpp"
#include "geometry/include/Vector3.hpp"
#include "geometry/include/Triangle.hpp"
#include "geometry/include/Triangulation.hpp"
#include "geometry/include/Ray.hpp"
#include <algorithm>

namespace openviewfactor {

template <typename FLOAT_TYPE> class BVHNode {
  private:
    Vector3<FLOAT_TYPE> _bounding_box_min;
    Vector3<FLOAT_TYPE> _bounding_box_max;
    unsigned int _child_one_index;
    unsigned int _first_triangle_index;
    unsigned int _num_triangles;

  public:
    OVF_HOST_DEVICE BVHNode();

    OVF_HOST_DEVICE Vector3<FLOAT_TYPE> getBoundingBoxMin() const;
    OVF_HOST_DEVICE Vector3<FLOAT_TYPE> getBoundingBoxMax() const;
    OVF_HOST_DEVICE Vector3<FLOAT_TYPE> getBoundingBoxSpan() const;

    OVF_HOST_DEVICE unsigned int getChildOneIndex() const;
    OVF_HOST_DEVICE unsigned int getChildTwoIndex() const;
    OVF_HOST_DEVICE unsigned int getFirstTriangleIndex() const;
    OVF_HOST_DEVICE unsigned int getNumTriangles() const;

    OVF_HOST_DEVICE BVHNode<FLOAT_TYPE>& setChildOneIndex(unsigned int const index);
    OVF_HOST_DEVICE BVHNode<FLOAT_TYPE>& setFirstTriangleIndex(unsigned int const index);
    OVF_HOST_DEVICE BVHNode<FLOAT_TYPE>& setNumTriangles(unsigned int const n);

    OVF_HOST_DEVICE BVHNode<FLOAT_TYPE>& growToIncludeTriangle(Triangle<FLOAT_TYPE> triangle);
    OVF_HOST_DEVICE BVHNode<FLOAT_TYPE>& growToIncludeTriangulation(const Triangulation<FLOAT_TYPE> &triangulation);

    OVF_HOST_DEVICE bool intersectRayWithNodeBoundingBox(Ray<FLOAT_TYPE> ray) const;

    OVF_HOST_DEVICE FLOAT_TYPE getNodeCost() const;
    OVF_HOST_DEVICE FLOAT_TYPE getSurfaceArea() const;

    OVF_HOST_DEVICE unsigned int getSplitLocationAxis() const;
    OVF_HOST_DEVICE FLOAT_TYPE evaluateNodeChildrenSurfaceAreaHeuristic(const Triangulation<FLOAT_TYPE> &submesh, unsigned int axis_index, FLOAT_TYPE candidate_position) const;
    OVF_HOST_DEVICE std::pair<FLOAT_TYPE, FLOAT_TYPE> getBestSplitLocationAndCost(const Triangulation<FLOAT_TYPE> &triangulation, unsigned int num_evaluation_points) const;

    OVF_HOST_DEVICE std::vector<unsigned int> getElementArraySubindices() const;
};

}

#endif