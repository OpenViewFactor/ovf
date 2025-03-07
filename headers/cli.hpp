#include "all_headers.hpp"

#pragma once

//! ----- COMMAND-LINE INTERACE ----- !//

const std::string OVF_VERSION_STRING = "0.3";

namespace cli {

namespace po = boost::program_options;

enum SelfIntersectionMode { NONE, BOTH, EMITTER, RECEIVER };
enum BackFaceCullMode { ON, OFF };
enum BlockingMode { NAIVE, BVH };
enum NumericMode { DAI, SAI };
enum ComputeMode { CPU_N, GPU, GPU_N };
enum PrecisionMode { SINGLE, DOUBLE };

//* -------------------- MAP SELF-INT INPUTS AND OUTPUTS -------------------- *//
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

//* -------------------- MAP BACK-FACE CULL INPUTS AND OUTPUTS -------------------- *//
//* map back face cull input string to enum
static std::map<std::string, BackFaceCullMode> BACKFACECULL_INPUT_TO_ENUM =
boost::assign::map_list_of(
  "ON", BackFaceCullMode::ON)(
  "OFF", BackFaceCullMode::OFF);

//* map back face cull enum to output string
static std::map<BackFaceCullMode, std::string> BACKFACECULL_ENUM_TO_OUTPUT =
boost::assign::map_list_of(
  BackFaceCullMode::ON, "ON")(
  BackFaceCullMode::OFF, "OFF");

//* -------------------- MAP BLOCKING TYPE INPUTS AND OUTPUTS -------------------- *//
//* map blocking type input string to enum
static std::map<std::string, BlockingMode> BLOCKING_TYPE_INPUT_TO_ENUM =
boost::assign::map_list_of(
  "NAIVE", BlockingMode::NAIVE)(
  "BVH", BlockingMode::BVH);

//* map blocking type enum to output string
static std::map<BlockingMode, std::string> BLOCKING_TYPE_ENUM_TO_OUTPUT =
boost::assign::map_list_of(
  BlockingMode::NAIVE, "NAIVE")(
    BlockingMode::BVH, "BVH");

//* -------------------- MAP NUMERICS INPUTS AND OUTPUTS -------------------- *//
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
//* map compute input string to enum
static std::map<std::string, ComputeMode> COMPUTE_INPUT_TO_ENUM =
boost::assign::map_list_of(
  "CPU_N", ComputeMode::CPU_N)(
  "GPU", ComputeMode::GPU)(
  "GPU_N", ComputeMode::GPU_N);

//* map compute enum to output string
static std::map<ComputeMode, std::string> COMPUTE_ENUM_TO_OUTPUT =
boost::assign::map_list_of(
  ComputeMode::CPU_N, "CPU_N")(
  ComputeMode::GPU, "GPU")(
  ComputeMode::GPU_N, "GPU_N");

//* -------------------- MAP PRECISION INPUTS AND OUTPUTS -------------------- *//
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
  std::cout << "[CHECK] Checking Self-Intersection Argument";
  if (!SELFINT_TYPE_INPUT_TO_ENUM.count(self_int_type)) {
    throw po::error("\t> [ERROR] Selfint type not recognized: " + self_int_type);
  }
  std::cout << "\t> [VALID]" << '\n';
}

void checkBackFaceCull(const std::string &back_face_cull_mode) {
  std::cout << "[CHECK] Checking Back Face Cull Argument";
  if (!BACKFACECULL_INPUT_TO_ENUM.count(back_face_cull_mode)) {
    throw po::error("\t> [ERROR] Back Face Cull option not recognized: " + back_face_cull_mode);
  }
  std::cout << "\t> [VALID]" << '\n';
}

void checkBlockingType(const std::string &blocking_type) {
  std::cout << "[CHECK] Checking Blocking Type Argument";
  if (!BLOCKING_TYPE_INPUT_TO_ENUM.count(blocking_type)) {
    throw po::error("\t> [ERROR] Blocking Type option not recognized: " + blocking_type);
  }
  std::cout << "\t> [VALID]" << '\n';
}

void checkNumerics(const std::string &numerics) {
  std::cout << "[CHECK] Checking Numeric Method Argument";
  if (!NUMERICS_INPUT_TO_ENUM.count(numerics)) {
    throw po::error("\t> [ERROR] Numeric method not recognized: " + numerics);
  }
  std::cout << "\t> [VALID]" << '\n';
}

void checkCompute(const std::string &compute) {
  std::cout << "[CHECK] Checking Compute Backend Argument";
  if (!COMPUTE_INPUT_TO_ENUM.count(compute)) {
    throw po::error("\t> [ERROR] Compute type not recognized: " + compute);
  }
  std::cout << "\t> [VALID]" << '\n';
}

void checkPrecision(const std::string &precision) {
  std::cout << "[CHECK] Checking Precision Argument";
  if (!PRECISION_INPUT_TO_ENUM.count(precision)) {
    throw po::error("\t\t> [ERROR] Precision type not recognized: " + precision);
  }
  std::cout << "\t\t> [VALID]" << '\n';
}

//* -------------------- DEFINE PROGRAM OPTIONS -------------------- *//
po::options_description getOptions() {
po::options_description options("OpenViewFactor Options",500,250);
options.add_options()
  ("help,h",
    "OpenViewFactor command-line interface")
  ("version,v",
    "OpenViewFactor version")
  ("inputs,i",
    po::value<std::vector<std::string>>()->multitoken(),
    "-i <EMITTER FILEPATH> <RECEIVER FILEPATH> \n[--+--] Filepaths to input meshes (Minimum of 1, Maximum of 2)")
  ("obstructions,o",
    po::value<std::vector<std::string>>()->multitoken(),
    "-o <OBSTRUCTOR FILEPATH> -o <OBSTRUCTOR FILEPATH> -o <etc.> \n[--+--] Filepath(s) to obstructing mesh(es) (Minimum of 0, No Maximum)")
  ("matrixout,m",
    po::value<std::string>()->default_value(std::string("ovf.out")),
    "-m <MATRIX OUTPUT FILEPATH> \n[--+--] Filepath for nonzero element-wise view factor map output (defaults to 'ovf.out', write 'NONE' to skip)")
  ("graphicout,g",
    po::value<std::vector<std::string>>()->default_value(std::vector<std::string>({std::string("emitter_out")}), "emitter_out")->multitoken(),
    "-g <GRAPHIC OUTPUT FILEPATH> \n[--+--] Filename for Paraview unstructured grid (.vtu) output (defaults to 'emitter_out', write 'NONE' to skip)")
  ("bvhout,b",
    po::value<std::string>()->default_value(std::string(std::string("NONE"))),
    "-b <BLOCKER BVH OUTPUT FILEPATH> \n[--+--] Filename for Paraview unstructured grid (.vtu) output of the blocker BVH structure (skips by default)")
  ("selfint,s",
    po::value<std::string>()->default_value("BOTH")->notifier(&checkSelfIntersectionType),
    "-s <NONE/EMITTER/RECEIVER/BOTH> \n[--+--] Determines which input mesh(es) are included in evaluating obstruction (defaults to BOTH)")
  ("backfacecull,f",
    po::value<std::string>()->default_value("ON")->notifier(&checkBackFaceCull),
    "-f <ON/OFF> \n[--+--] Determines whether to execute back face culling (defaults to ON)")
  ("blockingtype,t",
    po::value<std::string>()->default_value("BVH")->notifier(&checkBlockingType),
    "-t <BVH/NAIVE> \n[--+--] Determines which type of blocking to utilize (defaults to BVH)")
  ("numerics,n",
    po::value<std::string>()->default_value("DAI")->notifier(&checkNumerics),
    "-n <DAI/SAI> \n[--+--] Numeric integration method (defaults to DAI)")
  ("compute,c",
    po::value<std::string>()->default_value("CPU_N")->notifier(&checkCompute),
    "-c <CPU_N/GPU/GPU_N> \n[--+--] Compute backend (defaults to CPU_N)")
  ("precision,p",
    po::value<std::string>()->default_value("SINGLE")->notifier(&checkPrecision),
    "-p <SINGLE/DOUBLE> \n[--+--] Floating point precision (defaults to SINGLE)");
return options;
}

//* -------------------- DEFINE POSITIONAL OPTIONS -------------------- *//
po::positional_options_description getPositionalOptions() {
po::positional_options_description positional_options;
positional_options.add("inputs", 2);
positional_options.add("graphicout", 3);
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

}