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

TEST(Solver_Test, test_null_constructor) {
  DoubleAreaIntegration<float> dai;
  EXPECT_EQ((dai.getOptions()).getSelfIntersectionMode(), BOTH);
  EXPECT_EQ((dai.getOptions()).getNumericMode(), DAI);
  EXPECT_EQ((dai.getOptions()).getComputeMode(), CPU_N);
  EXPECT_EQ((dai.getOptions()).getPrecisionMode(), DOUBLE);
}

TEST(Solver_Test, test_custom_constructor) {
  SolverOptions solver_options;
  solver_options.setSelfIntersectionMode(NONE);
  solver_options.setNumericMode(SAI);
  solver_options.setComputeMode(GPU);
  solver_options.setPrecisionMode(SINGLE);
  DoubleAreaIntegration<float> dai;
  dai.setOptions(solver_options);
  EXPECT_EQ((dai.getOptions()).getSelfIntersectionMode(), NONE);
  EXPECT_EQ((dai.getOptions()).getNumericMode(), SAI);
  EXPECT_EQ((dai.getOptions()).getComputeMode(), GPU);
  EXPECT_EQ((dai.getOptions()).getPrecisionMode(), SINGLE);
}

TEST(Solver_Test, test_backFaceCullElements) {
  DoubleAreaIntegration<double> dai;
  Triangle<double> e(Vector3<double>(0.0,0.0,0.0),Vector3<double>(1.0,0.0,0.0),Vector3<double>(0.0,1.0,0.0));
  Triangle<double> r1(Vector3<double>(0.0,0.0,1.0),Vector3<double>(1.0,0.0,1.0),Vector3<double>(0.0,1.0,1.0));
  Triangle<double> r2(Vector3<double>(0.0,0.0,1.0),Vector3<double>(0.0,1.0,1.0),Vector3<double>(1.0,0.0,1.0));
  EXPECT_TRUE(dai.backFaceCullElements(e, r1));
  EXPECT_FALSE(dai.backFaceCullElements(e, r2));
}

TEST(Solver_Test, test_evaluateBlockingBetweenElements) {
  DoubleAreaIntegration<double> dai;
  Triangle<double> e(Vector3<double>(0.0,0.0,0.0),Vector3<double>(1.0,0.0,0.0),Vector3<double>(0.0,1.0,0.0));
  Triangle<double> r(Vector3<double>(0.0,0.0,1.0),Vector3<double>(1.0,0.0,1.0),Vector3<double>(0.0,1.0,1.0));
  Triangle<double> b1(Vector3<double>(0.0,0.0,0.5),Vector3<double>(1.0,0.0,0.5),Vector3<double>(0.0,1.0,0.5));
  Triangle<double> b2(Vector3<double>(9.0,9.0,0.5),Vector3<double>(10.0,9.0,0.5),Vector3<double>(9.0,10.0,0.5));
  auto blocker1 = std::make_shared<Triangulation<double>>();
  blocker1->addElement(b1);

  auto blocker_bvh = std::make_shared<BVH<double>>();
  blocker_bvh->linkToTriangulation(blocker1);
  blocker_bvh->constructBVH();
  Blockers<double> blockers;
  blockers.addBlocker(blocker_bvh);
  EXPECT_TRUE(dai.evaluateBlockingBetweenElements(e,r,blockers));
  
  auto blocker2 = std::make_shared<Triangulation<double>>();
  blocker2->addElement(b2);
  auto blocker_bvh2 = std::make_shared<BVH<double>>();
  blocker_bvh2->linkToTriangulation(blocker2);
  blocker_bvh2->constructBVH();
  Blockers<double> blockers2;
  blockers2.addBlocker(blocker_bvh2);
  EXPECT_FALSE(dai.evaluateBlockingBetweenElements(e,r,blockers2));
}

TEST(Solver_Test, test_evaluateBlockingBetweenMeshes) {
  auto emitter_mesh = std::make_shared<Triangulation<double>>();
  auto receiver_mesh = std::make_shared<Triangulation<double>>();
  auto blocker_mesh = std::make_shared<Triangulation<double>>();

  emitter_mesh->addElement(Triangle<double>({{0.0, 0.0, 0.0},{1.0, 0.0, 0.0},{0.0, 1.0, 0.0}}));
  emitter_mesh->addElement(Triangle<double>({{10.0, 10.0, 0.0},{9.0, 10.0, 0.0},{10.0, 9.0, 0.0}}));

  receiver_mesh->addElement(Triangle<double>({{0.0, 0.0, 10.0},{1.0, 0.0, 10.0},{0.0, 1.0, 10.0}}));
  receiver_mesh->addElement(Triangle<double>({{10.0, 10.0, 10.0},{9.0, 10.0, 10.0},{10.0, 9.0, 10.0}}));

  blocker_mesh->addElement(Triangle<double>({{0.0, 0.0, 5.0},{1.0, 0.0, 5.0},{0.0, 1.0, 5.0}}));
  blocker_mesh->addElement(Triangle<double>({{10.0, 10.0, 5.0},{9.0, 10.0, 5.0},{10.0, 9.0, 5.0}}));
  auto blocker_bvh = std::make_shared<BVH<double>>();
  blocker_bvh->linkToTriangulation(blocker_mesh);
  blocker_bvh->constructBVH();
  Blockers<double> blockers;
  blockers.addBlocker(blocker_bvh);

  DoubleAreaIntegration<double> dai;

  auto unblocked_indices = dai.evaluateBlockingBetweenMeshes(emitter_mesh, receiver_mesh, blockers, std::vector<unsigned int>({0,1,2,3}));
  EXPECT_EQ(unblocked_indices[0], 1);
  EXPECT_EQ(unblocked_indices[1], 2);
}

TEST(Solver_Test, test_solveViewFactorBetweenMeshes) {
  auto emitter_mesh = std::make_shared<Triangulation<double>>();
  auto receiver_mesh = std::make_shared<Triangulation<double>>();
  auto blocker_mesh = std::make_shared<Triangulation<double>>();

  emitter_mesh->addElement(Triangle<double>({{0.0, 0.0, 0.0},{1.0, 0.0, 0.0},{0.0, 1.0, 0.0}}));
  emitter_mesh->addElement(Triangle<double>({{10.0, 10.0, 0.0},{9.0, 10.0, 0.0},{10.0, 9.0, 0.0}}));

  receiver_mesh->addElement(Triangle<double>({{0.0, 0.0, 10.0},{1.0, 0.0, 10.0},{0.0, 1.0, 10.0}}));
  receiver_mesh->addElement(Triangle<double>({{10.0, 10.0, 10.0},{9.0, 10.0, 10.0},{10.0, 9.0, 10.0}}));

  blocker_mesh->addElement(Triangle<double>({{0.0, 0.0, 5.0},{1.0, 0.0, 5.0},{0.0, 1.0, 5.0}}));
  blocker_mesh->addElement(Triangle<double>({{10.0, 10.0, 5.0},{9.0, 10.0, 5.0},{10.0, 9.0, 5.0}}));
  auto blocker_bvh = std::make_shared<BVH<double>>();
  blocker_bvh->linkToTriangulation(blocker_mesh);
  blocker_bvh->constructBVH();
  Blockers<double> blockers;
  blockers.addBlocker(blocker_bvh);

  DoubleAreaIntegration<double> dai;

  auto unblocked_indices = dai.evaluateBlockingBetweenMeshes(emitter_mesh, receiver_mesh, blockers, std::vector<unsigned int>({0,1,2,3}));
  auto solution = std::make_shared<ViewFactor<double>>();
  dai.solveViewFactorBetweenMeshes(emitter_mesh, receiver_mesh, unblocked_indices, solution);

  EXPECT_DOUBLE_EQ(solution->getMatrixElementVF(0), 0.0);
  EXPECT_DOUBLE_EQ(solution->getMatrixElementVF(1), 0.0);
  EXPECT_DOUBLE_EQ(solution->getMatrixElementVF(2), 0.0);
  EXPECT_DOUBLE_EQ(solution->getMatrixElementVF(3), 0.0);
}