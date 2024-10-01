#ifndef OVF_CLI_H_
#define OVF_CLI_H

#include "STLReader.hpp"
#include "Vector3.hpp"
#include "Ray.hpp"
#include "Triangle.hpp"
#include "Triangulation.hpp"
#include "BVHNode.hpp"
#include "BVH.hpp"

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

//* -------------------- MAP SELF-INT INPUTS AND OUTPUTS -------------------- *//
enum SelfIntersectionMode { NONE, BOTH, EMITTER, RECEIVER };
//* map self-intersection type input string to enum
static std::map<std::string, SelfIntersectionMode> SELFINT_TYPE_INPUT_TO_ENUM =
  boost::assign::map_list_of(
    "NONE", SelfIntersectionMode::NONE)(
    "BOTH", SelfIntersectionMode::BOTH)(
    "EMITTER", SelfIntersectionMode::EMITTER)(
    "RECEIVER", SelfIntersectionMode::RECEIVER);

//* map self-intersection type enum to output string
static std::map<SelfIntersectionMode, std::string> SELFINT_TYPE_ENUM_TO_OUTPUT =
  boost::assign::map_list_of(
    SelfIntersectionMode::NONE, "NONE")(
    SelfIntersectionMode::BOTH, "BOTH")(
    SelfIntersectionMode::EMITTER, "EMITTER")(
    SelfIntersectionMode::RECEIVER, "RECEIVER");

//* -------------------- MAP NUMERICS INPUTS AND OUTPUTS -------------------- *//
enum NumericMode { DAI, SAI };
//* map numerics input string to enum
static std::map<std::string, NumericMode> NUMERICS_INPUT_TO_ENUM =
  boost::assign::map_list_of(
    "DAI", NumericMode::DAI)(
    "SAI", NumericMode::SAI);

//* map precision enum to output string
static std::map<NumericMode, std::string> NUMERICS_ENUM_TO_OUTPUT =
  boost::assign::map_list_of(
    NumericMode::DAI, "DAI")(
    NumericMode::SAI, "SAI");

//* -------------------- MAP COMPUTE INPUTS AND OUTPUTS -------------------- *//
enum ComputeMode { CPU, CPU_N, GPU, GPU_N };
//* map compute input string to enum
static std::map<std::string, ComputeMode> COMPUTE_INPUT_TO_ENUM =
  boost::assign::map_list_of(
    "CPU", ComputeMode::CPU)(
    "CPU_N", ComputeMode::CPU_N)(
    "GPU", ComputeMode::GPU)(
    "GPU_N", ComputeMode::GPU_N);

//* map compute enum to output string
static std::map<ComputeMode, std::string> COMPUTE_ENUM_TO_OUTPUT =
  boost::assign::map_list_of(
    ComputeMode::CPU, "CPU")(
    ComputeMode::CPU_N, "CPU_N")(
    ComputeMode::GPU, "GPU")(
    ComputeMode::GPU_N, "GPU_N");

//* -------------------- MAP PRECISION INPUTS AND OUTPUTS -------------------- *//
enum PrecisionMode { SINGLE, DOUBLE };
//* map precision input string to enum
static std::map<std::string, PrecisionMode> PRECISION_INPUT_TO_ENUM =
  boost::assign::map_list_of(
    "SINGLE", PrecisionMode::SINGLE)(
    "DOUBLE", PrecisionMode::DOUBLE);

//* map precision enum to output string
static std::map<PrecisionMode, std::string> PRECISION_ENUM_TO_OUTPUT =
  boost::assign::map_list_of(
    PrecisionMode::SINGLE, "SINGLE")(
    PrecisionMode::DOUBLE, "DOUBLE");

//* -------------------- NOTIFIERS -------------------- *//
void checkSelfIntersectionType(const std::string &self_int_type) {
  std::cout << "<-----> [CHECK] Checking Self-Intersection Argument" << '\n';
  if (!SELFINT_TYPE_INPUT_TO_ENUM.count(self_int_type)) {
    throw po::error("[ERROR] Selfint type not recognized: " + self_int_type);
  }
  std::cout << "<-----> [VALID] Valid Self-Intersection Argument" << '\n';
}

void checkNumerics(const std::string &numerics) {
  std::cout << "<-----> [CHECK] Checking Numeric Method Argument" << '\n';
  if (!NUMERICS_INPUT_TO_ENUM.count(numerics)) {
    throw po::error("[ERROR] Numeric method not recognized: " + numerics);
  }
  std::cout << "<-----> [VALID] Valid Numeric Method Argument" << '\n';
}

void checkCompute(const std::string &compute) {
  std::cout << "<-----> [CHECK] Checking Compute Backend Argument" << '\n';
  if (!COMPUTE_INPUT_TO_ENUM.count(compute)) {
    throw po::error("[ERROR] Compute type not recognized: " + compute);
  }
  std::cout << "<-----> [VALID] Valid Compute Backend Argument" << '\n';
}

void checkPrecision(const std::string &precision) {
  std::cout << "<-----> [CHECK] Checking Precision Argument" << '\n';
  if (!PRECISION_INPUT_TO_ENUM.count(precision)) {
    throw po::error("[ERROR] Precision type not recognized: " + precision);
  }
  std::cout << "<-----> [VALID] Valid Precision Argument" << '\n';
}

//* -------------------- DEFINE PROGRAM OPTIONS -------------------- *//
po::options_description getOptions() {
  po::options_description options("OpenViewFactor options",300,250);
  options.add_options()
    ("help,h", "\tOpenViewFactor command-line interface\n")
    ("version,v", "\tOpenViewFactor version\n")
    ("inputs,i", po::value<std::vector<std::string>>()->required(), "-i <EMITTER FILEPATH> <RECEIVER FILEPATH> \n    Filepaths to input meshes (Minimum of 1, Maximum of 2)\n")
    ("blocking,b", po::value<std::vector<std::string>>(), "-b <BLOCKER1 FILEPATH> -b <BLOCKER2 FILEPATH> -b <etc.> \n    Filepath(s) to blocking mesh(es) (Minimum of 0, No Maximum)\n")
    ("plaintextout,o", po::value<std::string>(), "-o <PLAINTEXT OUTPUT FILEPATH> \n    Filepath for nonzero element-wise view factor map output (defaults to 'ovf.out', write 'NONE' to skip)\n")
    ("graphicout,g", po::value<std::string>(), "-g <GRAPHIC OUTPUT FILEPATH> \n    Filepath for Paraview unstructured grid (.vtu) output (defaults to 'emitter.vtu' & 'receiver.vtu', write 'NONE' to skip)\n")
    ("selfint,s", po::value<std::string>()->default_value("BOTH")->notifier(&checkSelfIntersectionType), "-s <NONE/EMITTER/RECEIVER/BOTH> \n    Determines which input mesh(es) are included in evaluating obstruction (defaults to BOTH)\n")
    ("numerics,n", po::value<std::string>()->default_value("DAI")->notifier(&checkNumerics), "-n <DAI/SAI> \n    Numeric integration method (defaults to DAI)\n")
    ("compute,c", po::value<std::string>()->default_value("CPU_N")->notifier(&checkCompute), "-c <CPU/CPU_N/GPU/GPU_N \n    Compute backend (defaults to CPU_N)\n")
    ("precision,p", po::value<std::string>()->default_value("SINGLE")->notifier(&checkPrecision), "-p <SINGLE/DOUBLE> \n    Floating point precision (defaults to SINGLE)\n");
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
//TODO this is templatized in original openviewfactor and i think that's weird
void ovfWorkflow(po::variables_map variables_map) {
  std::cout << "\n[VERSION] OpenViewFactor Version " << OVF_VERSION_STRING << '\n' << '\n';

  //* ----- load solver settings ----- *//

  std::string numeric = variables_map["numerics"].as<std::string>();
  std::cout << "[LOG] Solver Setting Loaded: Numeric Method - " << numeric << '\n';

  std::string compute = variables_map["compute"].as<std::string>();
  std::cout << "[LOG] Solver Setting Loaded: Compute Backend - " << compute << '\n';

  std::string self_int_type = variables_map["selfint"].as<std::string>();
  std::cout << "[LOG] Solver Setting Loaded: Self-Intersection Mode - " << self_int_type << '\n';

  std::string precision = variables_map["precision"].as<std::string>();
  std::cout << "[LOG] Solver Setting Loaded: Floating Point Precision - " << precision << '\n';

  std::string matrix_outfile = variables_map["plaintextout"].as<std::string>();
  std::cout << "[LOG] Plain Text Matrix Output Path : " << matrix_outfile << '\n';
  bool write_matrix = (matrix_outfile == "NONE") ? false : true;

  std::string graphic_outfile = variables_map["graphicout"].as<std::string>();
  std::cout << "[LOG] Graphic File Output Path : " << graphic_outfile << '\n';
  bool write_graphic = (graphic_outfile == "NONE") ? false : true;

  //* ----- load input meshes ----- *//

  std::vector<std::string> input_filenames = variables_map["inputs"].as<std::vector<std::string>>();
  bool two_mesh_problem = (input_filenames.size() > 1) ? true : false;
  if (input_filenames.size() > 2) { std::cout << "<-----> [NOTIFIER] More than 2 input meshes were provided! Only the first two will be loaded" << '\n'; }
  std::cout << "[LOG] Input Emitter Mesh Loaded : " << input_filenames[0] << '\n';
  if (two_mesh_problem) { std::cout << "[LOG] Input Receiver Mesh Loaded : " << input_filenames[1] << '\n'; }

  //* ----- load blocking meshes ----- *//

  bool blocking_enabled = variables_map.count("blocking");
  if (blocking_enabled) {
    std::vector<std::string> blocker_filenames = variables_map["blocking"].as<std::vector<std::string>>();
    for (auto blocker : blocker_filenames) {
      std::cout << "[LOG] Blocking Mesh Loaded : " << blocker << '\n';
    }
  } else {
    std::cout << "[LOG] No Blocking Meshes Loaded" << '\n';
  }

  std::cout << "[LOG] All Meshes Loaded" << '\n';

  std::cout << "\n[RUN] Computing View Factors" << '\n';

  std::cout << "[LOG] View Factors Completed" << '\n';

  std::cout << "\n[RESULT] Surface-Surface View Factor : " << 3.1415 << '\n' << '\n';

  if (write_matrix) { std::cout << "[OUT] Writing Plain Text Matrix Output" << '\n'; }
  if (write_graphic) { std::cout << "[OUT] Writing Graphic File Output" << '\n'; }
}
#endif