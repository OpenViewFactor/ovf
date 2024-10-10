#include "Vector3.hpp"
#include "Triangle.hpp"
#include "Triangulation.hpp"
#include "BVHNode.hpp"
#include "BVH.hpp"
#include "STLReader.hpp"
#include "Ray.hpp"
#include "TestGlobals.h"
#include "gtest.h"

using namespace openviewfactor;

TEST(BVH_Test, test_linking) {
  BVH<float> bvh;

  EXPECT_EQ(bvh.isLinked(), false);

  STLReader<float> stl_reader;
  Triangulation<float> mesh = stl_reader.getMesh(OVF_INPUT("3_tall_box.stl"));

  bvh.linkToTriangulation(mesh);
  EXPECT_EQ(bvh.isLinked(), true);
}

TEST(BVH_Test, test_unlinked_construction) {
  BVH<float> bvh;
  EXPECT_THROW(bvh.constructBVH(), std::runtime_error);
}

TEST(BVH_Test, test_linked_construction) {
  BVH<float> bvh;
  STLReader<float> stl_reader;

  Triangulation<float> simple_mesh = stl_reader.getMesh(OVF_INPUT("xy_plane_unit_square_binary.stl"));

  bvh.linkToTriangulation(simple_mesh);
  bvh.constructBVH();
  EXPECT_EQ(bvh.getNumNodesUsed(), 1);

  Triangulation<float> tall_box_mesh = stl_reader.getMesh(OVF_INPUT("3_tall_box.stl"));
  bvh.linkToTriangulation(tall_box_mesh);
  bvh.constructBVH();

  EXPECT_EQ(bvh.getNumNodesUsed(), 5);
}