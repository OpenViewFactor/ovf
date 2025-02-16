#ifndef OVF_VECTOR3_FUNCTIONALIZE_HPP_
#define OVF_VECTOR3_FUNCTIONALIZE_HPP_

#include "config.hpp"
#include <array>
#include <cmath>

namespace openviewfactor {
  namespace Vector3 {
    template <typename t>
    gpuify size_t getVectorLongestAxis(std::array<t,3> &v);

    template <typename t>
    gpuify t getMagnitude(std::array<t,3> &v);

    template <typename t>
    gpuify std::array<t,3>& normalize(std::array<t,3> &v);

    template <typename t>
    gpuify std::array<t,3>& scale(std::array<t,3> &v, t scale);

    template <typename t>
    gpuify std::array<t,3>& flip(std::array<t,3> &v);

    template <typename t>
    gpuify t dot(const std::array<t,3> &a, const std::array<t,3> &b);

    template <typename t>
    gpuify std::array<t,3> cross(const std::array<t,3> &a, const std::array<t,3> &b);

    template <typename t>
    gpuify std::array<t,3> operator+(const std::array<t,3> &a, const std::array<t,3> &b);

    template <typename t>
    gpuify std::array<t,3> operator-(const std::array<t,3> &a, const std::array<t,3> &b);

    template <typename t>
    gpuify bool operator==(const std::array<t,3> &a, const std::array<t,3> &b);

    template <typename t>
    gpuify bool operator!=(const std::array<t,3> &a, const std::array<t,3> &b);

    template <typename t>
    gpuify std::array<t,3> vectorMinima(const std::array<t,3> &a, const std::array<t,3> &b);

    template <typename t>
    gpuify std::array<t,3> vectorMaxima(const std::array<t,3> &a, const std::array<t,3> &b);
  }
}

#endif