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

TEST(ViewFactor_Test, test_linking) {
  ViewFactor<float> vf;
  EXPECT_EQ(vf.getState(), UNLINKED);
  EXPECT_FALSE(vf.isLinked());

  STLReader<float> reader;
  auto emit = reader.getMesh(OVF_INPUT("rtg_model.stl"));
  auto receiver = reader.getMesh(OVF_INPUT("skull.stl"));

  vf.linkTriangulations(emit, emit);
  EXPECT_TRUE(vf.isLinked());
  EXPECT_EQ(vf.getState(), LINKED_ONE_MESH);
  EXPECT_EQ(*(vf.getEmitterMesh()), *emit);
  EXPECT_EQ(*(vf.getReceiverMesh()), *emit);

  vf.linkTriangulations(emit, receiver);
  EXPECT_TRUE(vf.isLinked());
  EXPECT_EQ(vf.getState(), LINKED_TWO_MESH);
  EXPECT_EQ(*(vf.getEmitterMesh()), *emit);
  EXPECT_EQ(*(vf.getReceiverMesh()), *receiver);
}

TEST(ViewFactor_Test, test_number_of_elements) {
  ViewFactor<float> vf;
  STLReader<float> reader;
  auto e = reader.getMesh(OVF_INPUT("xy_plane_unit_square_binary.stl"));
  auto r = reader.getMesh(OVF_INPUT("3_tall_box.stl"));
  EXPECT_EQ(e->getNumElements(), 2);
  EXPECT_EQ(r->getNumElements(), 28);
  vf.linkTriangulations(e,r);
  EXPECT_EQ(vf.getNumEmitterElements(), 2);
  EXPECT_EQ(vf.getNumReceiverElements(), 28);
}

TEST(ViewFactor_Test, test_number_of_triangulations) {
  ViewFactor<float> vf;
  STLReader<float> reader;
  auto e = reader.getMesh(OVF_INPUT("xy_plane_unit_square_binary.stl"));
  auto r = reader.getMesh(OVF_INPUT("3_tall_box.stl"));
  vf.linkTriangulations(e,e);
  EXPECT_EQ(vf.getNumTriangulations(), 1);

  vf.linkTriangulations(e,r);
  EXPECT_EQ(vf.getNumTriangulations(), 2);
}