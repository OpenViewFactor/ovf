classdef BVHNode < handle
  properties
    bounding_box_min Vector3 = Vector3(inf, inf, inf)
    bounding_box_max Vector3 = Vector3(-inf, -inf, -inf)
    child_one_index uint64 = 0
    first_triangle_index uint64 = 1
    number_of_triangles uint64 = 0
  end

  methods
    function obj = BVHNode(~)
    end

    function is_leaf = isLeaf(obj)
      is_leaf = ~(obj.number_of_triangles == 0);
    end
    function bounding_box_min = getAABBMin(obj)
      bounding_box_min = obj.bounding_box_min;
    end
    function bounding_box_max = getAABBMax(obj)
      bounding_box_max = obj.bounding_box_max;
    end
    function child_one_index = getChildOneIndex(obj)
      child_one_index = obj.child_one_index;
    end
    function child_two_index = getChildTwoIndex(obj)
      child_two_index = obj.child_one_index + 1;
    end
    function first_triangle_index = getFirstTriangleIndex(obj)
      first_triangle_index = obj.first_triangle_index;
    end
    function number_of_triangles = getNumTriangles(obj)
      number_of_triangles = obj.number_of_triangles;
    end

    function setChildOneIndex(obj, index)
      arguments
        obj BVHNode
        index (1,1) uint64
      end
      obj.child_one_index = index;
    end
    function setFirstTriangleIndex(obj, index)
      arguments
        obj BVHNode
        index (1,1) uint64
      end
      obj.first_triangle_index = index;
    end
    function setNumTriangles(obj, n)
      arguments
        obj BVHNode
        n (1,1) uint64
      end
      obj.number_of_triangles = n;
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
      obj.setNumTriangles(mesh.numElements);
    end
  end
end