#include "all_headers.hpp"

#include "geometry.hpp"
#include "io.hpp"

namespace po = boost::program_options;

//* -------------------- DEFINE PROGRAM OPTIONS -------------------- *//
po::options_description getOptions() {
  po::options_description options("MeshAnalysis Options",500,250);
  options.add_options()
    ("help,h",
      "MeshAnalysis command-line interface")
    ("input,i",
      po::value<std::string>(),
      "-i <MESH FILEPATH> \n[--+--] Filepath to input mesh (Maximum of 1)")
    ("AspectRatioOutput,a",
      po::value<std::string>()->default_value(std::string("aspect-ratio-output")),
      "-a <GRAPHIC OUTPUT FILEPATH> \n[--+--] Filename for Paraview unstructured grid (.vtu) output of Aspect Ratio (defaults to 'aspect-ratio-output', write 'NONE' to skip)")
    ("ElementQualityOutput,e",
      po::value<std::string>()->default_value(std::string("element-quality-output")),
      "-e <GRAPHIC OUTPUT FILEPATH> \n[--+--] Filename for Paraview unstructured grid (.vtu) output of Element Quality (defaults to 'element-quality-output', write 'NONE' to skip)")
    ("SkewnessOutput,s",
      po::value<std::string>()->default_value(std::string("skewness-output")),
      "-s <GRAPHIC OUTPUT FILEPATH> \n[--+--] Filename for Paraview unstructured grid (.vtu) output of Skewness (defaults to 'skewness-output', write 'NONE' to skip)");
  return options;
}


//* -------------------- PARSE COMMAND LINE -------------------- *//
po::variables_map parseCommandLine(int argc, char *argv[]) {
  po::options_description options = getOptions();
  po::variables_map variables_map;

  //* parse using Boost
  po::store(po::command_line_parser(argc, argv).options(options).run(), variables_map);
  return variables_map;
}




//* -------------------- MESH ANALYSIS -------------------- *//
void meshAnalysis(po::variables_map variables_map) {
  std::string input_filename = variables_map["input"].as<std::string>();

  std::string extension = ".vtu";
  std::string aspect_ratio_output = variables_map["AspectRatioOutput"].as<std::string>() + extension;
  std::cout << "[LOG] Aspect Ratio Output Filename: " << aspect_ratio_output << '\n';
  std::string element_quality_output = variables_map["ElementQualityOutput"].as<std::string>() + extension;
  std::cout << "[LOG] Element Quality Output Filename: " << element_quality_output << '\n';
  std::string skewness_output = variables_map["SkewnessOutput"].as<std::string>() + extension;
  std::cout << "[LOG] Skewness Output Filename: " << skewness_output << '\n';

  std::cout << '\n';

  std::cout << "[LOG] Loading Input Mesh : " << input_filename << '\n';
  geometry::mesh<double> m = geometry::getMesh<double>( input_filename );

  io::printMeshMetrics(&m);

  if (aspect_ratio_output != "NONE.vtu") {
    io::writeMeshMetrics(&m, aspect_ratio_output, io::MetricMode::ASPECT_RATIO);
  }
  if (element_quality_output != "NONE.vtu") {
    io::writeMeshMetrics(&m, element_quality_output, io::MetricMode::ELEMENT_QUALITY);
  }
  if (skewness_output != "NONE.vtu") {
    io::writeMeshMetrics(&m, skewness_output, io::MetricMode::SKEWNESS);
  }
}




int main(int argc, char *argv[]) {
  po::variables_map variables_map = parseCommandLine(argc, argv);
  
  if (variables_map.count("help")) {
    std::cout << getOptions() << '\n';
    return 0;
  }

  po::notify(variables_map);
  meshAnalysis(variables_map);
  return 0;
}