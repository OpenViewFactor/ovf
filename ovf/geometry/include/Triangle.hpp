#ifndef OVF_TRIANGLE_FUNCTIONALIZE_HPP_
#define OVF_TRIANGLE_FUNCTIONALIZE_HPP_

#include "config.hpp"
#include "Vector3.hpp"
#include <cmath>
#include <array>
#include <vector>
#include <algorithm>

namespace openviewfactor {
  namespace Triangle {
    template <typename t>
    gpuify std::array<t,3> getOA(const std::array<t,9> t);
    template <typename t>
    gpuify std::array<t,3> getOB(const std::array<t,9> t);
    template <typename t>
    gpuify std::array<t,3> getOC(const std::array<t,9> t);
    template <typename t>
    gpuify std::array<t,3> getNode(const std::array<t,9> t, size_t index);

    template <typename t>
    gpuify std::array<t,3> getAB(const std::array<t,9> t);
    template <typename t>
    gpuify std::array<t,3> getBC(const std::array<t,9> t);
    template <typename t>
    gpuify std::array<t,3> getCA(const std::array<t,9> t);
    template <typename t>
    gpuify std::array<t,3> getEdge(const std::array<t,9> t, size_t index);

    template <typename t>
    gpuify std::array<t,3> getNormal(const std::array<t,9> t);
    template <typename t>
    gpuify std::array<t,3> getCentroid(const std::array<t,9> t);
    template <typename t>
    gpuify t getArea(const std::array<t,9> t);

    template <typename t>
    gpuify t evaluateAspectRatio(const std::array<t,9> t);
    template <typename t>
    gpuify t evaluateElementQuality(const std::array<t,9> t);
    template <typename t>
    gpuify t evaluateSkewness(const std::array<t,9> t);

    template <typename t>
    gpuify bool operator==(const std::array<t,9> t1, std::array<t,9> &t2);
    template <typename t>
    gpuify bool operator!=(const std::array<t,9> t1, std::array<t,9> &t2);
  }
}

#endif