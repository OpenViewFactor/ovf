#ifndef OVF_CLI_H_
#define OVF_CLI_H

#include "Vector3.hpp"
#include "Ray.hpp"
#include "Triangle.hpp"
#include "Triangulation.hpp"
#include "STLReader.hpp"
#include "BVHNode.hpp"
#include "BVH.hpp"
#include "Blockers.hpp"

#include "IndexValuePair.hpp"
#include "ViewFactor.hpp"

#include "SolverOptions.hpp"
#include "DoubleAreaIntegration.hpp"
#include "SingleAreaIntegration.hpp"

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


//* -------------------- TIMER CLASS FROM ONLINE -------------------- *//
//* https://www.learncpp.com/cpp-tutorial/timing-your-code/
class Timer
{
private:
	// Type aliases to make accessing nested type easier
	using Clock = std::chrono::steady_clock;
	using Second = std::chrono::duration<double, std::ratio<1> >;

	std::chrono::time_point<Clock> m_beg { Clock::now() };

public:
	void reset()
	{
		m_beg = Clock::now();
	}

	double elapsed() const
	{
		return std::chrono::duration_cast<Second>(Clock::now() - m_beg).count();
	}
};



//* -------------------- MAP SELF-INT INPUTS AND OUTPUTS -------------------- *//
// enum SelfIntersectionMode { NONE, BOTH, EMITTER, RECEIVER };
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
// enum NumericMode { DAI, SAI };
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
// enum ComputeMode { CPU, CPU_N, GPU, GPU_N };
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
// enum PrecisionMode { SINGLE, DOUBLE };
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

//* -------------------- PARSE COMMAND LINE -------------------- *//
template <typename FLOAT_TYPE>
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

  //* ----- process graphical output files ----- *//

  std::vector<std::string> graphic_outfiles = variables_map["graphicout"].as<std::vector<std::string>>();
  std::string emitter_output_filename, receiver_output_filename, unified_output_filename;
  bool one_output_file = true;
  bool write_graphic;
  switch (graphic_outfiles.size()) {
    case 1:
      write_graphic = (graphic_outfiles[0] == "NONE") ? false : true;
      if (write_graphic) {
        unified_output_filename = graphic_outfiles[0] + ".vtu";
        std::cout << "[LOG] Unified Graphic File Output Path : " << unified_output_filename << '\n';
      } else {
        std::cout << "[LOG] NO Graphic File Output Path" << '\n';
      }
      break;
    case 2:
      write_graphic = true;
      emitter_output_filename = graphic_outfiles[0] + ".vtu";
      receiver_output_filename = graphic_outfiles[1] + ".vtu";
      std::cout << "[LOG] Emitter Graphic File Output Path : " << emitter_output_filename << '\n';
      std::cout << "[LOG] Receiver Graphic File Output Path : " << receiver_output_filename << '\n';
      break;
    case 3:
      write_graphic = true;
      emitter_output_filename = graphic_outfiles[0] + ".vtu";
      receiver_output_filename = graphic_outfiles[1] + ".vtu";
      unified_output_filename = graphic_outfiles[2] + ".vtu";
      std::cout << "[LOG] Emitter Graphic File Output Path : " << emitter_output_filename << '\n';
      std::cout << "[LOG] Receiver Graphic File Output Path : " << receiver_output_filename << '\n';
      std::cout << "[LOG] Unified Graphic File Output Path : " << unified_output_filename << '\n';
      break;
  }

  //* ----- load blocking meshes ----- *//
  Blockers<FLOAT_TYPE> blockers;

  bool blocking_enabled = variables_map.count("blocking");
  if (blocking_enabled) {
    std::vector<std::string> blocker_filenames = variables_map["blocking"].as<std::vector<std::string>>();
    blockers.setBlockers(blocker_filenames);
  } else {
    std::cout << "\n[LOG] No Blocking Meshes Loaded" << '\n';
  }

  //* ----- load input meshes ----- *//
  STLReader<FLOAT_TYPE> reader;

  Timer total_program_timer;

  std::vector<std::string> input_filenames = variables_map["inputs"].as<std::vector<std::string>>();
  bool two_mesh_problem = (input_filenames.size() > 1) ? true : false;
  if (input_filenames.size() > 2) { std::cout << "<-----> [NOTIFIER] More than 2 input meshes were provided! Only the first two will be loaded" << '\n'; }

  std::cout << "[LOG] Loading Input Emitter Mesh : " << input_filenames[0] << '\n';
  auto emitter = reader.getMesh(input_filenames[0]);
  std::cout << "[LOG] Emitter Mesh Size: " << emitter->getNumElements() << " Elements" << '\n';
  auto receiver = std::make_shared<Triangulation<FLOAT_TYPE>>();

  auto emitter_bvh = std::make_shared<BVH<FLOAT_TYPE>>();
  auto receiver_bvh = std::make_shared<BVH<FLOAT_TYPE>>();

  if (self_int_type == "BOTH" || self_int_type == "EMITTER") {
    std::cout << "[LOG] Constructing BVH for Emitter Mesh : " << input_filenames[0] << '\n';
    emitter_bvh->linkToTriangulation(emitter);
    emitter_bvh->constructBVH();
    blockers.addBlocker(emitter_bvh);
    std::cout << "[LOG] Construction Finished" << '\n';
  }
  if (two_mesh_problem) {
    std::cout << "[LOG] Loading Input Receiver Mesh : " << input_filenames[1] << '\n';
    receiver = reader.getMesh(input_filenames[1]);
  std::cout << "[LOG] Receiver Mesh Size: " << receiver->getNumElements() << " Elements" << '\n';
    if (self_int_type == "BOTH" || self_int_type == "RECEIVER") {
      std::cout << "[LOG] Constructing BVH for Receiver Mesh : " << input_filenames[1] << '\n';
      receiver_bvh->linkToTriangulation(receiver);
      receiver_bvh->constructBVH();
      blockers.addBlocker(receiver_bvh);
      std::cout << "[LOG] Construction Finished" << '\n';
    }
  }

  std::cout << "[LOG] All Meshes Loaded" << '\n';
  std::cout << "[LOG] Problem Size: " << emitter->getNumElements() * receiver->getNumElements() << " Pairs" << '\n';

  std::cout << "\n[RUN] Computing View Factors" << '\n';

  FLOAT_TYPE back_face_cull_time;
  FLOAT_TYPE blocking_time;
  Timer solver_timer;
  auto results = std::make_shared<ViewFactor<FLOAT_TYPE>>();
  if (numeric == "DAI") {
    DoubleAreaIntegration<FLOAT_TYPE> solver;
    auto unculled_indices = solver.backFaceCullMeshes(emitter, receiver);
    back_face_cull_time = solver_timer.elapsed();
    auto unblocked_indices = solver.evaluateBlockingBetweenMeshes(emitter, receiver, blockers, unculled_indices);
    blocking_time = solver_timer.elapsed() - back_face_cull_time;
    results->linkTriangulations(emitter, receiver, unblocked_indices.size());
    solver.solveViewFactorBetweenMeshes(emitter, receiver, unblocked_indices, results);
  } else if (numeric == "SAI") {
    SingleAreaIntegration<FLOAT_TYPE> solver;
    auto unculled_indices = solver.backFaceCullMeshes(emitter, receiver);
    back_face_cull_time = solver_timer.elapsed();
    auto unblocked_indices = solver.evaluateBlockingBetweenMeshes(emitter, receiver, blockers, unculled_indices);
    blocking_time = solver_timer.elapsed() - back_face_cull_time;
    results->linkTriangulations(emitter, receiver, unblocked_indices.size());
    solver.solveViewFactorBetweenMeshes(emitter, receiver, unblocked_indices, results);
  }

  auto solver_time = solver_timer.elapsed() - back_face_cull_time - blocking_time;


  std::cout << "[LOG] Back Face Cull Completed in " << back_face_cull_time << " [s]" << '\n';
  std::cout << "[LOG] Blocking Completed in " << blocking_time << " [s]" << '\n';
  std::cout << "[LOG] View Factors Completed in " << solver_time << "[s]" << '\n';
  std::cout << "[LOG] Total Solution Time: " << back_face_cull_time + blocking_time + solver_time << " [s]" << '\n';

  std::cout << "\n[RESULT] Surface-Surface View Factor : " << std::setprecision(15) << results->getSurfaceToSurfaceAverageVF() << '\n' << '\n';

  if (write_matrix) { std::cout << "[OUT] Writing Plain Text Matrix Output (not doing anything yet)" << '\n'; }
  if (write_graphic) { std::cout << "[OUT] Writing Graphic File Output (not doing anything yet)" << '\n'; }

  std::cout << "[LOG] Total I/O Time: " << total_program_timer.elapsed() << " [s]" << '\n';
}
#endif