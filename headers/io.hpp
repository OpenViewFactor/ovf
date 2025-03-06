#include "all_headers.hpp"

#include "geometry.hpp"
#include "solver.hpp"
#include "results.hpp"

#pragma once

//! ----- INPUT/OUTPUT ----- !//

namespace io {

template <typename T> void writeToFile(geometry::mesh<T>* m, const std::string& filename) {
  int dimension = 3;
  int cell_size = 3;
  auto num_elements = m->size();
  std::vector<int> triangulations(num_elements * cell_size);
  std::vector<double> points(num_elements * cell_size * dimension);

  for (int i = 0; i < num_elements; i++) {
    geometry::tri<T> t = (*m)[i];
    points[i*cell_size*dimension + 0] = (double)t[0][0];
    points[i*cell_size*dimension + 1] = (double)t[0][1];
    points[i*cell_size*dimension + 2] = (double)t[0][2];
    points[i*cell_size*dimension + 3] = (double)t[1][0];
    points[i*cell_size*dimension + 4] = (double)t[1][1];
    points[i*cell_size*dimension + 5] = (double)t[1][2];
    points[i*cell_size*dimension + 6] = (double)t[2][0];
    points[i*cell_size*dimension + 7] = (double)t[2][1];
    points[i*cell_size*dimension + 8] = (double)t[2][2];

    triangulations[i*cell_size + 0] = i*cell_size + 0;
    triangulations[i*cell_size + 1] = i*cell_size + 1;
    triangulations[i*cell_size + 2] = i*cell_size + 2;
  }

  leanvtk::VTUWriter writer;
  writer.write_surface_mesh(filename, dimension, cell_size, points, triangulations);
}

template <typename T> void writeToFile(geometry::BVH<T>* bvh, const std::string& filename) {
  int dimension = 3;
  int cell_size = 4;

  uint num_leaf_nodes = 0;
  uint nodes_used = (*bvh)._nodes_used;
  std::vector<uint> leaf_nodes_indices(nodes_used);
  std::iota(leaf_nodes_indices.begin(), leaf_nodes_indices.end(), 0);
  for (int i = 0; i < nodes_used; i++) {
    if ((*bvh)[i]->isLeaf()) {
      num_leaf_nodes++;
    } else {
      leaf_nodes_indices[i] = nodes_used;
    }
  }
  auto it = std::remove(leaf_nodes_indices.begin(), leaf_nodes_indices.end(), nodes_used);
  leaf_nodes_indices.erase(it, leaf_nodes_indices.end());

  std::vector<double> vertices(num_leaf_nodes * 8 * 3);
  std::vector<int> connectivity(num_leaf_nodes * 6 * 4);

  for (int i = 0; i < leaf_nodes_indices.size(); i++) {
    uint node_index = leaf_nodes_indices[i];

    //* adding bounding box vertices to vertices vector
    geometry::v3<T> bbmin = ((*bvh)[node_index])->min();
    geometry::v3<T> bbmax = ((*bvh)[node_index])->max();
    geometry::v3<double> p0((double)bbmin[0],(double)bbmin[1],(double)bbmin[2]);
    geometry::v3<double> p1((double)bbmax[0],(double)bbmin[1],(double)bbmin[2]);
    geometry::v3<double> p2((double)bbmax[0],(double)bbmax[1],(double)bbmin[2]);
    geometry::v3<double> p3((double)bbmin[0],(double)bbmax[1],(double)bbmin[2]);
    geometry::v3<double> p4((double)bbmin[0],(double)bbmax[1],(double)bbmax[2]);
    geometry::v3<double> p5((double)bbmin[0],(double)bbmin[1],(double)bbmax[2]);
    geometry::v3<double> p6((double)bbmax[0],(double)bbmin[1],(double)bbmax[2]);
    geometry::v3<double> p7((double)bbmax[0],(double)bbmax[1],(double)bbmax[2]);
    std::array<geometry::v3<double>,8> points = {p0, p1, p2, p3, p4, p5, p6, p7};
    for (int j = 0; j < points.size(); j++) {
      for (int k = 0; k < 3; k++) {
        vertices[8*3*i + 3*j + k] = points[j][k];
      }
    }

    //* adding bounding box face connectivities to connectivity vector
    uint num_points_filled = i*8;
    connectivity[i*6*4 + 0] = num_points_filled + 0;
    connectivity[i*6*4 + 1] = num_points_filled + 1;
    connectivity[i*6*4 + 2] = num_points_filled + 6;
    connectivity[i*6*4 + 3] = num_points_filled + 5;

    connectivity[i*6*4 + 4] = num_points_filled + 0;
    connectivity[i*6*4 + 5] = num_points_filled + 3;
    connectivity[i*6*4 + 6] = num_points_filled + 2;
    connectivity[i*6*4 + 7] = num_points_filled + 1;

    connectivity[i*6*4 + 8] = num_points_filled + 0;
    connectivity[i*6*4 + 9] = num_points_filled + 5;
    connectivity[i*6*4 + 10] = num_points_filled + 4;
    connectivity[i*6*4 + 11] = num_points_filled + 3;

    connectivity[i*6*4 + 12] = num_points_filled + 7;
    connectivity[i*6*4 + 13] = num_points_filled + 6;
    connectivity[i*6*4 + 14] = num_points_filled + 1;
    connectivity[i*6*4 + 15] = num_points_filled + 2;

    connectivity[i*6*4 + 16] = num_points_filled + 7;
    connectivity[i*6*4 + 17] = num_points_filled + 4;
    connectivity[i*6*4 + 18] = num_points_filled + 5;
    connectivity[i*6*4 + 19] = num_points_filled + 6;

    connectivity[i*6*4 + 20] = num_points_filled + 7;
    connectivity[i*6*4 + 21] = num_points_filled + 2;
    connectivity[i*6*4 + 22] = num_points_filled + 3;
    connectivity[i*6*4 + 23] = num_points_filled + 4;
  }
  leanvtk::VTUWriter writer;
  writer.write_surface_mesh(filename, dimension, cell_size, vertices, connectivity);
}







template <typename T> void prepareVTUMesh(geometry::mesh<T>* m, std::vector<int>* triangulations, std::vector<double>* points) {
  int dimension = 3;
  int cell_size = 3;
  uint num_elements = m->size();

  for (int i = 0; i < num_elements; i++) {
    geometry::tri<T> t = (*m)[i];
    (*points)[i*cell_size*dimension + 0] = (double)t[0][0];
    (*points)[i*cell_size*dimension + 1] = (double)t[0][1];
    (*points)[i*cell_size*dimension + 2] = (double)t[0][2];
    (*points)[i*cell_size*dimension + 3] = (double)t[1][0];
    (*points)[i*cell_size*dimension + 4] = (double)t[1][1];
    (*points)[i*cell_size*dimension + 5] = (double)t[1][2];
    (*points)[i*cell_size*dimension + 6] = (double)t[2][0];
    (*points)[i*cell_size*dimension + 7] = (double)t[2][1];
    (*points)[i*cell_size*dimension + 8] = (double)t[2][2];

    (*triangulations)[i*cell_size + 0] = i*cell_size + 0;
    (*triangulations)[i*cell_size + 1] = i*cell_size + 1;
    (*triangulations)[i*cell_size + 2] = i*cell_size + 2;
  }

}


enum VisualOutputMode { EMITTER, RECEIVER, BOTH };

template <typename T> void writeToFile(results::solution<T>* s, geometry::mesh<T>* e, geometry::mesh<T>* r, const std::string& filename, VisualOutputMode mode) {
  int dimension = 3;
  int cell_size = 3;

  uint num_elements;

  std::string field_name;
  
  if (mode == VisualOutputMode::EMITTER) {
    num_elements = e->size();
    field_name = "Emitter-to-Receiver View Factor";
  } else if (mode == VisualOutputMode::RECEIVER) {
    num_elements = r->size();
    field_name = "Receiver-from-Emitter View Factor";
  } else if (mode == VisualOutputMode::BOTH) {
    num_elements = e->size() + r->size();
    field_name = "BOTH?? View Factor";
  }

  std::vector<int> triangulations(num_elements * cell_size);
  std::vector<double> points(num_elements * cell_size * dimension);

  if (mode == VisualOutputMode::EMITTER) {
    prepareVTUMesh(e, &triangulations, &points);
  } else if (mode == VisualOutputMode::RECEIVER) {
    prepareVTUMesh(r, &triangulations, &points);
  } else if (mode == VisualOutputMode::BOTH) {
    geometry::mesh<T> m = (*e) + r;
    prepareVTUMesh(&m, &triangulations, &points);
  }

  std::vector<double> view_factors(num_elements * cell_size);

  for (int i = 0; i < num_elements; i++) {
    double element_view_factor;

    if (mode == VisualOutputMode::EMITTER) {
      element_view_factor = (double)( results::emitterElementVF(s, i, r->size()) );
    } else if (mode == VisualOutputMode::RECEIVER) {
      std::vector<T> e_areas = geometry::areas(e);
      T e_area = std::reduce(std::execution::par, e_areas.cbegin(), e_areas.cend());
      element_view_factor = (double)( results::receiverElementVF(s, i, e->size(), r->size(), &e_areas, e_area) );
    } else if (mode == VisualOutputMode::BOTH) {
      throw std::runtime_error("No. Just no. I'm not implementing this right now. No need");
    }

    view_factors[i*cell_size + 0] = element_view_factor;
    view_factors[i*cell_size + 1] = element_view_factor;
    view_factors[i*cell_size + 2] = element_view_factor;
  }

  leanvtk::VTUWriter writer;
  writer.add_scalar_field(field_name, view_factors);
  writer.write_surface_mesh(filename, dimension, cell_size, points, triangulations);
}
  
}