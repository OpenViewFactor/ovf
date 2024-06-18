function facing_wrong_direction = meshDirections(mesh_normals, cast_rays, mesh_type)
  arguments
    mesh_normals (:,1) Vector3
    cast_rays (:,1) Vector3
    mesh_type MeshType
  end
  facing_wrong_direction = logical.empty(length(cast_rays), 0);
  for i=1:length(mesh_normals)
    for j=1:length(cast_rays)/length(mesh_normals)
      full_matrix_index = i * receiver_mesh.numElements + j;
      switch mesh_type
        case MeshType.EMITTER
          facing_wrong_direction(full_matrix_index) = mesh_normals(i).dotProduct(cast_rays(full_matrix_index)) <= 0;
        case MeshType.RECEIVER
          facing_wrong_direction(full_matrix_index) = mesh_normals(i).dotProduct(cast_rays(full_matrix_index)) >= 0;
      end
    end
  end
end