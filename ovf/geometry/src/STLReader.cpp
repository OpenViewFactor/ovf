#include "STLReader.hpp"
#include <vector>

namespace openviewfactor {
  template <typename FLOAT_TYPE>
  Triangulation<FLOAT_TYPE> STLReader<FLOAT_TYPE>::getMesh(const std::string &filename) {
    //! set up inputs for the third-party library
    Triangulation<FLOAT_TYPE> mesh;
    std::vector<FLOAT_TYPE> coordinates, normals;
    std::vector<size_t> triangulations, solids;
    stl_reader::ReadStlFile(filename.c_str(), coordinates, normals, triangulations, solids);  //! make use of third-party function

    std::vector<Vector3<FLOAT_TYPE>> coordinates_vector;
    for (unsigned int point_index = 0; point_index < coordinates.size()/3; point_index++) {
      Vector3<FLOAT_TYPE> v(coordinates[(point_index*3)+0], coordinates[(point_index*3)+1], coordinates[(point_index*3)+2]);
      coordinates_vector.push_back(v);
    }

    std::vector<std::array<size_t, 3>> triangulations_vector;
    for (unsigned int triangle_index = 0; triangle_index < triangulations.size()/3; triangle_index++) {
      std::array<size_t, 3> v = {triangulations[(triangle_index*3)+0], triangulations[(triangle_index*3)+1], triangulations[(triangle_index*3)+2]};
      triangulations_vector.push_back(v);
    }
    mesh.setPoints(coordinates_vector).setConnectivity(triangulations_vector);
    return mesh;
  }

//* ----- Explicit Instantiation ----- *//
template class STLReader<float>;
template class STLReader<double>;
}