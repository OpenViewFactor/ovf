classdef BVH < handle
  properties
    all_BVHNodes BVHNode
    num_layers (1,1) uint64
    minimum_triangle_threshold (1,1) uint64
    nodes_used (1,1) uint64 = 0
    all_layer_height_indices (1,:) uint64
    mesh Mesh
  end

  methods (Access = public)
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
      obj.setLayerHeightIndices;
    end

    function setMinimumNumTriangles(obj, min_triangles)
      arguments
        obj BVH
        min_triangles (1,1) uint64
      end
      obj.minimum_triangle_threshold = min_triangles;
    end

    function num_nodes = numNodes(obj)
      num_nodes = sum(2.^linspace(0, double(obj.num_layers)-1, double(obj.num_layers)));
    end

    function constructBVH(obj)
      root_node_index = 1;
      obj.nodes_used = 1;
      root_node = obj.all_BVHNodes(root_node_index);
      root_node.growToIncludeMesh(obj.mesh);
      obj.subdivideNode(root_node_index);
    end
    
    function plotMesh(obj)
      obj.mesh.plotMesh
    end

    function plotLeafNodes(obj)
      figure
      hold on
      for i = 1 : obj.numNodes
        if obj.all_BVHNodes(i).getNumTriangles > 0
          obj.plotNodeSubMesh(i);
          obj.plotNodeBBox(i);
        end
      end
    end
  end

  methods (Access = private)
    function setLayerHeightIndices(obj)
      obj.all_layer_height_indices = ones(1, obj.numNodes, "uint64");
      obj.all_layer_height_indices([2,3]) = 2;
      has_node_been_split = false(1, obj.numNodes);
      has_node_been_split(1) = true;
      for i = 4 : 2 : obj.numNodes
        if obj.all_layer_height_indices(i - 2) < obj.num_layers
          current_layer_level = obj.all_layer_height_indices(i - 2) + 1;
          has_node_been_split(i - 2) = true;
        else
          j = -1;
          while current_layer_level == obj.num_layers || has_node_been_split(i - j) == true
            j = j + 2;
            current_layer_level = obj.all_layer_height_indices(i - j);
          end
          current_layer_level = obj.all_layer_height_indices(i - j) + 1;
          has_node_been_split(i - j) = true;
        end
        obj.all_layer_height_indices(i:i+1) = current_layer_level;
      end
    end

    function subdivideNode(obj, node_index)
      arguments
        obj BVH
        node_index (1,1) uint64
      end
      %! skip subdivision if the current iteration is already at the desired layer height
      if obj.all_layer_height_indices(node_index) == obj.num_layers
        return
      end

      current_node = obj.all_BVHNodes(node_index);
      %! skip subdivision if the current iteration contains too few triangles
      if current_node.getNumTriangles <= obj.minimum_triangle_threshold
        return
      end

      %! [1] determine the cardinal axis and position along it to split the bounding box
      split_data = obj.getSplitLocation(current_node);
      axis_index = split_data(1);
      location_on_axis_to_split = split_data(2);

      %! [2] split the group of primitives in two halves using the split plane
      split_index = obj.splitPrimitives(current_node, axis_index, location_on_axis_to_split);

      %! skip subdivision if the "split" contains either no triangles or all the triangles in the current node
      num_triangles_on_left = split_index - current_node.getFirstTriangleIndex;
      if num_triangles_on_left == 0 || num_triangles_on_left == current_node.getNumTriangles
        return
      end

      %! [3] create child nodes for each half
      left_child_index = obj.createChildNodes(current_node, split_index, num_triangles_on_left);

      %! [4] recurse into each of the child nodes
      obj.subdivideNode(left_child_index);
      obj.subdivideNode(left_child_index + 1);
    end

    function left_child_index = createChildNodes(obj, bvh_node, split_index, num_triangles_on_left)
      arguments
        obj BVH
        bvh_node BVHNode
        split_index (1,1) uint64
        num_triangles_on_left (1,1) uint64
      end
      left_child_index = obj.nodes_used + 1;
      obj.nodes_used = left_child_index + 1;

      bvh_node.setChildOneIndex(left_child_index);
      
      obj.all_BVHNodes(left_child_index).setFirstTriangleIndex(bvh_node.getFirstTriangleIndex);
      obj.all_BVHNodes(left_child_index).setNumTriangles(num_triangles_on_left);
      
      obj.all_BVHNodes(left_child_index + 1).setFirstTriangleIndex(split_index);
      obj.all_BVHNodes(left_child_index + 1).setNumTriangles(bvh_node.getNumTriangles - num_triangles_on_left);
      
      bvh_node.setNumTriangles(0);

      obj.constructNewNode(left_child_index);
      obj.constructNewNode(left_child_index + 1);
    end

    function constructNewNode(obj, node_index)
      arguments
        obj BVH
        node_index (1,1) uint64
      end
      obj.all_BVHNodes(node_index).growToIncludeMesh( ...
          obj.mesh.getSubMesh( ...
              obj.all_BVHNodes(node_index).getFirstTriangleIndex : ...
              (obj.all_BVHNodes(node_index).getFirstTriangleIndex + obj.all_BVHNodes(node_index).getNumTriangles - 1) ...
              ) ...
          );
    end

    function split_index = splitPrimitives(obj, bvh_node, axis_index, split_location)
      arguments
        obj BVH
        bvh_node BVHNode
        axis_index (1,1) uint8
        split_location (1,1) double
      end
      split_index = bvh_node.getFirstTriangleIndex;
      unsorted_index = split_index + bvh_node.getNumTriangles - 1;
      while split_index <= unsorted_index
        if obj.mesh.getElement(split_index).getCentroid.getComponentByAxis(axis_index) < split_location
          split_index = split_index + 1;
        else
          obj.mesh.swapElements(split_index, unsorted_index);
          unsorted_index = unsorted_index - 1;
        end
      end
    end

    function plotNodeSubMesh(obj, node_index)
      arguments
        obj BVH
        node_index (1,1) uint64
      end
      m = obj.extractNodeSubMesh(node_index);
      m.plotMesh;
    end

    function sub_mesh = extractNodeSubMesh(obj, node_index)
      arguments
        obj BVH
        node_index (1,1) uint64
      end
      sub_mesh = obj.mesh.getSubMesh( ...
                     obj.all_BVHNodes(node_index).getFirstTriangleIndex : ...
                     (obj.all_BVHNodes(node_index).getFirstTriangleIndex + obj.all_BVHNodes(node_index).getNumTriangles - 1) ...
                     );
    end

    function plotNodeBBox(obj, node_index)
      arguments
        obj BVH
        node_index (1,1) uint64
      end
      b = obj.all_BVHNodes(node_index);
      AABBmin = b.getAABBMin;
      AABBmax = b.getAABBMax;
      all_min = [AABBmin.getX, AABBmin.getY, AABBmin.getZ];
      yz_x = [AABBmax.getX, AABBmin.getY, AABBmin.getZ];
      z_xy = [AABBmax.getX, AABBmax.getY, AABBmin.getZ];
      xz_y = [AABBmin.getX, AABBmax.getY, AABBmin.getZ];
      xy_z = [AABBmin.getX, AABBmin.getY, AABBmax.getZ];
      y_xz = [AABBmax.getX, AABBmin.getY, AABBmax.getZ];
      all_max = [AABBmax.getX, AABBmax.getY, AABBmax.getZ];
      x_yz = [AABBmin.getX, AABBmax.getY, AABBmax.getZ];
      vertices = [all_min; yz_x; z_xy; xz_y; xy_z; y_xz; all_max; x_yz];
      faces = [1,2,3,4; ...
               1,2,6,5; ...
               1,4,8,5; ...
               2,3,7,6; ...
               3,4,8,7; ...
               5,6,7,8];
      patch('Faces', faces, 'Vertices', vertices, 'FaceColor', 'red', 'FaceAlpha', 0.25);
    end
  end

  methods (Static)
    function split_data = getSplitLocation(bvh_node)
      arguments
        bvh_node BVHNode
      end
      node_span = bvh_node.getAABBMax - bvh_node.getAABBMin;
      axis_index = node_span.getLongestDirection;
      split_location = bvh_node.getAABBMin.getComponentByAxis(axis_index) + node_span.getComponentByAxis(axis_index) / 2;
      split_data = [axis_index, split_location];
    end
  end
end