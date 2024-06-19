classdef BVH < handle
  properties
    all_BVHNodes BVHNode
    num_layers (1,1) uint64
    nodes_used (1,1) uint64 = 0
    mesh Mesh
  end

  methods
    function obj = BVH(~)
    end

    function linkToMesh(obj, mesh)
      arguments
        obj BVH
        mesh Mesh
      end
      obj.mesh = mesh;
    end

    function setLayerHeight(obj, num_layers)
      arguments
        obj BVH
        num_layers (1,1) uint64
      end
      obj.num_layers = num_layers;
      obj.all_BVHNodes(obj.numNodes) = BVHNode;
    end
    function num_nodes = numNodes(obj)
      num_nodes = sum(2.^linspace(1, obj.num_layers-1, obj.num_layers));
    end

    function constructBVH(obj)
      root_node_index = 1;
      obj.nodes_used = 1;
      root_node = obj.all_BVHNodes(root_node_index);
      root_node.growToIncludeMesh(obj.mesh);
      subdivideNode(root_node_index);
    end
    function subdivideNode(obj, node_index)
      arguments
        obj BVH
        node_index (1,1) uint64
      end
      current_node = obj.all_BVHNodes(node_index);

      if current_node.getNumTriangles <= 2
        return
      end

      %! [1] determine axis and position of the split plane
      node_span = current_node.getAABBMax - current_node.getAABBMin;
      axis_index = node_span.getLongestDirection;
      location_on_axis_to_split = current_node.getAABBMin.getComponentByAxis(axis_index) + node_span.getComponentByAxis(axis_index);

      %! [2] split the group of primitives in two halves using the split plane
      split_index = current_node.getFirstTriangleIndex;
      unsorted_index = split_index + current_node.getNumTriangles - 1;
      while split_index <= unsorted_index
        if obj.mesh.getElement(split_index).centroid.getComponentByAxis(axis_index) < location_on_axis_to_split
          split_index = split_indexi + 1;
        else
          obj.mesh.swapElements(split_index, unsorted_index);
          unsorted_index = unsorted_index - 1;
        end
      end

      %! [3] create child nodes for each half
      num_triangles_on_left = split_index - current_node.getFirstTriangleIndex;
      if num_triangles_on_left == 0 || num_triangles_on_left == current_node.getFirstTriangleIndex
        return
      end
      left_child_index = obj.nodes_used + 1;
      right_child_index = left_child_index + 1;
      obj.nodes_used = right_child_index;

      current_node.child_one_index = left_child_index;
      current_node.child_two_index = right_child_index;
      
      obj.all_BVHNodes(left_child_index).getFirstTriangleIndex = current_node.getFirstTriangleIndex;
      obj.all_BVHNodes(left_child_index).setNumTriangles(num_triangles_on_left);
      
      obj.all_BVHNodes(right_child_index).getFirstTriangleIndex = split_index;
      obj.all_BVHNodes(right_child_index).setNumTriangles(current_node.getNumTriangles - num_triangles_on_left);
      
      current_node.setNumTriangles(0);
      
      obj.all_BVHNodes(left_child_index).growToIncludeMesh(obj.mesh.getSubMesh(obj.all_BVHNodes(left_child_index).getFirstTriangleIndex:(obj.all_BVHNodes(left_child_index).getFirstTriangleIndex + obj.all_BVHNodes(left_child_index).getNumTriangles)));
      
      obj.all_BVHNodes(right_child_index).growToIncludeMesh(obj.mesh.getSubMesh(obj.all_BVHNodes(right_child_index).getFirstTriangleIndex:(obj.all_BVHNodes(right_child_index).getFirstTriangleIndex + obj.all_BVHNodes(right_child_index).getNumTriangles)));

      %! [4] recurse into each of the child nodes
      obj.subdivideNode(left_child_index);
      obj.subdivideNode(right_child_index);
    end
  end
end