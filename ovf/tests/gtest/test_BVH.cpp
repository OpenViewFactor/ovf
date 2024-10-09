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

TEST(BVH_Test, test_default) {
  BVH<float> bvh;

  EXPECT_EQ(bvh.isLinked(), false);

  STLReader<float> stl_reader;
  Triangulation<float> mesh = stl_reader.getMesh(OVF_INPUT("3_tall_box.stl"));

  bvh.linkToTriangulation(mesh);
  EXPECT_EQ(bvh.isLinked(), true);
}