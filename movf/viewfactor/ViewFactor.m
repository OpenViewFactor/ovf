classdef ViewFactor < handle
  properties (GetAccess = private, SetAccess = private)
    state ViewFactorState = ViewFactorState.UNLINKED
    emitter Mesh
    receiver Mesh
    nonzero_vf ViewFactorMap
  end

  methods
    function obj = ViewFactor(~)
    end

    function link(obj, emitter_mesh, optional_receiver_mesh)
      arguments
        obj ViewFactor
        emitter_mesh Mesh
        optional_receiver_mesh Mesh
      end
      switch nargin
        case 2
          obj.emitter = emitter_mesh;
          obj.state = ViewFactorState.LINKED_ONE_MESH;
        case 3
          obj.emitter = emitter_mesh;
          obj.receiver = optional_receiver_mesh;
          obj.state = ViewFactorState.LINKED_TWO_MESH;
      end
    end

    function tf = isLinked(obj)
      tf = obj.state ~= ViewFactorState.UNLINKED;
    end

    function num_emitters = getNumEmitterElements(obj)
      if obj.state ~= ViewFactorState.UNLINKED
        num_emitters = obj.emitter.numElements;
      else
        exception = MException("ViewFactor:Unlinked", "Cannot get number of emitter elements, ViewFactor object is not linked to a mesh");
        throw(exception)
      end
    end
    function num_receivers = getNumReceiverElements(obj)
      switch obj.state
        case ViewFactorState.LINKED_ONE_MESH
          num_receivers = obj.getNumEmitterElements;
        case ViewFactorState.LINKED_TWO_MESH
          num_receivers = obj.receiver.numElements;
        case ViewFactorState.UNLINKED
          exception = MException("ViewFactor:Unlinked", "Cannot get number of receiver elements, ViewFactor object is not linked to a mesh");
          throw(exception)
      end
    end

    function num_meshes = getNumMeshes(obj)
      switch obj.state
        case ViewFactorState.LINKED_ONE_MESH
          num_meshes = 1;
        case ViewFactorState.LINKED_TWO_MESH
          num_meshes = 2;
        case ViewFactorState.UNLINKED
          exception = MException("ViewFactor:Unlinked", "Cannot get linked meshes, ViewFactor object is not linked to a mesh");
          throw(exception)
      end
    end

    function emitter_mesh = getEmitterMesh(obj)
      if obj.state ~= ViewFactorState.UNLINKED
        emitter_mesh = obj.emitter;
      else
        exception = MException("ViewFactor:Unlinked", "Cannot get the emitter mesh, ViewFactor object is not linked to a mesh");
        throw(exception)
      end
    end
    function receiver_mesh = getReceiverMesh(obj)
      switch obj.state
        case ViewFactorState.LINKED_ONE_MESH
          receiver_mesh = obj.emitter;
        case ViewFactorState.LINKED_TWO_MESH
          receiver_mesh = obj.receiver;
        case ViewFactorState.UNLINKED
          exception = MException("ViewFactor:Unlinked", "Cannot get the receiver mesh, ViewFactor object is not linked to a mesh");
          throw(exception)
      end
    end

    function setElement(obj, emitter_index, receiver_index, value)
      arguments
        obj ViewFactor
        emitter_index (1,1) uint64
        receiver_index (1,1) uint64
        value (1,1) double
      end
      full_matrix_index = (emitter_index * obj.getNumReceiverElements) + receiver_index;
      obj.nonzero_vf = [obj.nonzero_vf; ViewFactorMap(full_matrix_index, value)];
    end
  end
end