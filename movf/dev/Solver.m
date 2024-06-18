%* [1] extract centroids from both meshes (m) (n)
%* [2] extract normal vectors (m) (n)
%* [3] extract elemental areas (m) (n)
%* [4] cast rays between all elements (m*n)
%* [5] run bfcull (m*n - b)
%* [6] determine blocking (NOT YET WRITTEN)
%* [7] run DAI

classdef Solver < handle
  properties
    options SolverOptions
  end

  methods
    function obj = Solver(options)
      arguments
        options SolverOptions
      end
      obj.options = options;
    end

    function view_factors = solveViewFactor(obj, emitter_mesh, receiver_mesh)
      arguments
        obj Solver
        emitter_mesh Mesh
        receiver_mesh Mesh
      end
    end
  end
end