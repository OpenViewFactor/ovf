classdef ViewFactor < handle
  properties (GetAccess = private, SetAccess = private)
    state ViewFactorState = ViewFactorState.UNLINKED
    emitter Mesh
    receiver Mesh
    nonzero_vf (1,:) IndexValuePair
  end

  methods
    function obj = ViewFactor(~)
    end

    function linkMeshes(obj, emitter_mesh, receiver_mesh)
      arguments
        obj ViewFactor
        emitter_mesh Mesh
        receiver_mesh Mesh
      end
      same_mesh = emitter_mesh == receiver_mesh;
      switch same_mesh
        case true
          obj.emitter = emitter_mesh;
          obj.state = ViewFactorState.LINKED_ONE_MESH;
        case false
          obj.emitter = emitter_mesh;
          obj.receiver = receiver_mesh;
          obj.state = ViewFactorState.LINKED_TWO_MESH;
      end
    end

    function state = getState(obj)
      state = obj.state;
    end
    function tf = isLinked(obj)
      tf = obj.state ~= ViewFactorState.UNLINKED;
    end

    function num_emitters = getNumEmitterElements(obj)
      if obj.isLinked
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
      full_matrix_index = ((emitter_index - 1) * obj.getNumReceiverElements) + (receiver_index - 1) + 1;
      pair = IndexValuePair(full_matrix_index, value);
      obj.nonzero_vf = [obj.nonzero_vf, pair];
    end

    function matrix_element = getMatrixElementVF(obj, emitter_index, receiver_index)
      arguments
        obj ViewFactor
        emitter_index (1,1) uint64
        receiver_index (1,1) uint64
      end
      full_matrix_index = ((emitter_index - 1) * obj.getNumReceiverElements) + (receiver_index - 1) + 1;
      map = obj.nonzero_vf;
      for map_index = 1 : length(map)
        pair = map(map_index);
        if full_matrix_index == pair.getFullMatrixIndex
          matrix_element = pair.getViewFactorValue;
        end
      end
    end

    function emitter_element_average = getEmitterElementToReceiverSurfaceVF(obj, emitter_index)
      arguments
        obj ViewFactor
        emitter_index (1,1) uint64
      end
      if obj.state == ViewFactorState.UNLINKED
        throw(MException('ViewFactor:AverageVF', 'Error getting emitting element-to-surface average: result is not linked a mesh'));
      end
      emitter_element_average = 0;
      for receiver_index = 1 : obj.getNumReceiverElements
        if emitter_index == receiver_index && obj.state == ViewFactorState.LINKED_ONE_MESH
          continue
        end
        emitter_element_average = emitter_element_average + obj.getMatrixElementVF(emitter_index, receiver_index);
      end
    end

    function receiver_element_average = getReceiverElementFromEmitterSurfaceVF(obj, receiver_index)
      arguments
        obj ViewFactor
        receiver_index (1,1) uint64
      end
      switch obj.state
        case ViewFactorState.LINKED_ONE_MESH
          receiver_element_average = obj.getEmitterElementToReceiverSurfaceVF(receiver_index);
        case ViewFactorState.LINKED_TWO_MESH
          receiver_element_average = 0;
          for emitter_index = 1 : obj.emitter.numElements
            area_reciprocal = obj.emitter.getElement(emitter_index).area / obj.receiver.getElement(receiver_index).area;
            receiver_element_average = receiver_element_average + obj.getMatrixElementVF(emitter_index, receiver_index) * area_reciprocal;
          end
        otherwise
          throw(MException('ViewFactor:AverageVF', 'Error getting receiving element-to-surface average: result is not linked a mesh'));
      end
    end

    function surface_to_surface_average = getSurfaceToSurfaceAverageVF(obj)
      if obj.state == ViewFactorState.UNLINKED
        throw(MException('ViewFactor:AverageVF', 'Error getting surface-to-surface average: result is not linked a mesh'));
      else
        surface_to_surface_average = 0;
        for emitter_index = 1 : obj.emitter.numElements
          surface_to_surface_average = surface_to_surface_average + obj.emitter.getElement(emitter_index).area * obj.getEmitterElementToReceiverSurfaceVF(emitter_index);
        end
        surface_to_surface_average = surface_to_surface_average / obj.emitter.area;
      end
    end
  end
end