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

TEST(Triangle_Tests, test_vertices) {
  Triangle<float> t({0,0,0}, {1,0,0}, {0,1,0});

  EXPECT_TRUE(t.getOA() == Vector3<float>(0.0,0.0,0.0));
  EXPECT_TRUE(t.getOB() == Vector3<float>(1.0,0.0,0.0));
  EXPECT_TRUE(t.getOC() == Vector3<float>(0.0,1.0,0.0));
}

TEST(Triangle_Tests, test_edges) {
  Triangle<float> t({0.0,0.0,0.0}, {1.0,0.0,0.0}, {0.0,1.0,0.0});

  EXPECT_TRUE(t.getAB() == Vector3<float>(1.0,0.0,0.0));
  EXPECT_TRUE(t.getBC() == Vector3<float>(-1.0,1.0,0.0));
  EXPECT_TRUE(t.getCA() == Vector3<float>(0.0,-1.0,0.0));
}

TEST(Triangle_Tests, test_triangle_properties) {
  Triangle<float> t({0.0,0.0,0.0}, {1.0,0.0,0.0}, {0.0,1.0,0.0});

  EXPECT_TRUE(t.getNormal() == Vector3<float>(0.0,0.0,1.0));
  Vector3<float> c = t.getCentroid();
  EXPECT_TRUE(t.getCentroid() == Vector3<float>(1.0/3.0,1.0/3.0,0.0));
  EXPECT_TRUE(t.getArea() == 0.5);
}

TEST(Triangle_Tests, test_setters) {
  Triangle<float> t;
  t.setOA(0.0,0.0,0.0).setOB(1.0,0.0,0.0).setOC(0.0,1.0,0.0);
  
  EXPECT_TRUE(t.getOA() == Vector3<float>(0.0,0.0,0.0));
  EXPECT_TRUE(t.getOB() == Vector3<float>(1.0,0.0,0.0));
  EXPECT_TRUE(t.getOC() == Vector3<float>(0.0,1.0,0.0));
}

TEST(Triangle_Tests, test_overloads) {
  Triangle<float> t1;
  t1.setOA(0.0,0.0,0.0).setOB(1.0,0.0,0.0).setOC(0.0,1.0,0.0);

  Triangle<float> t2;
  t2.setOA(0.0,0.0,0.0).setOB(1.0,0.0,0.0).setOC(0.0,1.0,0.0);

  EXPECT_TRUE(t1 == t2);

  EXPECT_FALSE(t1 != t2);
}

TEST(Triangle_Tests, test_extremes) {
  Triangle<double> t1({{2.0/3.0, 0.0, -1.0/3.0}, {-1.0/3.0, 0.0, -1.0/3.0}, {-1.0/3.0, 0.0, 2.0/3.0}});

  EXPECT_EQ(t1.getNormal(), Vector3<double>(0.0, 1.0, 0.0));

  Triangle<double> t2({{2.0/3.0, 1.0, 0.0}, {-1.0/3.0, 2.0, 0.0}, {-1.0/3.0, 1.0, 0.0}});

  EXPECT_EQ(t2.getNormal(), Vector3<double>(0.0, 0.0, 1.0));

  auto v = t2.getCentroid() - t1.getCentroid();
  EXPECT_TRUE(v.dot(t1.getNormal()) > 0.0);
  EXPECT_DOUBLE_EQ(v.dot(t2.getNormal()), 0.0);
}