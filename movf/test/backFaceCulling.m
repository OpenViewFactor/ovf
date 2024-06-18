function culled_element_indices = backFaceCulling(emitter_normals, receiver_normals, cast_rays)
  arguments
    emitter_normals (:,1) Vector3
    receiver_normals (:,1) Vector3
    cast_rays (:,1) Vector3
  end
  emitter_facing_wrong_direction = meshDirections(emitter_normals, cast_rays, MeshType.EMITTER);
  receiver_facing_wrong_direction = meshDirections(receiver_normals, cast_rays, MeshType.RECEIVER);

  culled_element_indices = or(emitter_facing_wrong_direction, receiver_facing_wrong_direction);
end