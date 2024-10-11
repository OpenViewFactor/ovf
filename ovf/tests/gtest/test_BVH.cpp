#include "Vector3.hpp"
#include "Triangle.hpp"
#include "Triangulation.hpp"
#include "BVHNode.hpp"
#include "BVH.hpp"
#include "STLReader.hpp"
#include "Ray.hpp"
#include "TestGlobals.h"
#include "gtest.h"

using namespace openviewfactor;

TEST(BVH_Test, test_linking) {
  BVH<float> bvh;

  EXPECT_EQ(bvh.isLinked(), false);

  STLReader<float> stl_reader;
  Triangulation<float> mesh = stl_reader.getMesh(OVF_INPUT("3_tall_box.stl"));

  bvh.linkToTriangulation(mesh);
  EXPECT_EQ(bvh.isLinked(), true);
}

TEST(BVH_Test, test_unlinked_construction) {
  BVH<float> bvh;
  EXPECT_THROW(bvh.constructBVH(), std::runtime_error);
}

TEST(BVH_Test, test_linked_construction) {
  STLReader<float> stl_reader;
  Triangulation<float> simple_mesh = stl_reader.getMesh(OVF_INPUT("xy_plane_unit_square_binary.stl"));

  BVH<float> simple_bvh;
  simple_bvh.linkToTriangulation(simple_mesh);
  simple_bvh.constructBVH();
  EXPECT_EQ(simple_bvh.getNumNodesUsed(), 1);

  Triangulation<float> skull = stl_reader.getMesh(OVF_INPUT("skull.stl"));

  for (unsigned int i = 0; i < skull.getNumElements(); i+=100) {
    std::cout << "triangle centroid: {" << skull[i].getCentroid()[0] << "," << skull[i].getCentroid()[1] << "," <<  skull[i].getCentroid()[2] << "}" << std::endl;
  }
  
  BVH<float> skull_bvh;
  skull_bvh.linkToTriangulation(skull);
  skull_bvh.constructBVH();

  EXPECT_EQ(skull_bvh.getNumNodesUsed(), 6053);
}