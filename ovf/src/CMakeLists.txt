# Set source files
set(_core_srcs  # define the core source files
    core/Vector3
    core/Triangle.cpp
    core/TriangleMesh.cpp
    core/ViewFactor.cpp
    core/ViewFactorWriter.cpp
    core/STLReader.cpp)

set(_cpp_solver_srcs  # define the solver source files
    solvers/cpp/DoubleAreaIntegration.cpp)

set(_omp_solver_srcs  # define the solver source files
    solvers/omp/DoubleAreaIntegration.cpp)

set(_omp_solver_srcs  # define the solver source files
    solvers/cuda/DoubleAreaIntegration.cpp)

set(_manager_srcs # define the manager source files
    manager/SolverFactory
    manager/SolverSpecifier.cpp)

set(_cli_srcs # define the cli source file
    cli/main.cpp)