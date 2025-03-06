#ifndef MESH_ANALYSIS_HPP_
#define MESH_ANALYSIS_HPP_

#include "Vector3.hpp"
#include "Triangle.hpp"
#include "Triangulation.hpp"
#include "STLReader.hpp"

#include <boost/assign.hpp>
#include <boost/program_options.hpp>
#include "lean_vtk.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace openviewfactor;
namespace po = boost::program_options;

void writeVTUFile(std::string& filename, std::shared_ptr<Triangulation<double>> mesh, std::string quality_type) {
  int dimension = 3;
  int cell_size = 3;
  auto num_elements = mesh->getNumElements();
  std::vector<int> triangulations(num_elements * cell_size);
  std::iota(triangulations.begin(), triangulations.end(), 0);
  std::vector<double> vertices(num_elements * cell_size * dimension);
  std::vector<double> quality_metric(num_elements * cell_size);

  std::vector<double> element_metric(num_elements, 0.0);

  if (quality_type == "Aspect Ratio") {
    element_metric = mesh->getAllElementsAspectRatios();
  } else if (quality_type == "Element Quality") {
    element_metric = mesh->getAllElementsElementQualities();
  } else if (quality_type == "Skewness") {
    element_metric = mesh->getAllElementsSkewness();
  }

  for (int i = 0; i < num_elements; i++) {
    vertices[9 * i + 0] = (((*mesh)[i]).getOA())[0];
    vertices[9 * i + 1] = (((*mesh)[i]).getOA())[1];
    vertices[9 * i + 2] = (((*mesh)[i]).getOA())[2];

    vertices[9 * i + 3] = (((*mesh)[i]).getOB())[0];
    vertices[9 * i + 4] = (((*mesh)[i]).getOB())[1];
    vertices[9 * i + 5] = (((*mesh)[i]).getOB())[2];

    vertices[9 * i + 6] = (((*mesh)[i]).getOC())[0];
    vertices[9 * i + 7] = (((*mesh)[i]).getOC())[1];
    vertices[9 * i + 8] = (((*mesh)[i]).getOC())[2];

    quality_metric[3 * i + 0] = element_metric[i];
    quality_metric[3 * i + 1] = element_metric[i];
    quality_metric[3 * i + 2] = element_metric[i];
  }

  leanvtk::VTUWriter writer;
  writer.add_scalar_field(quality_type, quality_metric);
  writer.write_surface_mesh(filename, dimension, cell_size, vertices, triangulations);
}

void meshAnalysis(po::variables_map variables_map) {
  std::string input_filename = variables_map["input"].as<std::string>();

  std::string extension = ".vtu";
  std::string aspect_ratio_output = variables_map["AspectRatioOutput"].as<std::string>() + extension;
  std::cout << "[LOG] Aspect Ratio Output Filename: " << aspect_ratio_output << '\n';
  std::string element_quality_output = variables_map["ElementQualityOutput"].as<std::string>() + extension;
  std::cout << "[LOG] Element Quality Output Filename: " << element_quality_output << '\n';
  std::string skewness_output = variables_map["SkewnessOutput"].as<std::string>() + extension;
  std::cout << "[LOG] Skewness Output Filename: " << skewness_output << '\n';

  STLReader<double> reader;

  std::cout << "[LOG] Loading Input Mesh : " << input_filename << '\n';
  auto mesh = reader.getMesh(input_filename);

  std::cout << " -----------------------------------------------------------------" << '\n';
  std::cout << " [Size]\t\t\t\t\t" << mesh->getNumElements() << " Elements" << '\n';
  std::cout << " -----------------------------------------------------------------" << '\n';
  std::cout << " [Aspect Ratio]\t\tMean\t\t" << mesh->evaluateAspectRatioMean() << '\n';
  std::cout << " [Aspect Ratio]\t\tMin/Max\t\t" << (mesh->evaluateAspectRatioMinMax()).first << " / " << (mesh->evaluateAspectRatioMinMax()).second << '\n';
  writeVTUFile(aspect_ratio_output, mesh, "Aspect Ratio");
  std::cout << " -----------------------------------------------------------------" << '\n';
  std::cout << " [Element Quality]\tMean\t\t" << mesh->evaluateElementQualityMean() << '\n';
  std::cout << " [Element Quality]\tMin/Max\t\t" << (mesh->evaluateElementQualityMinMax()).first << " / " << (mesh->evaluateElementQualityMinMax()).second << '\n';
  writeVTUFile(element_quality_output, mesh, "Element Quality");
  std::cout << " -----------------------------------------------------------------" << '\n';
  std::cout << " [Skewness]\t\tMean\t\t" << mesh->evaluateSkewnessMean() << '\n';
  std::cout << " [Skewness]\t\tMin/Max\t\t" << (mesh->evaluateSkewnessMinMax()).first << " / " << (mesh->evaluateSkewnessMinMax()).second << '\n';
  writeVTUFile(skewness_output, mesh, "Skewness");
  std::cout << " -----------------------------------------------------------------" << '\n';
}

#endif