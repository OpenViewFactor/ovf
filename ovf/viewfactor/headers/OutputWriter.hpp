#ifndef OVF_OUTPUT_WRITER_H_
#define OVF_OUTPUT_WRITER_H_

#include <fstream>

#include "ViewFactor.hpp"

namespace openviewfactor {
  template <typename FLOAT_TYPE> class OutputWriter {
    private:
      std::shared_ptr<ViewFactor<FLOAT_TYPE>> _vf;
    protected:
    public:
      OutputWriter<FLOAT_TYPE>();
      OutputWriter<FLOAT_TYPE>(std::shared_ptr<ViewFactor<FLOAT_TYPE>> vf);

      bool isLinked() const;
      OutputWriter<FLOAT_TYPE>& linkToViewFactor(std::shared_ptr<ViewFactor<FLOAT_TYPE>> vf);
      
      void writeEmitterVisualization(const std::string& filename) const;
      void writeReceiverVisualization(const std::string& filename) const;
      void writeCombinedVisualization(const std::string& filename) const;
  };
  template class OutputWriter<float>;
  template class OutputWriter<double>;
}

#endif