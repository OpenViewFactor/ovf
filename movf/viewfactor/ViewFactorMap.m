classdef ViewFactorMap < handle
  properties (GetAccess = private, SetAccess = private)
    mappings (:,1) IndexValuePair
  end

  methods
    function obj = ViewFactorMap(~)
    end

    function addValue(obj, index_value_pair)
      arguments
        obj ViewFactorMap
        index_value_pair (1,1) IndexValuePair
      end
      obj.mappings = [obj.mappings; index_value_pair];
    end
  end
end