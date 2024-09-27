#include "../../geometry/headers/Vector3.hpp"
#include "../../../third_party/google-test/googletest/include/gtest/gtest.h"

using namespace openviewfactor;

TEST(Vector3_Tests, test_getters) {
  Vector3<float> v(1.0,2.0,3.0);

  EXPECT_FLOAT_EQ(v.getX(), 1.0);
  EXPECT_FLOAT_EQ(v.getY(), 2.0);
  EXPECT_FLOAT_EQ(v.getZ(), 3.0);

  Vector3<float> i(1.0,0.0,0.0);
  EXPECT_FLOAT_EQ(i.getMagnitude(),1.0);
  Vector3<float> j(0.0,1.0,0.0);
  EXPECT_FLOAT_EQ(j.getMagnitude(),1.0);
  Vector3<float> k(0.0,0.0,1.0);
  EXPECT_FLOAT_EQ(k.getMagnitude(),1.0);

  Vector3<float> a(3.0,4.0,0);
  EXPECT_FLOAT_EQ(a.getMagnitude(),5.0);
}

TEST(Vector3_Tests, test_setters) {
  Vector3<float> v;

  v.setX(1.0).setY(2.0).setZ(3.0);
  
  EXPECT_FLOAT_EQ(v.getX(), 1.0);
  EXPECT_FLOAT_EQ(v.getY(), 2.0);
  EXPECT_FLOAT_EQ(v.getZ(), 3.0);
}

TEST(Vector3_Tests, test_scale_up) {
  Vector3<float> v(1.0,2.0,3.0);
  v.scale(3.0);

  EXPECT_FLOAT_EQ(v.getX(), 3.0);
  EXPECT_FLOAT_EQ(v.getY(), 6.0);
  EXPECT_FLOAT_EQ(v.getZ(), 9.0);
}
TEST(Vector3_Tests, test_scale_down) {
  Vector3<float> v(1.0,2.0,3.0);
  v.scale(0.5);

  EXPECT_FLOAT_EQ(v.getX(), 0.5);
  EXPECT_FLOAT_EQ(v.getY(), 1.0);
  EXPECT_FLOAT_EQ(v.getZ(), 1.5);
}

TEST(Vector3_Tests, test_vector_operations) {
  Vector3<float> i(1.0,0.0,0.0);
  Vector3<float> j(0.0,1.0,0.0);

  EXPECT_FLOAT_EQ(i.dot(j),0.0);
  EXPECT_FLOAT_EQ(i.dot(i),1.0);
  EXPECT_FLOAT_EQ(i.scale(2).dot(i.scale(3)),6.0);
  EXPECT_EQ(i.cross(j), Vector3<float>(0.0,0.0,1.0));
  EXPECT_EQ(i.scale(5).normalize(), Vector3<float>(1.0,0.0,0.0));
}

TEST(Vector3_Tests, test_operator_overloads) {
  Vector3<float> i(1.0,0.0,0.0);
  Vector3<float> j(0.0,1.0,0.0);
  Vector3<float> k(0.0,0.0,1.0);

  EXPECT_EQ(i + j, Vector3<float>(1.0,1.0,0.0));
  EXPECT_EQ(i - j, Vector3<float>(1.0,-1.0,0.0));

  EXPECT_TRUE(i.cross(j) == k);
  EXPECT_FALSE(i == k);
}