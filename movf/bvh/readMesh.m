function mesh = readMesh(filepath)
  arguments
    filepath string
  end
  matlab_triangulation = stlread(filepath);
  double_points = matlab_triangulation.Points;
  vector3_points = Vector3.empty(size(double_points, 1), 0);
  for i = 1 : size(double_points, 1)
    vector3_points(i) = Vector3(double_points(i, 1), double_points(i, 2), double_points(i, 3));
  end
  connectivity = matlab_triangulation.ConnectivityList;
  mesh = Mesh;
  mesh.setPoints(vector3_points);
  mesh.setConnectivity(connectivity);
end