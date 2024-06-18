function Fij = doubleAreaIntegration(cast_ray, emitter_normal, receiver_normal, emitter_area, receiver_area)
  arguments
    cast_ray (1,1) Vector3
    emitter_normal (1,1) Vector3
    receiver_normal (1,1) Vector3
    emitter_area (1,1) double
    receiver_area (1,1) Vector3
  end
  Fij = (-1) / (pi * emitter_area) * (cast_ray.dot(emitter_normal) * cast_ray.dot(receiver_normal) * receiver_area * emitter_area / (cast_ray.dot(cast_ray))^2);
end