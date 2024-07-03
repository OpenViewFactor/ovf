clear all
close all
clc

filename = input("Enter the mesh filepath: ", "s");
layer_height = input("Enter a layer height for the BVH: ");
minimum_triangles_per_bbox = input("Enter the minimum number of triangles per BV: ");

mesh = readMesh(filename);
bvh = BVH;
bvh.linkToMesh(mesh);
bvh.setLayerHeight(layer_height);
bvh.setMinimumNumTriangles(minimum_triangles_per_bbox);
bvh.constructBVH;
bvh.plotLeafNodes;