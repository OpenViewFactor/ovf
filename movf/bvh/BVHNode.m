classdef BVHNode < handle
  properties (GetAccess = private, SetAccess = private)
    bounding_box_min Vector3 = Vector3(inf, inf, inf)
    bounding_box_max Vector3 = Vector3(-inf, -inf, -inf)
    child_one_index uint64 = 0
    child_two uint64 = 0
    first_triangle_index uint64 = 0
    number_of_triangles uint64 = 0
  end

  methods
    function obj = BVHNode(~)
    end

    function growToIncludeTriangle(obj, triangle)
      arguments
        obj BVHNode
        triangle Triangle
      end
      obj.bounding_box_min = vectorMinima(obj.bounding_box_min, triangle.getOA);
      obj.bounding_box_min = vectorMinima(obj.bounding_box_min, triangle.getOB);
      obj.bounding_box_min = vectorMinima(obj.bounding_box_min, triangle.getOC);
      obj.bounding_box_max = vectorMaxima(obj.bounding_box_max, triangle.getOA);
      obj.bounding_box_max = vectorMaxima(obj.bounding_box_max, triangle.getOB);
      obj.bounding_box_max = vectorMaxima(obj.bounding_box_max, triangle.getOC);
    end

    function growToIncludeMesh(obj, mesh)
      arguments
        obj BVHNode
        mesh Mesh
      end
      for i=1:mesh.numElements
        obj.growToIncludeTriangle(mesh.getElement(i));
      end
    end
  end
end