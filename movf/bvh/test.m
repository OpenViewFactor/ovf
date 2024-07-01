clear all
close all
clc

layer_height = 20;
minimum_triangles_per_bbox = 2;

mesh = readMesh("assets/rtg_model.stl");
bvh = BVH;
bvh.linkToMesh(mesh);
bvh.setLayerHeight(layer_height);
bvh.setMinimumNumTriangles(minimum_triangles_per_bbox);
bvh.constructBVH;
bvh.plotMesh;
bvh.plotLeafNodes;