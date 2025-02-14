#ifndef MESH_ANALYSIS_CLI_OPTIONS_
#define MESH_ANALYSIS_CLI_OPTIONS_

#include <boost/assign.hpp>
#include <boost/program_options.hpp>

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

#endif