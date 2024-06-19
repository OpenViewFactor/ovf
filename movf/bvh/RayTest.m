function tests = RayTest
  tests = functiontests(localfunctions);
end

function testGetOrigin(testCase)
  r = Ray;
  r.setOrigin(Vector3(0,0,0));
  verifyEqual(testCase, r.getOrigin, Vector3(0,0,0));
  r.setOrigin(Vector3(1,2,3));
  verifyEqual(testCase, r.getOrigin, Vector3(1,2,3));
end

function testGetDirection(testCase)
  r = Ray;
  r.setDirection(Vector3(1,0,0));
  verifyEqual(testCase, r.getDirection, Vector3(1,0,0));
  r.setDirection(Vector3(2,3,4));
  verifyEqual(testCase, r.getDirection, Vector3(2,3,4));
end

%* ----- TEST RAY TRIANGLE INTERACTIONS ----- *%
function testBackFaceCull(testCase)
  r = Ray;
  r.setOrigin(Vector3(0,0,0));
  r.setDirection(Vector3(1,0,0));
  forwards_emitter = Triangle(Vector3(0,0,1), Vector3(0,-1,-1), Vector3(0,1,-1));
  backwards_emitter = Triangle(Vector3(0,-1,-1), Vector3(0,0,1), Vector3(0,1,-1));
  forwards_receiver = Triangle(Vector3(2,-1,-1), Vector3(2,0,1), Vector3(2,1,-1));
  backwards_receiver = Triangle(Vector3(2,0,1), Vector3(2,-1,-1), Vector3(2,1,-1));
  verifyEqual(testCase, r.backFaceCull(forwards_emitter, forwards_receiver), false);
  verifyEqual(testCase, r.backFaceCull(backwards_emitter, forwards_receiver), true);
  verifyEqual(testCase, r.backFaceCull(forwards_emitter, backwards_receiver), true);
  verifyEqual(testCase, r.backFaceCull(backwards_emitter, backwards_receiver), true);
end

function testTriangleIntersection(testCase)
  r = Ray;
  r.setOrigin(Vector3(0,0,0));
  r.setDirection(Vector3(1,0,0));
  forwards_receiver = Triangle(Vector3(2,-1,-1), Vector3(2,0,1), Vector3(2,1,-1));
  r.triangleIntersection(forwards_receiver);
  verifyEqual(testCase, r.getIntersectionDistance, 2);
end