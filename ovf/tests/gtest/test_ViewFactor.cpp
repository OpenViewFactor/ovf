#include "Vector3.hpp"
#include "Triangle.hpp"
#include "Triangulation.hpp"
#include "BVHNode.hpp"
#include "BVH.hpp"
#include "STLReader.hpp"
#include "Ray.hpp"

#include "IndexValuePair.hpp"
#include "ViewFactor.hpp"

#include "TestGlobals.h"
#include "gtest.h"

using namespace openviewfactor;

TEST(ViewFactor_Test, test_constructor) {
  ViewFactor<float> vf;
  EXPECT_EQ(vf.getState(), UNLINKED);
  EXPECT_EQ(vf.getEmitterMesh(), nullptr);
  EXPECT_EQ(vf.getReceiverMesh(), nullptr);
  EXPECT_EQ(vf.getMatrixElementVF(0), 0.0);
}