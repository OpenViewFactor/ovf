#include "geometry/include/STLReader.hpp"
#include <vector>

namespace openviewfactor {
  template <typename FLOAT_TYPE>
  Triangulation<FLOAT_TYPE> STLReader<FLOAT_TYPE>::getMesh(const std::string &filename) {
    //! set up inputs for the third-party library
    Triangulation<FLOAT_TYPE> mesh(filename);
    std::vector<FLOAT_TYPE> coordinates, normals;
    std::vector<size_t> triangulations, solids;
    stl_reader::ReadStlFile(filename.c_str(), coordinates, normals, triangulations, solids);  //! make use of third-party function

    for (size_t i = 0; i < (triangulations.size() / 3); i++) {
      Vector3<FLOAT_TYPE> OA = (reinterpret_cast<Vector3<FLOAT_TYPE> *>(coords.data()))[triangulations[3*i + 0]];
      Vector3<FLOAT_TYPE> OB = (reinterpret_cast<Vector3<FLOAT_TYPE> *>(coords.data()))[triangulations[3*i + 1]];
      Vector3<FLOAT_TYPE> OC = (reinterpret_cast<Vector3<FLOAT_TYPE> *>(coords.data()))[triangulations[3*i + 2]];
      Triangle<FLOAT_TYPE> t(OA, OB, OC);

      mesh.addElem(t);
    }
    return mesh;
  }

//* ----- Explicit Instantiation ----- *//
template class STLReader<float>;
template class STLReader<double>;
}