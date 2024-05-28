function tests = MeshTest
  tests = functiontests(localfunctions);
end

%* ----- TEST GETTERS ----- *%
function testGetName(testCase)
  v1 = Vector3(0,0,0);
  v2 = Vector3(1,0,0);
  v3 = Vector3(0,1,0);
  m = Mesh;
  m.setName(MeshType.EMITTER);
  m.addElement(v1, v2, v3);
  verifyEqual(testCase, m.getName, MeshType.EMITTER);
end

function testNumElements(testCase)
  v1 = Vector3(0,0,0);
  v2 = Vector3(1,0,0);
  v3 = Vector3(0,1,0);
  m = Mesh;
  m.setName(MeshType.EMITTER);
  m.addElement(v1, v2, v3);
  verifyEqual(testCase, m.numElements, 1);
end

function testNumPoints(testCase)
  v1 = Vector3(0,0,0);
  v2 = Vector3(1,0,0);
  v3 = Vector3(0,1,0);
  m = Mesh;
  m.setName(MeshType.EMITTER);
  m.addElement(v1, v2, v3);
  verifyEqual(testCase, m.numPoints, 3);
end