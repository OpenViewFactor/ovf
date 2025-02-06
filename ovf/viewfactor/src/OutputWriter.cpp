#include "OutputWriter.hpp"

namespace openviewfactor {
  template <typename FLOAT_TYPE>
  OutputWriter<FLOAT_TYPE>::OutputWriter()
    : _vf(nullptr) {};

  template <typename FLOAT_TYPE>
  OutputWriter<FLOAT_TYPE>::OutputWriter(std::shared_ptr<ViewFactor<FLOAT_TYPE>> vf)
    : _vf(vf) {};
  
  template <typename FLOAT_TYPE>
  bool OutputWriter<FLOAT_TYPE>::isLinked() const { return (_vf != nullptr); }
  template <typename FLOAT_TYPE>
  OutputWriter<FLOAT_TYPE>& OutputWriter<FLOAT_TYPE>::linkToViewFactor(std::shared_ptr<ViewFactor<FLOAT_TYPE>> vf) {
    _vf.reset();
    _vf = vf;
    return *this;
  }

  template <typename FLOAT_TYPE>
  void OutputWriter<FLOAT_TYPE>::writeEmitterVisualization(const std::string& filename) const {
    if (!(this->isLinked())) { throw std::runtime_error("Cannot write .vtu output with no ViewFactor linked"); }
    if (!(_vf->isLinked())) { throw std::runtime_error("Cannot write .vtu output with a ViewFactor object that is not linked to a mesh"); }

    int dimension = 3;
    int cell_size = 3;
    auto emitter_mesh = _vf->getEmitterMesh();
    auto num_emitter_elements = emitter_mesh->getNumElements();
    std::vector<int> triangulations(num_emitter_elements * cell_size);
    std::iota(triangulations.begin(), triangulations.end(), 0);
    std::vector<double> vertices(num_emitter_elements * cell_size * dimension);
    std::vector<double> view_factors(num_emitter_elements * cell_size);
    
    for (int i = 0; i < num_emitter_elements; i++) {
      vertices[9 * i + 0] = (((*emitter_mesh)[i]).getOA())[0];
      vertices[9 * i + 1] = (((*emitter_mesh)[i]).getOA())[1];
      vertices[9 * i + 2] = (((*emitter_mesh)[i]).getOA())[2];

      vertices[9 * i + 3] = (((*emitter_mesh)[i]).getOB())[0];
      vertices[9 * i + 4] = (((*emitter_mesh)[i]).getOB())[1];
      vertices[9 * i + 5] = (((*emitter_mesh)[i]).getOB())[2];

      vertices[9 * i + 6] = (((*emitter_mesh)[i]).getOC())[0];
      vertices[9 * i + 7] = (((*emitter_mesh)[i]).getOC())[1];
      vertices[9 * i + 8] = (((*emitter_mesh)[i]).getOC())[2];

      auto element_view_factor = _vf->getEmitterElementToReceiverSurfaceVF(i); 
      view_factors[3 * i + 0] = element_view_factor;
      view_factors[3 * i + 1] = element_view_factor;
      view_factors[3 * i + 2] = element_view_factor;
    }

    leanvtk::VTUWriter writer;
    writer.add_scalar_field("View Factor", view_factors);
    writer.write_surface_mesh(filename, dimension, cell_size, vertices, triangulations);
  }

  template <typename FLOAT_TYPE>
  void OutputWriter<FLOAT_TYPE>::writeReceiverFromEmitterVisualization(const std::string& filename) const {
    //TODO write method for outputting a .vtu file for receiving surface
  }

  template <typename FLOAT_TYPE>
  void OutputWriter<FLOAT_TYPE>::writeReceiverToEmitterVisualization(const std::string& filename) const {
    //TODO write method for outputting a .vtu file for receiving surface
  }

  template <typename FLOAT_TYPE>
  void OutputWriter<FLOAT_TYPE>::writeCombinedVisualization(const std::string& filename) const {
    //TODO write method for outputting a .vtu file for both surfaces in one
  }
}