#ifndef _OVF_SINGLEAREAINTEGRATION_H_
#define _OVF_SINGLEAREAINTEGRATION_H_

#include "Solver.hpp"

namespace openviewfactor {
  template <typename FLOAT_TYPE> class SingleAreaIntegration : public Solver<FLOAT_TYPE> {
    private:
    protected:
    public:
      OVF_HOST_DEVICE FLOAT_TYPE solveViewFactorBetweenElements(const Vector3<FLOAT_TYPE>& emitter_centroid, const Vector3<FLOAT_TYPE>& emitter_normal, const Triangle<FLOAT_TYPE>& receiver_element) const;
  };
}

#endif