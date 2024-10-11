#include "Vector3.hpp"
#include "Triangle.hpp"
#include "Triangulation.hpp"

#include "STLReader.hpp"
#include <vector>
#include <iostream>

namespace openviewfactor {
  template <typename FLOAT_TYPE>
  Triangulation<FLOAT_TYPE> STLReader<FLOAT_TYPE>::getMesh(const std::string &filename) {
    //! set up inputs for the third-party library
    Triangulation<FLOAT_TYPE> mesh;
    std::vector<FLOAT_TYPE> coordinates, normals;
    std::vector<size_t> triangulations, solids;
    stl_reader::ReadStlFile(filename.c_str(), coordinates, normals, triangulations, solids);  //! make use of third-party function

    std::vector<Vector3<FLOAT_TYPE>> reformatted_coordinates;
    Vector3<FLOAT_TYPE> v;
    for (size_t coordinates_index = 0; coordinates_index < coordinates.size() / 3; coordinates_index++) {

      std::cout << "coordinates: {" << (coordinates.data())[3 * coordinates_index + 0] << "," << (coordinates.data())[3 * coordinates_index + 1] << "," << (coordinates.data())[3 * coordinates_index + 2] << "}" << std::endl;

      v.setX((coordinates.data())[3 * coordinates_index + 0]);
      v.setY((coordinates.data())[3 * coordinates_index + 1]);
      v.setZ((coordinates.data())[3 * coordinates_index + 2]);

      reformatted_coordinates.push_back(v);
    }

    for (size_t triangle_index = 0; triangle_index < triangulations.size() / 3; triangle_index++) {
      Triangle<FLOAT_TYPE> t;

      std::cout << "reformatted coordinates: {" << reformatted_coordinates[(triangulations.data())[3 * triangle_index + 0]][0] << "," << reformatted_coordinates[(triangulations.data())[3 * triangle_index + 0]][1] << "," << reformatted_coordinates[(triangulations.data())[3 * triangle_index + 0]][2] << "}" << "   {";

      std::cout << reformatted_coordinates[(triangulations.data())[3 * triangle_index + 1]][0] << "," << reformatted_coordinates[(triangulations.data())[3 * triangle_index + 1]][1] << "," << reformatted_coordinates[(triangulations.data())[3 * triangle_index + 1]][2] << "}" << "   {";

      std::cout << reformatted_coordinates[(triangulations.data())[3 * triangle_index + 2]][0] << "," << reformatted_coordinates[(triangulations.data())[3 * triangle_index + 2]][1] << "," << reformatted_coordinates[(triangulations.data())[3 * triangle_index + 2]][2] << "}" << std::endl;

      t.setOA(reformatted_coordinates[(triangulations.data())[3 * triangle_index + 0]]);
      t.setOB(reformatted_coordinates[(triangulations.data())[3 * triangle_index + 1]]);
      t.setOC(reformatted_coordinates[(triangulations.data())[3 * triangle_index + 2]]);
      mesh.addElement(t);

      std::cout << "added element centroid: {" << mesh[triangle_index].getCentroid()[0] << "," << mesh[triangle_index].getCentroid()[1] << "," << mesh[triangle_index].getCentroid()[2] << "}" << std::endl;
    }
    return mesh;
  }

//* ----- Explicit Instantiation ----- *//
template class STLReader<float>;
template class STLReader<double>;
}