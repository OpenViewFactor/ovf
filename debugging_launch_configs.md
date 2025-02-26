# Debugging Launch Configurations

## Windows

```json
{
  "version": "0.2.0",
  "configurations": [
    {
      "name" : "Perpendicular Plates Config",
      "type" : "cppvsdbg",
      "request" : "launch",
      "program" : "C:\\Development\\OpenViewFactor\\ovf\\build\\ovf\\cli\\Debug\\openviewfactor.exe",
      "args" : ["-g", "NONE", "-p", "DOUBLE", "-n", "DAI", "-c", "CPU_N", "-s", "NONE", "-f", "ON",
        "-i", 
        "C:\\Development\\OpenViewFactor\\verification-scripts\\submodules\\verification-meshes\\prp-rect-approx10000els\\horizontal-horzDim=0.1-shrdDim=1.stl",
        "C:\\Development\\OpenViewFactor\\verification-scripts\\submodules\\verification-meshes\\prp-rect-approx10000els\\vertical-vertDim=0.1-shrdDim=1.stl"],
      "stopAtEntry" : false,
      "cwd" : "${workspaceRoot}",
      "environment": [],
      "console" : "integratedTerminal"
    },
    {
      "name" : "Cylinder-Sphere Cylinder Blocking",
      "type" : "cppvsdbg",
      "request" : "launch",
      "program" : "C:\\Development\\OpenViewFactor\\ovf\\build\\ovf\\cli\\Debug\\openviewfactor.exe",
      "args" : ["-g", "C:\\Development\\OpenViewFactor\\e_out", "C:\\Development\\OpenViewFactor\\r_out", "-p", "DOUBLE", "-n", "DAI", "-c", "CPU_N", "-s", "NONE", "-f", "ON",
        "-i", 
        "C:\\Development\\OpenViewFactor\\verification-scripts\\submodules\\verification-meshes/coaxial-cylinders-10844els/inner-r=0.125-h=1.stl",
        "C:\\Development\\OpenViewFactor\\verification-scripts\\submodules\\verification-meshes/concentric-spheres/outer_sphere_r=3.stl",
        "-b",
        "C:\\Development\\OpenViewFactor\\verification-scripts\\submodules\\verification-meshes/coaxial-cylinders-10844els/inner-r=0.25-h=1.stl"],
      "stopAtEntry" : false,
      "cwd" : "${workspaceRoot}",
      "environment": [],
      "console" : "integratedTerminal"
    },
    {
      "name" : "Square-Square Square Blocking",
      "type" : "cppvsdbg",
      "request" : "launch",
      "program" : "C:\\Development\\OpenViewFactor\\ovf\\build\\ovf\\cli\\Debug\\openviewfactor.exe",
      "args" : ["-g", "C:\\Development\\OpenViewFactor\\e_out", "C:\\Development\\OpenViewFactor\\r_out", "-p", "DOUBLE", "-n", "DAI", "-c", "CPU_N", "-s", "NONE", "-f", "OFF",
        "-i", 
        "C:\\Development\\OpenViewFactor\\verification-scripts\\submodules\\verification-meshes\\algnd-par-rect-10034els\\bottom-xDim=1-yDim=1.stl",
        "C:\\Development\\OpenViewFactor\\verification-scripts\\submodules\\verification-meshes\\algnd-par-rect-10034els\\top-xDim=1-yDim=1-sep=1.stl",
        "-b",
        "C:\\Development\\OpenViewFactor\\blocking_square.stl"],
      "stopAtEntry" : false,
      "cwd" : "${workspaceRoot}",
      "environment": [],
      "console" : "integratedTerminal"
    }
  ]
}
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