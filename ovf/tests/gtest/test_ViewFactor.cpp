#include "Vector3.hpp"
#include "Triangle.hpp"
#include "Triangulation.hpp"
#include "BVHNode.hpp"
#include "BVH.hpp"
#include "STLReader.hpp"
#include "Ray.hpp"

#include "ViewFactor.hpp"

#include "TestGlobals.h"
#include "gtest.h"

using namespace openviewfactor;

TEST(ViewFactor_Test, test_constructor) {
  ViewFactor<float> vf;
  EXPECT_EQ(vf.getState(), UNLINKED);
  EXPECT_EQ(vf.getEmitterMesh(), nullptr);
  EXPECT_EQ(vf.getReceiverMesh(), nullptr);
  // EXPECT_THROW(vf.getMatrixElementVF(0), std::runtime_error);
}

TEST(ViewFactor_Test, test_linking) {
  ViewFactor<float> vf;
  EXPECT_EQ(vf.getState(), UNLINKED);
  EXPECT_FALSE(vf.isLinked());

  STLReader<float> reader;
  auto emit = reader.getMesh(OVF_INPUT("rtg_model.stl"));
  auto receiver = reader.getMesh(OVF_INPUT("skull.stl"));

  vf.linkTriangulations(emit, emit, emit->getNumElements()*emit->getNumElements());
  EXPECT_TRUE(vf.isLinked());
  EXPECT_EQ(vf.getState(), LINKED_ONE_MESH);
  EXPECT_EQ(*(vf.getEmitterMesh()), *emit);
  EXPECT_EQ(*(vf.getReceiverMesh()), *emit);

  vf.linkTriangulations(emit, receiver, emit->getNumElements()*receiver->getNumElements());
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
  vf.linkTriangulations(e,r, 2*28);
  EXPECT_EQ(vf.getNumEmitterElements(), 2);
  EXPECT_EQ(vf.getNumReceiverElements(), 28);
}

TEST(ViewFactor_Test, test_number_of_triangulations) {
  ViewFactor<float> vf;
  STLReader<float> reader;
  auto e = reader.getMesh(OVF_INPUT("xy_plane_unit_square_binary.stl"));
  auto r = reader.getMesh(OVF_INPUT("3_tall_box.stl"));
  vf.linkTriangulations(e,e, 4);
  EXPECT_EQ(vf.getNumTriangulations(), 1);

  vf.linkTriangulations(e,r, e->getNumElements() * r->getNumElements());
  EXPECT_EQ(vf.getNumTriangulations(), 2);
}

TEST(ViewFactor_Test, test_matrix_elements) {
  ViewFactor<float> vf;
  STLReader<float> reader;
  auto e = reader.getMesh(OVF_INPUT("xy_plane_unit_square_binary.stl"));
  auto r = reader.getMesh(OVF_INPUT("xy_plane_offset_unit_square_binary.stl"));

  // EXPECT_THROW(vf.getMatrixElementVF(0), std::runtime_error);
  // EXPECT_THROW(vf.getEmitterElementToReceiverSurfaceVF(0), std::runtime_error);
  // EXPECT_THROW(vf.getReceiverElementToEmitterSurfaceVF(0), std::runtime_error);
  EXPECT_THROW(vf.getSurfaceToSurfaceAverageVF(), std::runtime_error);

  vf.linkTriangulations(e,r, 4);

  // EXPECT_THROW(vf.setElement(4,0.5), std::runtime_error);

  //* THE FOLLOWING VALUES ARE NOT PHYSICALLY ACCURATE, BUT THEY FULFILL THE PURPOSES OF THIS TEST
  // EXPECT_NO_THROW(vf.setElement(0,0.2));
  // EXPECT_NO_THROW(vf.setElement(1,0.3));
  // EXPECT_NO_THROW(vf.setElement(2,0.3));
  // EXPECT_NO_THROW(vf.setElement(3,0.2));
  
  // vf.setElement(0,0.2);
  // vf.setElement(1,0.3);
  // vf.setElement(2,0.3);
  // vf.setElement(3,0.2);

  // EXPECT_FLOAT_EQ(vf.getMatrixElementVF(0), 0.2);
  // EXPECT_FLOAT_EQ(vf.getMatrixElementVF(1), 0.3);
  // EXPECT_FLOAT_EQ(vf.getMatrixElementVF(2), 0.3);
  // EXPECT_FLOAT_EQ(vf.getMatrixElementVF(3), 0.2);

  // EXPECT_FLOAT_EQ(vf.getEmitterElementToReceiverSurfaceVF(0), 0.5);
  // EXPECT_FLOAT_EQ(vf.getEmitterElementToReceiverSurfaceVF(1), 0.5);
  // EXPECT_FLOAT_EQ(vf.getReceiverElementToEmitterSurfaceVF(0), 0.5);
  // EXPECT_FLOAT_EQ(vf.getReceiverElementToEmitterSurfaceVF(1), 0.5);

  // EXPECT_FLOAT_EQ(vf.getSurfaceToSurfaceAverageVF(), 0.5);
}