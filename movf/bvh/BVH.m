classdef BVH < handle
  properties
    all_BVHNodes BVHNode
    nodes_used (1,1) uint64 = 0
    mesh Mesh
    mesh_element_indices (1,:) uint64
    minimum_triangle_threshold (1,1) uint64 = 2
    num_cost_evaluation_points (1,1) uint64 = 4
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
      obj.mesh_element_indices = linspace(1, mesh.numElements, mesh.numElements);
      obj.all_BVHNodes(obj.maxNumNodes) = BVHNode;
    end

    function setMinimumNumTriangles(obj, min_triangles)
      arguments
        obj BVH
        min_triangles (1,1) uint64
      end
      obj.minimum_triangle_threshold = min_triangles;
    end

    function setNumCostEvaluationPoints(obj, num_cost_evaluation_points)
      arguments
        obj BVH
        num_cost_evaluation_points (1,1) uint64
      end
      obj.num_cost_evaluation_points = num_cost_evaluation_points;
    end

    function constructBVH(obj)
      fprintf('Constructing BVH for mesh with %d elements\n', obj.mesh.numElements)
      root_node_index = 1;
      obj.nodes_used = 1;
      root_node = obj.all_BVHNodes(root_node_index);
      root_node.growToIncludeMesh(obj.mesh);
      root_node.setNumTriangles(obj.mesh.numElements);
      obj.subdivideNode(root_node_index);
    end

    function intersectRayWithBVH(obj, ray)
      arguments
        obj BVH
        ray Ray
      end
      obj.intersectRayWithBVHNode(ray, 1);
    end
    
    function plotMesh(obj)
      obj.mesh.plotMesh
      obj.plotNodeBBox(1);
    end

    function plotLeafNodes(obj)
      for i = 1 : obj.maxNumNodes
        if obj.all_BVHNodes(i).getNumTriangles > 0
          obj.plotNodeBBox(i);
          obj.plotNodeSubMesh(i);
        end
      end
    end
  end

  methods (Access = private)
    function max_num_nodes = maxNumNodes(obj)
      max_num_nodes = obj.mesh.numElements * 2 - 1;
    end

    function subdivideNode(obj, node_index)
      arguments
        obj BVH
        node_index (1,1) uint64
      end
      fprintf('Subdividing Node %d\n', node_index)

      current_node = obj.all_BVHNodes(node_index);
      %! skip subdivision if the current iteration contains too few triangles
      if current_node.getNumTriangles <= obj.minimum_triangle_threshold
        fprintf('Not splitting node %d, number of triangles lower than threshold\n', node_index)
        return
      end

      %! [1] determine the cardinal axis and position along it to split the bounding box
      [axis_index, location_on_axis_to_split, best_cost] = obj.getSplitLocation(node_index);
      %! skip subdivision if the current iteration is already lower cost than a possible subdivision
      if current_node.getNodeCost <= best_cost
        fprintf('RETURN || splitting node %d provides no decrease in cost\n', node_index)
        return
      end

      %! [2] split the group of primitives in two halves using the split plane
      split_index = obj.splitPrimitives(node_index, axis_index, location_on_axis_to_split);

      %! skip subdivision if the "split" contains either no triangles or all the triangles in the current node
      num_triangles_on_left = split_index - current_node.getFirstTriangleIndex;
      if num_triangles_on_left == 0 || num_triangles_on_left == current_node.getNumTriangles
        fprintf('RETURN || somehow one side has all the triangles\n')
        return
      end

      %! [3] create child nodes for each half
      left_child_index = obj.createChildNodes(node_index, split_index, num_triangles_on_left);

      %! [4] recurse into each of the child nodes
      obj.subdivideNode(left_child_index);
      obj.subdivideNode(left_child_index + 1);
    end

    function [axis_index, best_position, best_cost] = getSplitLocation(obj, node_index)
      arguments
        obj BVH
        node_index (1,1) uint64
      end
      fprintf('Evaluating optimal split location\n')
      current_node = obj.all_BVHNodes(node_index);
      
      node_span = current_node.getAABBSpan;
      axis_index = node_span.getLongestDirection;
      span_on_longest_direction = node_span.getComponentByAxis(axis_index);
      
      minimum_value_on_axis = current_node.getAABBMin.getComponentByAxis(axis_index);

      num_evaluation_points = obj.num_cost_evaluation_points;
      evaluation_point_costs = zeros(1, num_evaluation_points);
      evaluation_point_positions = zeros(1, num_evaluation_points);
      node_submesh = obj.extractNodeSubMesh( node_index );
      for i = 1 : num_evaluation_points
        evaluation_point_positions(i) = minimum_value_on_axis + span_on_longest_direction * (double(i) / (double(num_evaluation_points) + 1));
        evaluation_point_costs(i) = evaluateNodeChildrenSurfaceAreaHeuristic(current_node, node_index, node_submesh, axis_index, evaluation_point_positions(i));
      end

      [best_cost, best_cost_index] = min(evaluation_point_costs);
      best_position = evaluation_point_positions(best_cost_index);
    end

    function index_at_which_to_split = splitPrimitives(obj, node_index, axis_index, split_location)
      arguments
        obj BVH
        node_index (1,1) uint64
        axis_index (1,1) uint8
        split_location (1,1) double
      end
      fprintf('Splitting primitives on axis %d at %.2f\n', axis_index, split_location)
      current_node = obj.all_BVHNodes(node_index);
      index_at_which_to_split = current_node.getFirstTriangleIndex;
      index_of_last_unsorted_element = index_at_which_to_split + current_node.getNumTriangles - 1;
      while index_at_which_to_split <= index_of_last_unsorted_element
        mesh_element = obj.mesh.getElement(obj.mesh_element_indices(index_at_which_to_split));
        if mesh_element.getCentroid.getComponentByAxis(axis_index) < split_location
          index_at_which_to_split = index_at_which_to_split + 1;
        else
          obj.swapElements(index_at_which_to_split, index_of_last_unsorted_element);
          index_of_last_unsorted_element = index_of_last_unsorted_element - 1;
        end
      end
    end

    function swapElements(obj, index_one, index_two)
      arguments
        obj BVH
        index_one (1,1) uint64
        index_two (1,1) uint64
      end
      value_at_index_one = obj.mesh_element_indices(index_one);
      value_at_index_two = obj.mesh_element_indices(index_two);
      obj.mesh_element_indices(index_one) = value_at_index_two;
      obj.mesh_element_indices(index_two) = value_at_index_one;
    end

    function left_child_index = createChildNodes(obj, node_index, split_index, num_triangles_on_left)
      arguments
        obj BVH
        node_index (1,1) uint64
        split_index (1,1) uint64
        num_triangles_on_left (1,1) uint64
      end
      fprintf('Preparing the children nodes of node %d\n', node_index)
      fprintf('<> prior to creation of children nodes, num_triangles_on_left = %d <>\n', num_triangles_on_left)
      current_node = obj.all_BVHNodes(node_index);

      left_child_index = obj.nodes_used + 1;
      obj.nodes_used = left_child_index + 1;

      current_node.setChildOneIndex(left_child_index);
      
      obj.all_BVHNodes(left_child_index).setFirstTriangleIndex(current_node.getFirstTriangleIndex);
      obj.all_BVHNodes(left_child_index).setNumTriangles(num_triangles_on_left);
      
      obj.all_BVHNodes(left_child_index + 1).setFirstTriangleIndex(split_index);
      obj.all_BVHNodes(left_child_index + 1).setNumTriangles(current_node.getNumTriangles - num_triangles_on_left);
      
      fprintf('<> after creation of children nodes, %d elements on left, %d elements on right\n', ...
        num_triangles_on_left, ...
        current_node.getNumTriangles - num_triangles_on_left)

      current_node.setNumTriangles(0);

      obj.constructNewNode(left_child_index);
      obj.constructNewNode(left_child_index + 1);
    end

    function constructNewNode(obj, node_index)
      arguments
        obj BVH
        node_index (1,1) uint64
      end
      fprintf('Constructing new node %d\n', node_index)
      current_node = obj.all_BVHNodes(node_index);
      fprintf('Node being constructed should contain %d triangles\n', current_node.getNumTriangles)
      fprintf('Submesh being extracted has %d elements\n', length( obj.getSubMeshIndices(node_index) ))
      current_node.growToIncludeMesh( obj.mesh.getSubMesh( obj.getSubMeshIndices(node_index) ) );
    end

    function intersectRayWithBVHNode(obj, ray, node_index)
      arguments
        obj BVH
        ray Ray
        node_index (1,1) uint64
      end
      fprintf('Intersection Test\n')
      current_node = obj.all_BVHNodes(node_index);
      if ~current_node.intersectRayWithNodeBBox(ray)
        return;
      end
      if current_node.isLeaf
        for i = current_node.getFirstTriangleIndex : current_node.getFirstTriangleIndex + current_node.getNumTriangles - 1
          ray.triangleIntersection(obj.mesh.getElement(obj.mesh_element_indices(i)));
        end
      else
        obj.intersectRayWithBVHNode(ray, current_node.child_one_index);
        obj.intersectRayWithBVHNode(ray, current_node.child_one_index + 1);
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
      sub_mesh = obj.mesh.getSubMesh( obj.getSubMeshIndices(node_index) );
    end

    function plotNodeBBox(obj, node_index)
      arguments
        obj BVH
        node_index (1,1) uint64
      end
      b = obj.all_BVHNodes(node_index);
      AABBmin = b.getAABBMin;
      AABBmax = b.getAABBMax;
      all_min = [AABBmin.getX, AABBmin.getY, AABBmin.getZ];   %! all coords are minimum
      yz_x = [AABBmax.getX, AABBmin.getY, AABBmin.getZ];      %! yz min, x max
      z_xy = [AABBmax.getX, AABBmax.getY, AABBmin.getZ];      %! z min, xy max
      xz_y = [AABBmin.getX, AABBmax.getY, AABBmin.getZ];      %! xz min, y max
      xy_z = [AABBmin.getX, AABBmin.getY, AABBmax.getZ];      %! xy min, z max
      y_xz = [AABBmax.getX, AABBmin.getY, AABBmax.getZ];      %! y min, xz max
      all_max = [AABBmax.getX, AABBmax.getY, AABBmax.getZ];   %! all coords are maximum
      x_yz = [AABBmin.getX, AABBmax.getY, AABBmax.getZ];      %! x min, yz max
      vertices = [all_min; yz_x; z_xy; xz_y; xy_z; y_xz; all_max; x_yz];
      faces = [1,2,3,4; ... %! construct all faces by indexing through vertices
               1,2,6,5; ...
               1,4,8,5; ...
               2,3,7,6; ...
               3,4,8,7; ...
               5,6,7,8];
      if b.getNumTriangles > 0
        face_color = 'red';
        face_alpha = 0.3;
      else
        face_color = 'green';
        face_alpha = 0.1;
      end
      patch('Faces', faces, 'Vertices', vertices, 'FaceColor', face_color, 'FaceAlpha', face_alpha);
    end

    function submesh_indices = getSubMeshIndices(obj, node_index)
      arguments
        obj BVH
        node_index (1,1) uint64
      end
      current_node = obj.all_BVHNodes(node_index);
      submesh_indices = obj.mesh_element_indices(current_node.getElementArraySubindices);
    end
  end

  methods (Static)
  end
end