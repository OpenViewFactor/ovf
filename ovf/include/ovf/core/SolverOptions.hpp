#ifndef OVF_SOLVEROPTIONS_H_
#define OVF_SOLVEROPTIONS_H_

#include "ovf/core/Triangulation.hpp"
#include <memory>
#include <string>

namespace openviewfactor {
  enum SelfIntersectionMode {
    NONE = 0,
    EMITTER = 1,
    RECEIVER = 2,
    ALL = 3
  };
  template <typename FLOAT_TYPE> class SolverOptions {
    private:
      std::vector<std::shared_ptr<Triangulation<FLOAT_TYPE>>> _blockers;
      SelfIntersectionMode _self_intersection_mode;
    
    public:
    SolverOptions(std::vector<std::shared_ptr<Triangulation<FLOAT_TYPE>>> blockers = {},
                  SelfIntersectionMode self_intersection_mode = ALL)
      : _blockers(blockers), _self_intersection_mode(self_intersection_mode) {}
    
    std::vector<std::shared_ptr<Triangulation<FLOAT_TYPE>>> getBlockers() const { return _blockers; }
    SelfIntersectionMode getSelfIntersectionMode() const { return _self_intersection_mode; }
    size_t getNumBlockers() const { return _blockers.size(); }

    SolverOptions& clearBlockers() {
      _blockers.clear();
      return *this;
    }
    SolverOptions& addBlocker(std::shared_ptr<Triangulation<FLOAT_TYPE>> blocker) {
      _blockers.push_back(blocker);
      return *this;
    }
    SolverOptions& setSelfIntersectionMode(const SelfIntersectionMode &mode) {
      _self_intersection_mode = mode;
      return *this;
    }

  };
}

#endif