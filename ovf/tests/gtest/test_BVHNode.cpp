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

TEST(BVHNode_Tests, test_default) {
  BVHNode<float> node;
  EXPECT_EQ(node.getFirstTriangleIndex(), 0);
  EXPECT_EQ(node.getChildOneIndex(), 0);
  EXPECT_EQ(node.getChildTwoIndex(), 1);
  EXPECT_EQ(node.getNumTriangles(), 0);
  EXPECT_EQ(node.getBoundingBoxMin(), Vector3<float>(0.0,0.0,0.0));
  EXPECT_EQ(node.getBoundingBoxMax(), Vector3<float>(0.0,0.0,0.0));
}

TEST(BVHNode_Tests, test_add_triangle) {
  BVHNode<float> node;
  EXPECT_EQ(node.getFirstTriangleIndex(), 0);
  EXPECT_EQ(node.getChildOneIndex(), 0);
  EXPECT_EQ(node.getChildTwoIndex(), 1);
  EXPECT_EQ(node.getNumTriangles(), 0);
  EXPECT_EQ(node.getBoundingBoxMin(), Vector3<float>(0.0,0.0,0.0));
  EXPECT_EQ(node.getBoundingBoxMax(), Vector3<float>(0.0,0.0,0.0));
  
  Triangle<float> t(Vector3<float>(0.0,0.0,0.0), Vector3<float>(1.0,0.0,0.0), Vector3<float>(0.0,1.0,0.0));

  node.growToIncludeTriangle(t);

  EXPECT_EQ(node.getFirstTriangleIndex(), 0);
  EXPECT_EQ(node.getChildOneIndex(), 0);
  EXPECT_EQ(node.getChildTwoIndex(), 1);
  EXPECT_EQ(node.getNumTriangles(), 1);
  EXPECT_EQ(node.getBoundingBoxMin(), Vector3<float>(0.0,0.0,0.0));
  EXPECT_EQ(node.getBoundingBoxMax(), Vector3<float>(1.0,1.0,0.0));
}

TEST(BVHNode_Tests, test_add_triangulation) {
  BVHNode<float> node;
  EXPECT_EQ(node.getFirstTriangleIndex(), 0);
  EXPECT_EQ(node.getChildOneIndex(), 0);
  EXPECT_EQ(node.getChildTwoIndex(), 1);
  EXPECT_EQ(node.getNumTriangles(), 0);
  EXPECT_EQ(node.getBoundingBoxMin(), Vector3<float>(0.0,0.0,0.0));
  EXPECT_EQ(node.getBoundingBoxMax(), Vector3<float>(0.0,0.0,0.0));

  STLReader<float> stl_reader = STLReader<float>();
  Triangulation<float> mesh = stl_reader.getMesh(OVF_INPUT("xy_plane_unit_square_binary.stl"));

  node.growToIncludeTriangulation(mesh);
  EXPECT_EQ(node.getFirstTriangleIndex(), 0);
  EXPECT_EQ(node.getChildOneIndex(), 0);
  EXPECT_EQ(node.getChildTwoIndex(), 1);
  EXPECT_EQ(node.getNumTriangles(), 2);
  EXPECT_EQ(node.getBoundingBoxMin(), Vector3<float>(0.0,0.0,0.0));
  EXPECT_EQ(node.getBoundingBoxMax(), Vector3<float>(1.0,1.0,0.0));
}

TEST(BVHNode_Tests, test_surface_area) {
  STLReader<float> stl_reader = STLReader<float>();
  Triangulation<float> simple_mesh = stl_reader.getMesh(OVF_INPUT("xy_plane_unit_square_binary.stl"));

  BVHNode<float> simple_node;
  simple_node.growToIncludeTriangulation(simple_mesh);
  EXPECT_EQ(simple_node.getSurfaceArea(), 2.0);

  Triangulation<float> box_mesh = stl_reader.getMesh(OVF_INPUT("3_tall_box.stl"));
  BVHNode<float> box_node;
  box_node.growToIncludeTriangulation(box_mesh);
  EXPECT_EQ(box_node.getSurfaceArea(), 14.0);
}

TEST(BVHNode_Tests, test_cost) {
  STLReader<float> stl_reader = STLReader<float>();
  Triangulation<float> simple_mesh = stl_reader.getMesh(OVF_INPUT("xy_plane_unit_square_binary.stl"));

  BVHNode<float> simple_node;
  simple_node.growToIncludeTriangulation(simple_mesh);
  EXPECT_EQ(simple_node.getNodeCost(), 4.0);

  Triangulation<float> box_mesh = stl_reader.getMesh(OVF_INPUT("3_tall_box.stl"));
  BVHNode<float> box_node;
  box_node.growToIncludeTriangulation(box_mesh);
  EXPECT_EQ(box_node.getNodeCost(), 392.0);
}

TEST(BVHNode_Tests, test_split_axis) {
  STLReader<float> stl_reader = STLReader<float>();
  Triangulation<float> simple_mesh = stl_reader.getMesh(OVF_INPUT("xy_plane_unit_square_binary.stl"));

  BVHNode<float> simple_node;
  simple_node.growToIncludeTriangulation(simple_mesh);
  EXPECT_EQ(simple_node.getSplitLocationAxis(), 1);

  Triangulation<float> box_mesh = stl_reader.getMesh(OVF_INPUT("3_tall_box.stl"));
  BVHNode<float> box_node;
  box_node.growToIncludeTriangulation(box_mesh);
  EXPECT_EQ(box_node.getSplitLocationAxis(), 2);
}

TEST(BVHNode_Tests, test_split_location_and_cost) {
  STLReader<float> stl_reader = STLReader<float>();
  Triangulation<float> mesh = stl_reader.getMesh(OVF_INPUT("3_tall_box.stl"));

  BVHNode<float> node;
  node.growToIncludeTriangulation(mesh);
  std::pair<float, float> location_and_cost = node.getBestSplitLocationAndCost(mesh, 1);
  EXPECT_EQ(location_and_cost.first, 1.5);

  location_and_cost = node.getBestSplitLocationAndCost(mesh, 2);
  EXPECT_EQ(location_and_cost.first, 1.0);
}