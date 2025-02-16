#ifndef OVF_TRIANGULATION_FUNCTIONALIZE_HPP_
#define OVF_TRIANGULATION_FUNCTIONALIZE_HPP_

#include "config.hpp"
#include "Vector3.hpp"
#include "Triangle.hpp"
#include <array>
#include <vector>
#include <algorithm>
#include <cmath>
#include <memory>
#include <numeric>

namespace openviewfactor {
  namespace Triangulation {
    template <typename t>
    gpuify std::array<t,9> getElement(const std::vector<size_t> &triangulations, const std::vector<t> &points, size_t index);

    template <typename t>
    gpuify t getArea(const std::vector<size_t> &triangulations, const std::vector<t> &points);

    template <typename t>
    gpuify void getSubMesh(const std::vector<size_t> &triangulations, const std::vector<t> &points, std::vector<size_t> &sub_mesh_triangulations, std::vector<t> &sub_mesh_points);

    template <typename t>
    gpuify void getCentroids(const std::vector<size_t> &triangulations, const std::vector<t> &points, std::vector<std::array<t,3>> &centroids);

    template <typename t>
    gpuify void getNormals(const std::vector<size_t> &triangulations, const std::vector<t> &points, std::vector<std::array<t,3>> &normals);

    template <typename t>
    gpuify void getAreas(const std::vector<size_t> &triangulations, const std::vector<t> &points, std::vector<t> &areas);

    template <typename t>
    gpuify void getTriangles(const std::vector<size_t> &triangulations, const std::vector<t> &points, std::vector<std::array<t,9>> &triangles);

    template <typename t>
    gpuify void addElement(const std::vector<size_t> &triangulations, const std::vector<t> &points, std::array<t,9> &t);


    //* ----- MESH QUALITY ANALYSIS ----- *//
    template <typename t>
    gpuify void getAllAspectRatios(const std::vector<size_t> &triangulations, const std::vector<t> &points, std::vector<t> &aspect_ratios);
    template <typename t>
    gpuify void getAllElementQualities(const std::vector<size_t> &triangulations, const std::vector<t> &points, std::vector<t> &element_qualities);
    template <typename t>
    gpuify void getAllSkewness(const std::vector<size_t> &triangulations, const std::vector<t> &points, std::vector<t> &skewness);
  }
}

#endif