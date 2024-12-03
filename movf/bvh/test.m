clear all
close all
clc

warning('off','all')

% delete(gcp('nocreate'))
% parpool
% poolObj = gcp;

filename = input("Enter the mesh filepath: ", "s");
mesh = readMesh(filename);
bvh = BVH;
bvh.linkToMesh(mesh);
bvh.setMinimumNumTriangles(2);
construction_start = tic;
bvh.constructBVH;
total_construction_time = toc(construction_start);

fprintf("nodes used: %d\n", bvh.nodes_used);

figure
hold on
bvh.plotLeafNodes;

ray1 = Ray;
ray1.setOrigin(Vector3(125, 0, 30));
ray1.setDirection(Vector3(0, -1, 0));
intersection_timer_start = tic;
bvh.intersectRayWithBVH(ray1);
total_intersection_time = toc(intersection_timer_start);
ray1.plotRay;

fprintf('BVH construction took %.1f [s]\n', total_construction_time)
fprintf('Ray intersection took %.10f [s]\n', total_intersection_time)