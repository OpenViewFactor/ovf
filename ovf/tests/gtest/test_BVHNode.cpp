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
  
  Triangle<float> t(Vector3<float>(0,0,0), Vector3<float>(1,0,0), Vector3<float>(0,1,0));

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