#include "geometry/include/Vector3.hpp"
#include "TestGlobals.h"
#include "gtest/gtest.h"
#include <random>
#include <string>
#include <vector>

using namespace openviewfactor;

TEST(Vector3_Tests, test_getters) {
  Vector3<float> v(1, 2, 3);

  EXPECT_FLOAT_EQ(v.getX(), 1);
  EXPECT_FLOAT_EQ(v.getY(), 2);
  EXPECT_FLOAT_EQ(v.getZ(), 3);
}

TEST(Vector3_Tests, test_setters) {
  Vector3<float> v;

  v.setX(1).setY(2).setZ(3);
  
  EXPECT_FLOAT_EQ(v.getX(), 1);
  EXPECT_FLOAT_EQ(v.getY(), 2);
  EXPECT_FLOAT_EQ(v.getZ(), 3);
}