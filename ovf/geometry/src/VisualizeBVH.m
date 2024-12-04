% Visualize Boundary Volume Hierarchies

clear all
close all
clc

file_path = input("Enter the full BVH filepath: ", "s");
stl_path = input("Enter the full STL filepath: ", "s");
bvh_stl = stlread(stl_path);

file_contents = readtable(file_path,"EmptyLineRule","skip","ReadRowNames",true)
row_names = file_contents.Properties.RowNames;

mesh_points = double.empty(0,3);
mesh_connections = double.empty(0,3);

for i = 1 : length(row_names)
  if row_names{i}(1) == 'p'
    mesh_points = [mesh_points; ...
      file_contents(row_names{i},"X").Variables, ...
      file_contents(row_names{i},"Y").Variables, ...
      file_contents(row_names{i},"Z").Variables];
  elseif row_names{i}(1) == 'c'
    mesh_connections = [mesh_connections; ...
      file_contents(row_names{i},"X").Variables, ...
      file_contents(row_names{i},"Y").Variables, ...
      file_contents(row_names{i},"Z").Variables];
  end
end

mesh_points
mesh_connections

T = triangulation(mesh_connections, mesh_points);

trisurf(T, 'EdgeColor', 'k', 'FaceColor', 'r', 'FaceAlpha', 0.4)
hold on
trisurf(bvh_stl, 'FaceColor', 'b', 'FaceAlpha', 0.2)
daspect([1,1,1])