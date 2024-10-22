#include "Vector3.hpp"
#include "Triangle.hpp"
#include "Triangulation.hpp"
#include "BVHNode.hpp"
#include "BVH.hpp"
#include "STLReader.hpp"
#include "Ray.hpp"

#include "IndexValuePair.hpp"

#include "TestGlobals.h"
#include "gtest.h"

using namespace openviewfactor;

TEST(IndexValuePair_Test, test_constructor) {
  IndexValuePair<float> ivp;
  EXPECT_EQ(ivp.getFullMatrixIndex(), 0);
  EXPECT_EQ(ivp.getValue(), 0.0);
}