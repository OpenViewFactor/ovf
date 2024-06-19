function tests = Vector3Test
  tests = functiontests(localfunctions);
end


%* ----- TEST GETTERS ----- *%
function testGetX(testCase)
  v = Vector3(1,2,3);
  verifyEqual(testCase, v.getX, 1);
end

function testGetY(testCase)
  v = Vector3(1,2,3);
  verifyEqual(testCase, v.getY, 2);
end

function testGetZ(testCase)
  v = Vector3(1,2,3);
  verifyEqual(testCase, v.getZ, 3);
end

function testGetMagnitude(testCase)
  v = Vector3(3,4,0);
  verifyEqual(testCase, v.getMagnitude, 5);
end

%* ----- TEST SETTERS ----- *%
function testSetX(testCase)
  v = Vector3(1,2,3);
  v.setX(4);
  verifyEqual(testCase, v.getX, 4);
end

function testSetY(testCase)
  v = Vector3(1,2,3);
  v.setY(5);
  verifyEqual(testCase, v.getY, 5);
end

function testSetZ(testCase)
  v = Vector3(1,2,3);
  v.setZ(6);
  verifyEqual(testCase, v.getZ, 6);
end

%* ----- TEST VECTOR OPERATIONS ----- *%
function testDotProduct(testCase)
  a = Vector3(1,0,0);
  b = Vector3(0,1,0);
  c = Vector3(0,0,1);
  verifyEqual(testCase, a.dotProduct(b), 0);
  verifyEqual(testCase, a.dotProduct(c), 0);
  verifyEqual(testCase, b.dotProduct(c), 0);
  verifyEqual(testCase, a.dotProduct(a), 1);
end

function testCrossProduct(testCase)
  a = Vector3(1,0,0);
  b = Vector3(0,1,0);
  c = Vector3(0,0,1);
  verifyEqual(testCase, a.crossProduct(b), c);
  verifyEqual(testCase, b.crossProduct(c), a);
  verifyEqual(testCase, c.crossProduct(a), b);
end

function testDirection(testCase)
  v = Vector3(2,0,0);
  verifyEqual(testCase, v.direction, Vector3(1,0,0));
end

function testPlus(testCase)
  a = Vector3(1,0,0);
  b = Vector3(0,1,0);
  c = Vector3(1,1,0);
  verifyEqual(testCase, a + b, c);
end

function testMinus(testCase)
  a = Vector3(1,0,0);
  b = Vector3(0,1,0);
  c = Vector3(1,-1,0);
  verifyEqual(testCase, a - b, c);
end

%* ----- TEST LONGEST DIRECTION METHOD ---- 8%
function testLongestDirection(testCase)
  a = Vector3(1,2,3);
  verifyEqual(testCase, a.getLongestDirection, 3);
  b = Vector3(2,3,1);
  verifyEqual(testCase, b.getLongestDirection, 2);
  c = Vector3(3,2,1);
  verifyEqual(testCase, c.getLongestDirection, 1);
end

%* ----- TEST AXIS EXTRACTION METHOD ---- *%
function testGetComponentByAxis(testCase)
  a = Vector3(1,2,3);
  verifyEqual(testCase, a.getComponentByAxis(1), 1);
  verifyEqual(testCase, a.getComponentByAxis(2), 2);
  verifyEqual(testCase, a.getComponentByAxis(3), 3);
end
%! the equality function is already being tested in every other test