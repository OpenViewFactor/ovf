#include "BVHNode.hpp"
#include "gtest.h"

using namespace openviewfactor;

TEST(BVHNode_Tests, test_default) {
  BVHNode<float> node;
  EXPECT_EQ(node.getFirstTriangleIndex(), 0);
  EXPECT_EQ(node.getChildOneIndex(), 0);
  EXPECT_EQ(node.getNumTriangles(), 0);
  EXPECT_EQ(node.getBoundingBoxMin(), Vector3<float>(0,0,0));
  EXPECT_EQ(node.getBoundingBoxMax(), Vector3<float>(0,0,0));
}