#ifndef COMMANDLINEOPTIONS_HPP_
#define COMMANDLINEOPTIONS_HPP_

#include "Vector3.hpp"
#include "Ray.hpp"
#include "Triangle.hpp"
#include "Triangulation.hpp"
#include "STLReader.hpp"
#include "BVHNode.hpp"
#include "BVH.hpp"
#include "Blockers.hpp"

#include "ViewFactor.hpp"

#include "SolverOptions.hpp"
#include "DoubleAreaIntegration.hpp"
#include "SingleAreaIntegration.hpp"

#include "OutputWriter.hpp"

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

//* map precision enum to output string
static std::map<BackFaceCullMode, std::string> BACKFACECULL_ENUM_TO_OUTPUT =
  boost::assign::map_list_of(
    BackFaceCullMode::ON, "ON")(
    BackFaceCullMode::OFF, "OFF");

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
  std::cout << "[CHECK] Checking Self-Intersection Argument";
  if (!BACKFACECULL_INPUT_TO_ENUM.count(back_face_cull_mode)) {
    throw po::error("\t> [ERROR] Selfint type not recognized: " + back_face_cull_mode);
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
    ("blocking,b",
      po::value<std::vector<std::string>>()->multitoken(),
      "-b <BLOCKER1 FILEPATH> -b <BLOCKER2 FILEPATH> -b <etc.> \n[--+--] Filepath(s) to blocking mesh(es) (Minimum of 0, No Maximum)")
    ("plaintextout,o",
      po::value<std::string>()->default_value(std::string("ovf.out")),
      "-o <PLAINTEXT OUTPUT FILEPATH> \n[--+--] Filepath for nonzero element-wise view factor map output (defaults to 'ovf.out', write 'NONE' to skip)")
    ("graphicout,g",
      po::value<std::vector<std::string>>()->default_value(std::vector<std::string>({std::string("unified_graphic_out")}), "unified_graphic_out")->multitoken(),
      "-g <GRAPHIC OUTPUT FILEPATH> \n[--+--] Filename for Paraview unstructured grid (.vtu) output (defaults to 'unified_graphic_out', write 'NONE' to skip)")
    ("selfint,s",
      po::value<std::string>()->default_value("BOTH")->notifier(&checkSelfIntersectionType),
      "-s <NONE/EMITTER/RECEIVER/BOTH> \n[--+--] Determines which input mesh(es) are included in evaluating obstruction (defaults to BOTH)")
    ("backfacecull,-f",
      po::value<std::string>()->default_value("ON")->notifier(&checkBackFaceCull),
      "-f <ON/OFF> \n[--+--] Determines whether to execute back face culling (defaults to ON)")
    ("numerics,n",
      po::value<std::string>()->default_value("DAI")->notifier(&checkNumerics),
      "-n <DAI/SAI> \n[--+--] Numeric integration method (defaults to DAI)")
    ("compute,c",
      po::value<std::string>()->default_value("CPU_N")->notifier(&checkCompute),
      "-c <CPU/CPU_N/GPU/GPU_N> \n[--+--] Compute backend (defaults to CPU_N)")
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

#endif