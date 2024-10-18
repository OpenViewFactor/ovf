#include "Vector3.hpp"
#include "Triangle.hpp"
#include "Triangulation.hpp"

#include "STLReader.hpp"
#include <vector>
#include <array>
#include <iostream>

namespace openviewfactor {
  template <typename FLOAT_TYPE>
  std::shared_ptr<Triangulation<FLOAT_TYPE>> STLReader<FLOAT_TYPE>::getMesh(const std::string &filename) {
    //! set up inputs for the third-party library
    auto mesh = std::make_shared<Triangulation<FLOAT_TYPE>>();
    // Triangulation<FLOAT_TYPE> mesh;
    std::vector<FLOAT_TYPE> coordinates, normals;
    std::vector<size_t> triangulations, solids;
    stl_reader::ReadStlFile(filename.c_str(), coordinates, normals, triangulations, solids);  //! make use of third-party function

    std::vector<Vector3<FLOAT_TYPE>> reformatted_coordinates;
    Vector3<FLOAT_TYPE> v;
    for (size_t coordinates_index = 0; coordinates_index < coordinates.size() / 3; coordinates_index++) {

      v.setX((coordinates.data())[3 * coordinates_index + 0]);
      v.setY((coordinates.data())[3 * coordinates_index + 1]);
      v.setZ((coordinates.data())[3 * coordinates_index + 2]);

      reformatted_coordinates.push_back(v);
    }

    for (size_t triangle_index = 0; triangle_index < triangulations.size() / 3; triangle_index++) {
      std::array<Vector3<FLOAT_TYPE>, 3> points = {reformatted_coordinates[(triangulations.data())[3 * triangle_index + 0]],
                                                   reformatted_coordinates[(triangulations.data())[3 * triangle_index + 1]],
                                                   reformatted_coordinates[(triangulations.data())[3 * triangle_index + 2]]};
      Triangle<FLOAT_TYPE> t(points);
      mesh->addElement(t);
    }
    return mesh;
  }

//* ----- Explicit Instantiation ----- *//
template class STLReader<float>;
template class STLReader<double>;
}