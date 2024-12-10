#include "Vector3.hpp"
#include "Triangle.hpp"
#include "Triangulation.hpp"
#include "BVHNode.hpp"
#include "BVH.hpp"
#include "STLReader.hpp"
#include "Ray.hpp"
#include "Solver.hpp"
#include "DoubleAreaIntegration.hpp"
#include "TestGlobals.h"
#include "gtest.h"

using namespace openviewfactor;

TEST(DAI_Test, test_NullConstructor) {
  DoubleAreaIntegration<float> dai();
}