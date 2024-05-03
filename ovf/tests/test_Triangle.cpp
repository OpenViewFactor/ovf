#include "geometry/include/Triangle.hpp"
#include "TestGlobals.h"
#include "gtest/gtest.h"
#include <random>
#include <string>
#include <vector>

using namespace openviewfactor;

TEST(Triangle_Tests, test_getters) {
  Triangle<float> t({0,0,0}, {1,0,0}, {0,1,0});

  EXPECT_TRUE(t.getOA() == {0,0,0});
  EXPECT_TRUE(t.getOB() == {1,0,0});
  EXPECT_TRUE(t.getOC() == {0,1,0});
}

TEST(Triangle_Tests, test_setters) {
  Triangle<float> t;

  v.setOA({0,0,0}).setOB({1,0,0}).setOC({0,1,0});
  
  EXPECT_TRUE(t.getOA() == {0,0,0});
  EXPECT_TRUE(t.getOB() == {1,0,0});
  EXPECT_TRUE(t.getOC() == {0,1,0});
}