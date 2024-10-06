#include "Triangulation.hpp"
#include "STLReader.hpp"
#include "TestGlobals.h"
#include "gtest.h"

using namespace openviewfactor;

TEST(Triangulation_Tests, test_getters) {
  Vector3<float> p1(0,0,0);
  Vector3<float> p2(1,0,0);
  Vector3<float> p3(1,1,0);
  Vector3<float> p4(0,1,0);

  Triangulation<float> tgln;
  tgln.setPoints({p1,p2,p3,p4}).setConnectivity({{0,1,2},{0,2,3}});

  Triangulation<float> sub_mesh;
  sub_mesh.setPoints({p1,p2,p3,p4}).setConnectivity({{0,1,2}});

  EXPECT_EQ(tgln.getNumElements(), 2);
  EXPECT_EQ(tgln.getNumPoints(), 4);
  EXPECT_EQ(tgln.getNumBytes(), 48 + 48);

  EXPECT_EQ(tgln.getMeshArea(), 1);
  EXPECT_EQ(tgln.getSubMesh({0}), sub_mesh);
}

TEST(Triangulation_Tests, test_operators) {
  Vector3<float> p1(0,0,0);
  Vector3<float> p2(1,0,0);
  Vector3<float> p3(1,1,0);
  Vector3<float> p4(0,1,0);
  
  Triangulation<float> tgln;
  tgln.setPoints({p1,p2,p3,p4}).setConnectivity({{0,1,2},{0,2,3}});

  EXPECT_EQ(tgln[0], Triangle<float>(p1,p2,p3));
  EXPECT_EQ(tgln[1], Triangle<float>(p1,p3,p4));
}

TEST(Triangulation_Tests, test_stlread_binary) {
  STLReader<float> stl_reader = STLReader<float>();
  Triangulation<float> mesh = stl_reader.getMesh(OVF_INPUT("xy_plane_unit_square_binary.stl"));

  Triangle<float> t1( { {0.0 , 0.0 , 0.0} , {1.0 , 0.0 , 0.0} , {1.0 , 1.0 , 0.0} } );
  Triangle<float> t2( { {0.0 , 0.0 , 0.0} , {1.0 , 1.0 , 0.0} , {0.0 , 1.0 , 0.0} } );

  EXPECT_EQ(mesh[0], t1);
  EXPECT_EQ(mesh[1], t2);
}

TEST(Triangulation_Tests, test_stlread_ascii) {
  STLReader<float> stl_reader = STLReader<float>();
  Triangulation<float> mesh = stl_reader.getMesh(OVF_INPUT("xy_plane_unit_square_text.stl"));

  Triangle<float> t1( { {0.0 , 0.0 , 0.0} , {1.0 , 0.0 , 0.0} , {1.0 , 1.0 , 0.0} } );
  Triangle<float> t2( { {0.0 , 0.0 , 0.0} , {1.0 , 1.0 , 0.0} , {0.0 , 1.0 , 0.0} } );

  EXPECT_EQ(mesh[0], t1);
  EXPECT_EQ(mesh[1], t2);
}