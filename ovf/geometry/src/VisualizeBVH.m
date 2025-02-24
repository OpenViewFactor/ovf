% Visualize Boundary Volume Hierarchies

clear all
close all
clc

file_path = input("Enter the full BVH filepath: ", "s");
stl_path = input("Enter the full STL filepath: ", "s");
bvh_stl = stlread(stl_path);

file_contents = readtable(file_path,"EmptyLineRule","skip");
file_contents.Type = categorical(file_contents{:, "Encoding"});
mesh_points = file_contents{file_contents.Type == "p", ["X","Y","Z"]};
mesh_connections = file_contents{file_contents.Type == "c", ["X","Y","Z"]};

T = triangulation(mesh_connections, mesh_points);

bvh = trisurf(T, 'EdgeColor', 'none', 'FaceColor', 'r', 'FaceAlpha', 0.2);
hold on
stlmesh = trisurf(bvh_stl, 'FaceColor', 'b', 'FaceAlpha', 0.1);
daspect([1,1,1])
grid off