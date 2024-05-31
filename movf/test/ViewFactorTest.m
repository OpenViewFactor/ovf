function tests = ViewFactorTest
  tests = functiontests(localfunctions);
end

function testLinkMeshes(testCase)
  e = Mesh;
  e.setName(MeshType.EMITTER);
  e.addElement(Vector3(0,0,0), Vector3(1,0,0), Vector3(0,1,0));
  v = ViewFactor;
  v.linkMeshes(e, e);
  verifyEqual(testCase, v.isLinked, true);
  verifyEqual(testCase, v.getState, ViewFactorState.LINKED_ONE_MESH);
end

function testNumEmitterElements(testCase)
  e = Mesh;
  e.setName(MeshType.EMITTER);
  e.addElement(Vector3(0,0,0), Vector3(1,0,0), Vector3(1,1,0));
  e.addElement(Vector3(0,0,0), Vector3(1,1,0), Vector3(0,1,0));
  v = ViewFactor;
  v.linkMeshes(e, e);
  verifyEqual(testCase, v.getNumEmitterElements, 2);
end
function testNumReceiverElements(testCase)
  e = Mesh;
  e.setName(MeshType.EMITTER);
  e.addElement(Vector3(0,0,0), Vector3(1,0,0), Vector3(1,1,0));
  e.addElement(Vector3(0,0,0), Vector3(1,1,0), Vector3(0,1,0));
  v = ViewFactor;
  v.linkMeshes(e, e);
  verifyEqual(testCase, v.getNumReceiverElements, 2);

  r = Mesh;
  r.setName(MeshType.RECEIVER);
  r.addElement(Vector3(0,0,1), Vector3(1,0,1), Vector3(1,1,1));
  r.addElement(Vector3(0,0,1), Vector3(1,1,1), Vector3(0,1,1));
  r.addElement(Vector3(1,0,1), Vector3(2,0,1), Vector3(2,1,1));
  v = ViewFactor;
  v.linkMeshes(e, r);
  verifyEqual(testCase, v.getNumReceiverElements, 3);
end

function testNumMeshes(testCase)
  e = Mesh;
  e.setName(MeshType.EMITTER);
  e.addElement(Vector3(0,0,0), Vector3(1,0,0), Vector3(1,1,0));
  e.addElement(Vector3(0,0,0), Vector3(1,1,0), Vector3(0,1,0));
  v = ViewFactor;
  v.linkMeshes(e, e);
  verifyEqual(testCase, v.getNumMeshes, 1);

  r = Mesh;
  r.setName(MeshType.RECEIVER);
  r.addElement(Vector3(0,0,1), Vector3(1,0,1), Vector3(1,1,1));
  r.addElement(Vector3(0,0,1), Vector3(1,1,1), Vector3(0,1,1));
  r.addElement(Vector3(1,0,1), Vector3(2,0,1), Vector3(2,1,1));
  v = ViewFactor;
  v.linkMeshes(e, r);
  verifyEqual(testCase, v.getNumMeshes, 2);
end

function testGetEmitterMesh(testCase)
  e = Mesh;
  e.setName(MeshType.EMITTER);
  e.addElement(Vector3(0,0,0), Vector3(1,0,0), Vector3(1,1,0));
  e.addElement(Vector3(0,0,0), Vector3(1,1,0), Vector3(0,1,0));
  v = ViewFactor;
  v.linkMeshes(e, e);
  verifyEqual(testCase, v.getEmitterMesh, e);
end

function testGetReceiverMesh(testCase)
  e = Mesh;
  e.setName(MeshType.EMITTER);
  e.addElement(Vector3(0,0,0), Vector3(1,0,0), Vector3(1,1,0));
  e.addElement(Vector3(0,0,0), Vector3(1,1,0), Vector3(0,1,0));
  v = ViewFactor;
  v.linkMeshes(e, e);
  verifyEqual(testCase, v.getReceiverMesh, e);

  r = Mesh;
  r.setName(MeshType.RECEIVER);
  r.addElement(Vector3(0,0,1), Vector3(1,0,1), Vector3(1,1,1));
  r.addElement(Vector3(0,0,1), Vector3(1,1,1), Vector3(0,1,1));
  r.addElement(Vector3(1,0,1), Vector3(2,0,1), Vector3(2,1,1));
  v = ViewFactor;
  v.linkMeshes(e, r);
  verifyEqual(testCase, v.getReceiverMesh, r);
end

function testGetMatrixElementVF(testCase)
  e = Mesh;
  e.setName(MeshType.EMITTER);
  e.addElement(Vector3(0,0,0), Vector3(1,0,0), Vector3(1,1,0));
  e.addElement(Vector3(0,0,0), Vector3(1,1,0), Vector3(0,1,0));
  v = ViewFactor;
  v.linkMeshes(e, e);
  v.setElement(1, 1, 0);
  verifyEqual(testCase, v.getMatrixElementVF(1, 1), 0);
end

function testGetEmitterElementToReceiverSurfaceVF(testCase)
  e = Mesh;
  e.setName(MeshType.EMITTER);
  e.addElement(Vector3(0,0,0), Vector3(1,0,0), Vector3(1,1,0));
  e.addElement(Vector3(0,0,0), Vector3(1,1,0), Vector3(0,1,0));

  r = Mesh;
  r.setName(MeshType.RECEIVER);
  r.addElement(Vector3(0,0,1), Vector3(1,0,1), Vector3(1,1,1));
  r.addElement(Vector3(0,0,1), Vector3(1,1,1), Vector3(0,1,1));
  r.addElement(Vector3(1,0,1), Vector3(2,0,1), Vector3(2,1,1));
  
  v = ViewFactor;
  v.linkMeshes(e, r);
  v.setElement(1,1,1);
  v.setElement(1,2,1);
  v.setElement(1,3,1);
  v.setElement(2,1,1);
  v.setElement(2,2,1);
  v.setElement(2,3,1);
  verifyEqual(testCase, v.getEmitterElementToReceiverSurfaceVF(1), 3);
end