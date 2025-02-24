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
      "program" : "/home/natan/Development/OpenViewFactor/ovf/build/ovf/cli/openviewfactor",
      "args" : ["-g", "NONE", "-p", "DOUBLE", "-n", "DAI", "-c", "CPU_N", "-s", "NONE", "-f", "ON",
        "-i", 
        "/home/natan/Development/OpenViewFactor/verification-scripts/submodules/verification-meshes/prp-rect-approx10000els/horizontal-horzDim=0.1-shrdDim=1.stl",
        "/home/natan/Development/OpenViewFactor/verification-scripts/submodules/verification-meshes/prp-rect-approx10000els/vertical-vertDim=0.1-shrdDim=1.stl"],
      "stopAtEntry" : false,
      "cwd" : "${workspaceRoot}",
      "environment": []
    },
    {
      "name" : "Cylinder-Cylinder Blocking",
      "type" : "cppdbg",
      "request" : "launch",
      "program" : "/home/natan/Development/OpenViewFactor/ovf/build/ovf/cli/openviewfactor",
      "args" : ["-g", "NONE", "-p", "DOUBLE", "-n", "DAI", "-c", "CPU_N", "-s", "NONE", "-f", "ON",
        "-i", 
        "/home/natan/Development/OpenViewFactor/verification-scripts/submodules/verification-meshes/coaxial-cylinders-10844els/inner-r=0.125-h=1.stl",
        "/home/natan/Development/OpenViewFactor/verification-scripts/submodules/verification-meshes/concentric-spheres/outer_sphere_r=3.stl",
        "-b",
        "/home/natan/Development/OpenViewFactor/verification-scripts/submodules/verification-meshes/coaxial-cylinders-10844els/inner-r=0.25-h=1.stl"],
      "stopAtEntry" : false,
      "cwd" : "${workspaceRoot}",
      "environment": []
    }
  ]
}
```