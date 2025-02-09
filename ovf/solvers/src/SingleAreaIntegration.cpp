#include "SingleAreaIntegration.hpp"

namespace openviewfactor {
  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE FLOAT_TYPE SingleAreaIntegration<FLOAT_TYPE>::solveViewFactorBetweenElements(const Vector3<FLOAT_TYPE>& emitter_centroid, const Vector3<FLOAT_TYPE>& emitter_normal, const Triangle<FLOAT_TYPE>& receiver_element) const {
    std::array<Vector3<FLOAT_TYPE>,3> cast_rays = {
      receiver_element.getOA() - emitter_centroid,
      receiver_element.getOB() - emitter_centroid,
      receiver_element.getOC() - emitter_centroid };
    
    std::array<Vector3<FLOAT_TYPE>,3> section_normals = {
      (cast_rays[1]).cross(cast_rays[0]),
      (cast_rays[2]).cross(cast_rays[1]),
      (cast_rays[0]).cross(cast_rays[2]) };

    std::array<FLOAT_TYPE,3> section_projections = {
      (cast_rays[1]).dot(cast_rays[0]),
      (cast_rays[2]).dot(cast_rays[1]),
      (cast_rays[0]).dot(cast_rays[2]) };

    FLOAT_TYPE edge_integral = 0.0;
    for (int i = 0; i < 3; i++) {
      auto section_normal = section_normals[i];
      FLOAT_TYPE scale_factor = 1.0 / section_normal.getMagnitude();
      edge_integral += ( dot(section_normal, emitter_normal) * scale_factor ) * ( OVF_PI*0.5 - std::atan(section_projections[i] * scale_factor) );
    }

    FLOAT_TYPE scale_factor = 1.0 / (2.0*OVF_PI);
    auto fij = (edge_integral * scale_factor);
    if (std::isnan(fij)) {
      return 0.0;
    } else {
      return fij;
    }
  }

  template class SingleAreaIntegration<float>;
  template class SingleAreaIntegration<double>;
}