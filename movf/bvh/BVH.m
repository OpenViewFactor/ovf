classdef BVH < handle
  properties
    all_BVHNodes BVHNode
    num_layers (1,1) uint64
    nodes_used (1,1) uint64 = 0
  end

  methods
    function obj = BVH(num_layers)
      arguments
        num_layers (1,1) uint64 = 5
      end
      obj.num_layers = num_layers;
    end
  end
end