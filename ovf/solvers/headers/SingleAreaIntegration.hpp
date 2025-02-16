#ifndef _OVF_SINGLEAREAINTEGRATION_H_
#define _OVF_SINGLEAREAINTEGRATION_H_

#include "Solver.hpp"

namespace openviewfactor {
  template <typename t> class SingleAreaIntegration : public Solver<t> {
    private:
    protected:
    public:
      gpuify t solveViewFactorBetweenElements(const Vector3<t>& emitter_centroid, const Vector3<t>& emitter_normal, const Triangle<t>& receiver_element) const;
  };
}

#endif