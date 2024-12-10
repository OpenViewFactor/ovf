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

function testGetSubMesh(testCase)
  vectors = Vector3.empty(0,12);
  for i=1:12
    v = Vector3(i,2*i,3*i);
    vectors(i) = v;
  end
  m = Mesh;
  m.setName(MeshType.EMITTER);
  for i=1:floor(length(vectors)/3)
    m.addElement(vectors(3*i - 2), vectors(3*i - 1), vectors(3*i));
  end
  verifyEqual(testCase, m.numPoints, 12);
  verifyEqual(testCase, m.numElements, 4);
  subm = m.getSubMesh(1:3);
  verifyEqual(testCase, subm.numPoints, 12);
  verifyEqual(testCase, subm.numElements, 3);
end

%* ---- TEST SETTERS ---- *%
function testSetConnectivity(testCase)
  m = Mesh;
  verifyEqual(testCase, m.numElements, 0);
  m.setConnectivity([1,2,3]);
  verifyEqual(testCase, m.numElements, 1);
end

function testSetPoints(testCase)
  m = Mesh;
  verifyEqual(testCase, m.numPoints, 0);
  m.setPoints([Vector3(0,0,0), Vector3(1,0,0), Vector3(1,1,1)]);
  verifyEqual(testCase, m.numPoints, 3);
end

%* ----- TEST SWAP METHOD ---- *%
function testSwapElements(testCase)
  vectors = Vector3.empty(0,6);
  for i=1:6
    v = Vector3(i,2*i,3*i);
    vectors(i) = v;
  end
  m = Mesh;
  m.setName(MeshType.EMITTER);
  for i=1:floor(length(vectors)/3)
    m.addElement(vectors(3*i - 2), vectors(3*i - 1), vectors(3*i));
  end
  verifyEqual(testCase, m.getElement(1), Triangle(vectors(1),vectors(2),vectors(3)));
  verifyEqual(testCase, m.getElement(2), Triangle(vectors(4),vectors(5),vectors(6)));

  m.swapElements(1,2);
  verifyEqual(testCase, m.getElement(2), Triangle(vectors(1),vectors(2),vectors(3)));
  verifyEqual(testCase, m.getElement(1), Triangle(vectors(4),vectors(5),vectors(6)));
end