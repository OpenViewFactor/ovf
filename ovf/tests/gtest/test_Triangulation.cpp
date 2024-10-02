#include "Triangulation.hpp"
#include "gtest.h"

using namespace openviewfactor;

TEST(Triangulation_Tests, test_getters) {
  Vector3<float> p1(0,0,0);
  Vector3<float> p2(1,0,0);
  Vector3<float> p3(1,1,0);
  Vector3<float> p4(0,1,0);

  Triangulation<float> tgln;
  tgln.setPoints({p1,p2,p3,p4}).setConnectivity({{0,1,2},{0,2,3}});

  Triangulation<float> sub_mesh;
  sub_mesh.setPoints({p1,p2,p3,p4}).setConnectivity({{0,1,2}});

  EXPECT_EQ(tgln.getNumElements(), 2);
  EXPECT_EQ(tgln.getNumPoints(), 4);
  EXPECT_EQ(tgln.getNumBytes(), 48 + 48);

  EXPECT_EQ(tgln.getMeshArea(), 1);
  EXPECT_EQ(tgln.getSubMesh({0}), sub_mesh);
}

TEST(Triangulation_Tests, test_operators) {
  Vector3<float> p1(0,0,0);
  Vector3<float> p2(1,0,0);
  Vector3<float> p3(1,1,0);
  Vector3<float> p4(0,1,0);
  
  Triangulation<float> tgln;
  tgln.setPoints({p1,p2,p3,p4}).setConnectivity({{0,1,2},{0,2,3}});

  EXPECT_EQ(tgln[0], Triangle<float>(p1,p2,p3));
  EXPECT_EQ(tgln[1], Triangle<float>(p1,p3,p4));
}