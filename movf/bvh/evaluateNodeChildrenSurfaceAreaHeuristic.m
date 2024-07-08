function node_cost = evaluateNodeChildrenSurfaceAreaHeuristic(current_node, node_index, node_submesh, axis_index, candidate_position)
  arguments
    current_node BVHNode
    node_index (1,1) uint64
    node_submesh Mesh
    axis_index (1,1) uint64
    candidate_position (1,1) double
  end
  fprintf('Evaluating surface area heuristic for node %d on axis %d at %.2f\n', node_index, axis_index, candidate_position)
  left_box = BVHNode; right_box = BVHNode;

  left_container = zeros(1, current_node.getNumTriangles);
  right_container = zeros(1, current_node.getNumTriangles);
  triangles = node_submesh.getTriangles;
  for i = 1 : current_node.getNumTriangles
    triangle = triangles(i);
    if triangle.getCentroid.getComponentByAxis(axis_index) < candidate_position
      left_container(i) = 1;
    else
      right_container(i) = 1;
    end
  end

  element_indices = 1 : current_node.getNumTriangles + 1;
  left_submesh_indices = element_indices(logical(left_container));
  right_submesh_indices = element_indices(logical(right_container));
  left_box.growToIncludeMesh(node_submesh.getSubMesh(left_submesh_indices));
  right_box.growToIncludeMesh(node_submesh.getSubMesh(right_submesh_indices));

  left_count = sum(left_container);
  right_count = sum(right_container);
  left_cost = left_box.getSurfaceArea * left_count;
  right_cost = right_box.getSurfaceArea * right_count;
  node_cost = 0;
  if ~isnan(left_cost)
    node_cost = node_cost + left_cost;
  end
  if ~isnan(right_cost)
    node_cost = node_cost + right_cost;
  end
  if node_cost == 0
    node_cost = inf;
  end
  fprintf('left cost = %.0f || right cost = %.0f || total node cost = %.0f\n', left_cost, right_cost, node_cost)
  fprintf('left # tri = %d || right # tri = %d || total # tri = %d\n', left_count, right_count, left_count + right_count)
  fprintf('left SA = %.0f || right SA = %.0f\n', left_box.getSurfaceArea, right_box.getSurfaceArea)
end