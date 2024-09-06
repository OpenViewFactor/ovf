#ifndef OVF_CLI_H_
#define OVF_CLI_H

#include "../geometry/headers/STLReader.hpp"
#include "../geometry/headers/Vector3.hpp"
#include "../geometry/headers/Ray.hpp"
#include "../geometry/headers/Triangle.hpp"
#include "../geometry/headers/Triangulation.hpp"
#include "../geometry/headers/BVHNode.hpp"
#include "../geometry/headers/BVH.hpp"

#include <boost/assign.hpp>
#include <boost/program_options.hpp>
#include <chrono>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <numeric>
#include <exception>
#include <iomanip>
#include <typeinfo>
#include <limits>

#include <memory>
#include <map>
#include <set>
#include <type_traits>

using namespace openviewfactor;
namespace po = boost::program_options;

const std::string OVF_VERSION_STRING = "0.2";

//TODO LIST:
//TODO [1] Define SelfIntersectionMode
//TODO [2] Define PrecisionMode
//TODO [3] Define ComputeMode
//TODO [4] Define NumericMode

// //* -------------------- MAP SELF-INT INPUTS AND OUTPUTS -------------------- *//
// enum SelfIntersectionMode { NONE, BOTH, EMITTER, RECEIVER };
// //* map self-intersection type input string to enum
// static std::map<std::string, SelfIntersectionMode> SELFINT_TYPE_INPUT_TO_ENUM =
//   boost::assign::map_list_of(
//     "NONE", SelfIntersectionMode::NONE)(
//     "BOTH", SelfIntersectionMode::BOTH)(
//     "EMITTER", SelfIntersectionMode::EMITTER)(
//     "RECEIVER", SelfIntersectionMode::RECEIVER);

// //* map self-intersection type enum to output string
// static std::map<std::string, SelfIntersectionMode> SELFINT_TYPE_ENUM_TO_OUTPUT =
//   boost::assign::map_list_of(
//     SelfIntersectionMode::NONE, "NONE")(
//     SelfIntersectionMode::BOTH, "BOTH")(
//     SelfIntersectionMode::EMITTER, "EMITTER")(
//     SelfIntersectionMode::RECEIVER, "RECEIVER");

// //* -------------------- MAP NUMERICS INPUTS AND OUTPUTS -------------------- *//
// enum NumericMode { DAI, SAI };
// //* map numerics input string to enum
// static std::map<std::string, NumericMode> NUMERICS_INPUT_TO_ENUM =
//   boost::assign::map_list_of(
//     "DAI", NumericMode::DAI)(
//     "SAI", NumericMode::SAI);

// //* map precision enum to output string
// static std::map<std::string, NumericMode> NUMERICS_ENUM_TO_OUTPUT =
//   boost::assign::map_list_of(
//     NumericMode::DAI, "DAI")(
//     NumericMode::SAI, "SAI");

// //* -------------------- MAP COMPUTE INPUTS AND OUTPUTS -------------------- *//
// enum ComputeMode { CPU, CPU_N, GPU, GPU_N };
// //* map compute input string to enum
// static std::map<std::string, ComputeMode> COMPUTE_INPUT_TO_ENUM =
//   boost::assign::map_list_of(
//     "CPU", ComputeMode::CPU)(
//     "CPU_N", ComputeMode::CPU_N)(
//     "GPU", ComputeMode::GPU)(
//     "GPU_N", ComputeMode::GPU_N);

// //* map compute enum to output string
// static std::map<std::string, ComputeMode> COMPUTE_ENUM_TO_OUTPUT =
//   boost::assign::map_list_of(
//     ComputeMode::CPU, "CPU")(
//     ComputeMode::CPU_N, "CPU_N")(
//     ComputeMode::GPU, "GPU")(
//     ComputeMode::GPU_N, "GPU_N");

// //* -------------------- MAP PRECISION INPUTS AND OUTPUTS -------------------- *//
// enum PrecisionMode { SINGLE, DOUBLE };
// //* map precision input string to enum
// static std::map<std::string, PrecisionMode> PRECISION_INPUT_TO_ENUM =
//   boost::assign::map_list_of(
//     "SINGLE", PrecisionMode::SINGLE)(
//     "DOUBLE", PrecisionMode::DOUBLE);

// //* map precision enum to output string
// static std::map<std::string, PrecisionMode> PRECISION_ENUM_TO_OUTPUT =
//   boost::assign::map_list_of(
//     PrecisionMode::SINGLE, "SINGLE")(
//     PrecisionMode::DOUBLE, "DOUBLE");

//* -------------------- NOTIFIERS -------------------- *//
void checkSelfIntersectionType(const std::string &self_int_type) {
  // if (!SELFINT_TYPE_INPUT_TO_ENUM.count(self_int_type)) {
  //   throw po::error("--- [OVF INPUT ERROR] Selfint type not recognized: " + self_int_type);
  // }
  std::cout << "[NOTIFIER] Checking Self-Intersection" << '\n';
}

void checkNumerics(const std::string &numerics) {
  // if (!NUMERICS_INPUT_TO_ENUM.count(numerics)) {
  //   throw po::error("--- [OVF INPUT ERROR] Numeric method not recognized: " + numerics);
  // }
  std::cout << "[NOTIFIER] Checking Numerics" << '\n';
}

void checkCompute(const std::string &compute) {
  // if (!COMPUTE_INPUT_TO_ENUM.count(compute)) {
  //   throw po::error("--- [OVF INPUT ERROR] Compute type not recognized: " + compute);
  // }
  std::cout << "[NOTIFIER] Checking Compute" << '\n';
}

void checkPrecision(const std::string &precision) {
  // if (!PRECISION_INPUT_TO_ENUM.count(precision)) {
  //   throw po::error("--- [OVF INPUT ERROR] Precision type not recognized: " + precision);
  // }
  std::cout << "[NOTIFIER] Checking Precision" << '\n';
}

//* -------------------- DEFINE PROGRAM OPTIONS -------------------- *//
po::options_description getOptions() {
  po::options_description options("OpenViewFactor options",300,250);
  options.add_options()
    ("help,h", "OpenViewFactor Help printout")
    ("version,v", "OpenViewFactor Version")
    ("inputs,i", po::value<std::vector<std::string>>()->required(), "Input Mesh Filepaths (Minimum of 1, Maximum of 2): -i <EMITTER FILEPATH> <RECEIVER FILEPATH>")
    ("blocking,b", "Blocking Mesh(es) Filepath(s) (Minimum of 0, Maximum of 10): -b <BLOCKER1 FILEPATH> <BLOCKER2 FILEPATH> <etc.>")
    ("plaintextout,o", "Output Matrix Filepath (defaults to 'ovf.out', write 'NONE' to skip): -o <PLAINTEXT OUTPUT FILEPATH>")
    ("graphicout,g", "Output .vtu Filepath (defaults to 'emitter.vtu' & 'receiver.vtu', write 'NONE' to skip): -g <GRAPHIC OUTPUT FILEPATH>")
    ("selfint,s", po::value<std::string>()->default_value("BOTH")->notifier(&checkSelfIntersectionType), "Self-Intersection Mode for which Input Meshes to Include in Obstruction Evaluation (defaults to BOTH): -s <NONE/EMITTER/RECEIVER/BOTH>")
    ("numerics,n", po::value<std::string>()->default_value("DAI")->notifier(&checkNumerics), "Numeric Method (defaults to DAI): -n <DAI/SAI>")
    ("compute,c", po::value<std::string>()->default_value("CPU_N")->notifier(&checkCompute), "Compute Backend (defaults to CPU_N): -c <CPU/CPU_N/GPU/GPU_N")
    ("precision,p", po::value<std::string>()->default_value("SINGLE")->notifier(&checkPrecision), "Floating Point Precision (defaults to SINGLE): -p <SINGLE/DOUBLE>");
  return options;
}

//* -------------------- DEFINE POSITIONAL OPTIONS -------------------- *//
po::positional_options_description getPositionalOptions() {
  po::positional_options_description positional_options;
  positional_options.add("inputs", 2);
  return positional_options;
}

//* -------------------- PARSE COMMAND LINE -------------------- *//
po::variables_map parseCommandLine(int argc, char *argv[]) {
  po::options_description options = getOptions();
  po::positional_options_description positional_options = getPositionalOptions();
  po::variables_map variables_map;

  //* parse using Boost
  po::store(po::command_line_parser(argc, argv).options(options).positional(positional_options).run(), variables_map);
  return variables_map;
}

//* -------------------- PARSE COMMAND LINE -------------------- *//
//TODO this is templatized and i think that's weird
void ovfWorkflow(po::variables_map variables_map) {
  std::cout << "[OVF START] OpenViewFactor Version " << OVF_VERSION_STRING << '\n';

  //* ----- load solver settings ----- *//

  std::string numeric = variables_map["numerics"].as<std::string>();
  std::cout << "[OVF LOG] Solver Setting Loaded: Numeric Method - " << numeric << '\n';

  std::string compute = variables_map["compute"].as<std::string>();
  std::cout << "[OVF LOG] Solver Setting Loaded: Compute Backend - " << compute << '\n';

  std::string self_int_type = variables_map["selfint"].as<std::string>();
  std::cout << "[OVF LOG] Solver Setting Loaded: Self-Intersection Mode - " << self_int_type << '\n';

  std::string precision = variables_map["precision"].as<std::string>();
  std::cout << "[OVF LOG] Solver Setting Loaded: Floating Point Precision - " << precision << '\n';

  std::string matrix_outfile = variables_map["plaintextout"].as<std::string>();
  std::cout << "[OVF LOG] Plain Text Matrix Output Path : " << matrix_outfile << '\n';
  bool write_matrix = (matrix_outfile == "NONE") ? false : true;

  std::string graphic_outfile = variables_map["graphicout"].as<std::string>();
  std::cout << "[OVF LOG] Graphic File Output Path : " << graphic_outfile << '\n';
  bool write_graphic = (graphic_outfile == "NONE") ? false : true;

  //* ----- load input meshes ----- *//

  std::vector<std::string> input_filenames = variables_map["inputs"].as<std::vector<std::string>>();
  bool two_mesh_problem = (input_filenames.size() == 2) ? true : false;
  std::cout << "[OVF LOG] Input Emitter Mesh Loaded : " << input_filenames[0] << '\n';
  if (two_mesh_problem) { std::cout << "[OVF LOG] Input Receiver Mesh Loaded : " << input_filenames[1] << '\n'; }

  //* ----- load blocking meshes ----- *//

  bool blocking_enabled = variables_map.count("blocking");
  if (blocking_enabled) {
    std::vector<std::string> blocker_filenames = variables_map["blocking"].as<std::vector<std::string>>();
    // std::vector<std::string> blocker_filenames;
    // std::string blocker_input = variables_map["blocking"].as<std::string>();
    // size_t space_location = 0;
    // while (space_location != std::numeric_limits<size_t>::max()) {
    //   space_location = blocker_input.find(" ");
    //   blocker_filenames.push_back(blocker_input.substr(0,space_location));
    //   blocker_input = blocker_input.substr(space_location + 1, blocker_input.size() - (space_location + 1));
    // }
    for (auto blocker : blocker_filenames) {
      std::cout << "[OVF LOG] Blocking Mesh Loaded : " << blocker << '\n';
    }
  } else {
    std::cout << "[OVF LOG] No Blocking Meshes Loaded" << '\n';
  }

  std::cout << "[OVF LOG] All Meshes Loaded" << '\n';

  std::cout << "[OVF RUN] Computing View Factors" << '\n';

  std::cout << "[OVF LOG] View Factors Completed" << '\n';

  std::cout << "[OVF RESULT] Surface-Surface View Factor : " << 3.1415 << '\n';

  if (write_matrix) { std::cout << "[OVF LOG] Writing Plain Text Matrix Output" << '\n'; }
  if (write_graphic) { std::cout << "[OVF LOG] Writing Graphic File Output" << '\n'; }
}
#endif