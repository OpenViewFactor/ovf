classdef SolverOptions < handle
  properties (GetAccess = private, SetAccess = private)
    blockers (1,:) Mesh
    self_intersection_mode SelfIntersectionMode = SelfIntersectionMode.NONE
    numeric_mode NumericMode = NumericMode.DAI
    refinement_mode RefinementMode = RefinementMode.NONE
  end

  methods
    function obj = SolverOptions(blockers, self_intersection_mode)
      arguments
        blockers (:,1) Mesh
        self_intersection_mode SelfIntersectionMode
      end
      obj.blockers = blockers;
      obj.self_intersection_mode = self_intersection_mode;
    end

    function blockers = getBlockers(obj)
      blockers = obj.blockers;
    end
    function self_intersection_mode = getSelfIntersectionMode(obj)
      self_intersection_mode = obj.self_intersection_mode;
    end
    function numeric_mode = getNumericMode(obj)
      numeric_mode = obj.numeric_mode;
    end
    function refinement_mode = getRefinementMode(obj)
      refinement_mode = obj.refinement_mode;
    end

    function clearBlockers(obj)
      obj.blockers = Mesh.empty(0,0);
    end
    function addBlocker(obj, blocker)
      arguments
        obj SolverOptions
        blocker (1,:) Mesh
      end
      obj.blockers = [obj.blockers, blocker];
    end

    function setSelfIntersectionMode(obj, mode)
      arguments
        obj SolverOptions
        mode SelfIntersectionMode
      end
      obj.self_intersection_mode = mode;
    end
    function setNumericMode(obj, mode)
      arguments
        obj SolverOptions
        mode NumericMode
      end
      obj.numeric_mode = mode;
    end
    function setRefinementMode(obj, mode)
      arguments
        obj SolverOptions
        mode RefinementMode
      end
      obj.refinement_mode = mode;
    end
  end
end