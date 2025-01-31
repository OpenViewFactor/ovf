#include "STLReader.hpp"

namespace openviewfactor {
  template <typename FLOAT_TYPE>
  std::shared_ptr<Triangulation<FLOAT_TYPE>> STLReader<FLOAT_TYPE>::getMesh(const std::string &filename) {
    //! set up inputs for the third-party library
    auto mesh = std::make_shared<Triangulation<FLOAT_TYPE>>();
    std::vector<FLOAT_TYPE> coordinates, normals;
    std::vector<size_t> triangulations, solids;
    stl_reader::ReadStlFile(filename.c_str(), coordinates, normals, triangulations, solids);  //! make use of third-party function

    std::vector<std::array<size_t,3>> reformatted_triangulations(triangulations.size() / 3);
    #pragma omp parallel for
    for (int triangle_index = 0; triangle_index < reformatted_triangulations.size(); triangle_index++) {
      reformatted_triangulations[triangle_index] = std::array<size_t, 3>({triangulations[3 * triangle_index + 0],
                                                                          triangulations[3 * triangle_index + 1],
                                                                          triangulations[3 * triangle_index + 2]});
    }

    std::vector<Vector3<FLOAT_TYPE>> reformatted_coordinates(coordinates.size() / 3);
    #pragma omp parallel for
    for (int coordinates_index = 0; coordinates_index < reformatted_coordinates.size(); coordinates_index++) {
      reformatted_coordinates[coordinates_index] = Vector3<FLOAT_TYPE>((coordinates.data())[3 * coordinates_index + 0],
                                                                       (coordinates.data())[3 * coordinates_index + 1],
                                                                       (coordinates.data())[3 * coordinates_index + 2]);
    }

    mesh->setConnectivity(reformatted_triangulations);
    mesh->setPoints(reformatted_coordinates);
    return mesh;
  }

//* ----- Explicit Instantiation ----- *//
template class STLReader<float>;
template class STLReader<double>;
}