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
  auto mesh = stl_reader.getMesh(OVF_INPUT("3_tall_box.stl"));

  bvh.linkToTriangulation(mesh);
  EXPECT_EQ(bvh.isLinked(), true);
}

TEST(BVH_Test, test_unlinked_construction) {
  BVH<float> bvh;
  EXPECT_THROW(bvh.constructBVH(), std::runtime_error);
}

TEST(BVH_Test, test_linked_construction) {
  STLReader<float> stl_reader;
  auto simple_mesh = stl_reader.getMesh(OVF_INPUT("xy_plane_unit_square_binary.stl"));

  BVH<float> simple_bvh;
  simple_bvh.linkToTriangulation(simple_mesh);
  simple_bvh.constructBVH();
  EXPECT_EQ(simple_bvh.getNumNodesUsed(), 1);

  auto box = stl_reader.getMesh(OVF_INPUT("3_tall_box.stl"));
  BVH<float> box_bvh;
  box_bvh.linkToTriangulation(box);
  box_bvh.constructBVH();
  box_bvh.writeToFile(OVF_OUTPUT("3_tall_box_bvh.txt"));
  EXPECT_EQ(box_bvh.getNumNodesUsed(), 17);

  auto skull = stl_reader.getMesh(OVF_INPUT("skull.stl"));
  
  BVH<float> skull_bvh;
  skull_bvh.linkToTriangulation(skull);
  skull_bvh.constructBVH();
  skull_bvh.writeToFile(OVF_OUTPUT("skull_bvh.txt"));
  EXPECT_EQ(skull_bvh.getNumNodesUsed(), 6053);
}