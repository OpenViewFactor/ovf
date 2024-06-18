function minimum_vector = vectorMinima(v1, v2)
  arguments
    v1 Vector3
    v2 Vector3
  end
  min_x = min(v1.getX, v2.getX);
  min_y = min(v1.getY, v2.getY);
  min_z = min(v1.getZ, v2.getZ);
  minimum_vector = Vector3(min_x, min_y, min_z);
end