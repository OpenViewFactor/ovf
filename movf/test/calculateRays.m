function cast_rays = calculateRays(emitter_centroids, receiver_centroids)
  arguments
    emitter_centroids (:,1) Vector3
    receiver_centroids (:,1) Vector3
  end
  cast_rays = Vector3.empty(length(emitter_centroids) * length(receiver_centroids), 0);
  for i=1:size(emitter_centroids)
    for j=1:size(receiver_centroids)
      full_matrix_index = i * receiver_mesh.numElements + j;
      cast_rays(full_matrix_index) = receiver_centroids(j) - emitter_centroids(i);
    end
  end
end