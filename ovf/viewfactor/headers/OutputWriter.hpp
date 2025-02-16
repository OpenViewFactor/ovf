#ifndef OVF_OUTPUT_WRITER_H_
#define OVF_OUTPUT_WRITER_H_

#include <fstream>
#include <algorithm>

#include "ViewFactor.hpp"

#include "lean_vtk.hpp"

namespace openviewfactor {
  template <typename t> class OutputWriter {
    private:
      std::shared_ptr<ViewFactor<t>> _vf;
    protected:
    public:
      OutputWriter();
      OutputWriter(std::shared_ptr<ViewFactor<t>> vf);

      bool isLinked() const;
      OutputWriter<t>& linkToViewFactor(std::shared_ptr<ViewFactor<t>> vf);
      
      void writeEmitterVisualization(const std::string& filename) const;
      void writeReceiverFromEmitterVisualization(const std::string& filename) const;
      void writeReceiverToEmitterVisualization(const std::string& filename) const;
      void writeCombinedVisualization(const std::string& filename) const;
  };
  template class OutputWriter<float>;
  template class OutputWriter<double>;
}

#endif