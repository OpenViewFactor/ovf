function tests = TriangleTest
  tests = functiontests(localfunctions);
end

function testGetOA(testCase)
  OA = Vector3(0, 0, 0);
  OB = Vector3(1, 0, 0);
  OC = Vector3(0, 1, 0);
  t = Triangle(OA, OB, OC);
  verifyEqual(testCase, t.getOA, OA);
end

function testGetOB(testCase)
  OA = Vector3(0, 0, 0);
  OB = Vector3(1, 0, 0);
  OC = Vector3(0, 1, 0);
  t = Triangle(OA, OB, OC);
  verifyEqual(testCase, t.getOB, OB);
end

function testGetOC(testCase)
  OA = Vector3(0, 0, 0);
  OB = Vector3(1, 0, 0);
  OC = Vector3(0, 1, 0);
  t = Triangle(OA, OB, OC);
  verifyEqual(testCase, t.getOC, OC);
end

function testGetAB(testCase)
  OA = Vector3(0, 0, 0);
  OB = Vector3(1, 0, 0);
  OC = Vector3(0, 1, 0);
  t = Triangle(OA, OB, OC);
  verifyEqual(testCase, t.getAB, OB - OA);
end

function testGetBC(testCase)
  OA = Vector3(0, 0, 0);
  OB = Vector3(1, 0, 0);
  OC = Vector3(0, 1, 0);
  t = Triangle(OA, OB, OC);
  verifyEqual(testCase, t.getBC, OC - OB);
end

function testGetCA(testCase)
  OA = Vector3(0, 0, 0);
  OB = Vector3(1, 0, 0);
  OC = Vector3(0, 1, 0);
  t = Triangle(OA, OB, OC);
  verifyEqual(testCase, t.getCA, OA - OC);
end

function testNormal(testCase)
  OA = Vector3(0, 0, 0);
  OB = Vector3(1, 0, 0);
  OC = Vector3(0, 1, 0);
  k = Vector3(0,0,1);
  t = Triangle(OA, OB, OC);
  verifyEqual(testCase, t.normal, k);
end

function testCentroid(testCase)
  OA = Vector3(0, 0, 0);
  OB = Vector3(1, 0, 0);
  OC = Vector3(0, 1, 0);
  t = Triangle(OA, OB, OC);
  c = Vector3(1/3, 1/3, 0);
  verifyEqual(testCase, t.centroid, c);
end

function testArea(testCase)
  OA = Vector3(0, 0, 0);
  OB = Vector3(1, 0, 0);
  OC = Vector3(0, 1, 0);
  t = Triangle(OA, OB, OC);
  verifyEqual(testCase, t.area, 0.5);
end

%* ----- TEST SETTERS ----- *%
function testSetOA(testCase)
  OA = Vector3(0, 0, 0);
  OB = Vector3(1, 0, 0);
  OC = Vector3(0, 1, 0);
  t = Triangle(OA, OB, OC);
  t.setOA(1, 1, 0);
  verifyEqual(testCase, t.getOA, Vector3(1, 1, 0));
end
function testSetOB(testCase)
  OA = Vector3(0, 0, 0);
  OB = Vector3(1, 0, 0);
  OC = Vector3(0, 1, 0);
  t = Triangle(OA, OB, OC);
  t.setOB(1, 1, 0);
  verifyEqual(testCase, t.getOB, Vector3(1, 1, 0));
end
function testSetOC(testCase)
  OA = Vector3(0, 0, 0);
  OB = Vector3(1, 0, 0);
  OC = Vector3(0, 1, 0);
  t = Triangle(OA, OB, OC);
  t.setOC(1, 1, 0);
  verifyEqual(testCase, t.getOC, Vector3(1, 1, 0));
end

%! the equality function is already being tested in every other test