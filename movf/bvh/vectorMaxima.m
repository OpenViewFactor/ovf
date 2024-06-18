function maximum_vector = vectorMaxima(v1, v2)
  arguments
    v1 Vector3
    v2 Vector3
  end
  max_x = max(v1.getX, v2.getX);
  max_y = max(v1.getY, v2.getY);
  max_z = max(v1.getZ, v2.getZ);
  maximum_vector = Vector3(max_x, max_y, max_z);
end