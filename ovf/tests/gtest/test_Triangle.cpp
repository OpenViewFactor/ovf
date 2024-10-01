#include "Triangle.hpp"
#include "gtest.h"
#include <random>
#include <string>
#include <vector>

using namespace openviewfactor;

TEST(Triangle_Tests, test_getters) {
  Triangle<float> t({0,0,0}, {1,0,0}, {0,1,0});

  EXPECT_TRUE(t.getOA() == Vector3<float>(0,0,0));
  EXPECT_TRUE(t.getOB() == Vector3<float>(1,0,0));
  EXPECT_TRUE(t.getOC() == Vector3<float>(0,1,0));
}

TEST(Triangle_Tests, test_setters) {
  Triangle<float> t;
  t.setOA(0,0,0).setOB(1,0,0).setOC(0,1,0);
  
  EXPECT_TRUE(t.getOA() == Vector3<float>(0,0,0));
  EXPECT_TRUE(t.getOB() == Vector3<float>(1,0,0));
  EXPECT_TRUE(t.getOC() == Vector3<float>(0,1,0));
}