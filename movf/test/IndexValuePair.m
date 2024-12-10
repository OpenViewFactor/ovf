classdef IndexValuePair < handle
  properties
    full_matrix_index (1,1) uint64
    view_factor_value (1,1) double
  end

  methods
    function obj = IndexValuePair(index, value)
      arguments
        index (1,1) uint64
        value (1,1) double
      end
      obj.full_matrix_index = index;
      obj.view_factor_value = value;
    end

    function index = getFullMatrixIndex(obj)
      index = obj.full_matrix_index;
    end
    function value = getViewFactorValue(obj)
      value = obj.view_factor_value;
    end
  end
end