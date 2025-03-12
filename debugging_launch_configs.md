# Debugging Launch Configurations

## Windows

```json

```

## Linux

```json
{
  "version": "0.2.0",
  "configurations": [
    {
      "name" : "Perpendicular Plates Config",
      "type" : "cppdbg",
      "request" : "launch",
      "program" : "/home/natan/Development/OpenViewFactor/ovf/build/ovf",
      "args" : ["-g", "NONE", "-p", "DOUBLE", "-n", "DAI", "-c", "CPU_N", "-s", "NONE", "-f", "ON",
        "-i", 
        "/home/natan/Development/OpenViewFactor/verification-scripts/submodules/verification-meshes/prp-rect-approx10000els/horizontal-horzDim=0.1-shrdDim=1.stl",
        "/home/natan/Development/OpenViewFactor/verification-scripts/submodules/verification-meshes/prp-rect-approx10000els/vertical-vertDim=0.1-shrdDim=1.stl"],
      "stopAtEntry" : false,
      "cwd" : "${workspaceRoot}",
      "environment": []
    },
    {
      "name" : "Cylinder-Cylinder",
      "type" : "cppdbg",
      "request" : "launch",
      "program" : "/home/natan/Development/OpenViewFactor/ovf/build/ovf",
      "args" : ["-p", "DOUBLE", "-n", "DAI", "-c", "CPU_N", "-s", "NONE", "-f", "ON",
        "-g", "/home/natan/Downloads/cylinder_e_out",
        "-m", "NONE",
        "-i", 
        "/home/natan/Development/OpenViewFactor/verification-scripts/submodules/verification-meshes/coaxial-cylinders-10844els/inner-r=0.125-h=1.stl",
        "/home/natan/Development/OpenViewFactor/verification-scripts/submodules/verification-meshes/coaxial-cylinders-10844els/outer-r=0.25-h=1.stl"],
      "stopAtEntry" : false,
      "cwd" : "${workspaceRoot}",
      "environment": []
    },
    {
      "name" : "Cylinder-Sphere Blocking",
      "type" : "cppdbg",
      "request" : "launch",
      "program" : "/home/natan/Development/OpenViewFactor/ovf/build/ovf",
      "args" : ["-p", "DOUBLE", "-n", "DAI", "-c", "CPU_N", "-s", "NONE", "-f", "ON",
        "-b", "/home/natan/Downloads/cylinder_blocker_bvh",
        "-g", "/home/natan/Downloads/cylinder_e_out",
        "-m", "NONE",
        "-i", 
        "/home/natan/Development/OpenViewFactor/verification-scripts/submodules/verification-meshes/coaxial-cylinders-10844els/inner-r=0.125-h=1.stl",
        "/home/natan/Development/OpenViewFactor/verification-scripts/submodules/verification-meshes/concentric-spheres/outer_sphere_r=3.stl",
        "-o",
        "/home/natan/Development/OpenViewFactor/verification-scripts/submodules/verification-meshes/coaxial-cylinders-10844els/inner-r=0.25-h=1.stl"],
      "stopAtEntry" : false,
      "cwd" : "${workspaceRoot}",
      "environment": []
    },
    {
      "name" : "Parallel Plates",
      "type" : "cppdbg",
      "request" : "launch",
      "program" : "/home/natan/Development/OpenViewFactor/ovf/build/ovf",
      "args" : ["-p", "DOUBLE", "-n", "DAI", "-c", "CPU_N", "-s", "NONE", "-f", "OFF",
        "-g", "/home/natan/Downloads/new-par-plates-e_out",
        "-m", "NONE",
        "-i", 
        "/home/natan/Development/OpenViewFactor/verification-scripts/submodules/verification-meshes/algnd-par-rect-10034els/bottom-xDim=1-yDim=1.stl",
        "/home/natan/Development/OpenViewFactor/verification-scripts/submodules/verification-meshes/algnd-par-rect-10034els/top-xDim=1-yDim=1-sep=1.stl"],
      "stopAtEntry" : false,
      "cwd" : "${workspaceRoot}",
      "environment": []
    },
    {
      "name" : "Parallel Plates Blocking",
      "type" : "cppdbg",
      "request" : "launch",
      "program" : "/home/natan/Development/OpenViewFactor/ovf/build/ovf",
      "args" : ["-p", "DOUBLE", "-n", "DAI", "-c", "CPU_N", "-s", "NONE", "-f", "OFF",
        "-b", "/home/natan/Downloads/parallel_plate_blocker_bvh",
        "-g", "/home/natan/Downloads/blocked_parallel_plates_e_out",
        "-m", "NONE",
        "-i", 
        "/home/natan/Development/OpenViewFactor/verification-scripts/submodules/verification-meshes/algnd-par-rect-10034els/bottom-xDim=1-yDim=1.stl",
        "/home/natan/Development/OpenViewFactor/verification-scripts/submodules/verification-meshes/algnd-par-rect-10034els/top-xDim=1-yDim=1-sep=1.stl",
        "-o",
        "/home/natan/Development/OpenViewFactor/verification-scripts/submodules/verification-meshes/algnd-par-rect-10034els/blocking_square.stl"],
      "stopAtEntry" : false,
      "cwd" : "${workspaceRoot}",
      "environment": []
    },
    {
      "name" : "Parallel Plates 2 Blockers",
      "type" : "cppdbg",
      "request" : "launch",
      "program" : "/home/natan/Development/OpenViewFactor/ovf/build/ovf",
      "args" : ["-p", "DOUBLE", "-n", "DAI", "-c", "CPU_N", "-s", "NONE", "-f", "OFF",
        "-b", "/home/natan/Downloads/parallel_plates_2_blocker_bvh",
        "-g", "/home/natan/Downloads/blocked_parallel_plates_e_out",
        "-m", "NONE",
        "-i", 
        "/home/natan/Development/OpenViewFactor/verification-scripts/submodules/verification-meshes/algnd-par-rect-10034els/bottom-xDim=1-yDim=1.stl",
        "/home/natan/Development/OpenViewFactor/verification-scripts/submodules/verification-meshes/algnd-par-rect-10034els/top-xDim=1-yDim=1-sep=1.stl",
        "-o",
        "/home/natan/Development/OpenViewFactor/verification-scripts/submodules/verification-meshes/algnd-par-rect-10034els/blocker_1.stl",
        "/home/natan/Development/OpenViewFactor/verification-scripts/submodules/verification-meshes/algnd-par-rect-10034els/blocker_2.stl"],
      "stopAtEntry" : false,
      "cwd" : "${workspaceRoot}",
      "environment": []
    },
    {
      "name" : "Two-Element Parallel Plates",
      "type" : "cppdbg",
      "request" : "launch",
      "program" : "/home/natan/Development/OpenViewFactor/ovf/build/ovf",
      "args" : ["-p", "DOUBLE", "-n", "DAI", "-c", "CPU_N", "-s", "NONE", "-f", "ON",
        "-m", "NONE",
        "-i", 
        "/home/natan/Development/OpenViewFactor/verification-scripts/submodules/verification-meshes/algnd-par-rect-10034els/bottom-2-element.stl",
        "/home/natan/Development/OpenViewFactor/verification-scripts/submodules/verification-meshes/algnd-par-rect-10034els/top-2-element.stl"],
      "stopAtEntry" : false,
      "cwd" : "${workspaceRoot}",
      "environment": []
    },
    {
      "name" : "Two-Element Parallel Plates Blocking",
      "type" : "cppdbg",
      "request" : "launch",
      "program" : "/home/natan/Development/OpenViewFactor/ovf/build/ovf",
      "args" : ["-p", "DOUBLE", "-n", "DAI", "-c", "CPU_N", "-s", "NONE", "-f", "OFF",
        "-b", "/home/natan/Downloads/two-element-square-bvh",
        "-m", "NONE",
        "-i", 
        "/home/natan/Development/OpenViewFactor/verification-scripts/submodules/verification-meshes/algnd-par-rect-10034els/bottom-2-element.stl",
        "/home/natan/Development/OpenViewFactor/verification-scripts/submodules/verification-meshes/algnd-par-rect-10034els/top-2-element.stl",
        "-o",
        "/home/natan/Development/OpenViewFactor/verification-scripts/submodules/verification-meshes/algnd-par-rect-10034els/blocking_square.stl"],
      "stopAtEntry" : false,
      "cwd" : "${workspaceRoot}",
      "environment": []
    },
  ]
}
```