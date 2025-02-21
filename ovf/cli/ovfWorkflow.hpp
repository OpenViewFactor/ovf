#ifndef OVF_CLI_H_
#define OVF_CLI_H

#include "CommandLineOptions.hpp"

const std::string OVF_VERSION_STRING = "0.2";

using namespace openviewfactor;
namespace po = boost::program_options;

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

  std::string back_face_cull_mode = variables_map["backfacecull"].as<std::string>();
  std::cout << "[LOG] Solver Setting Loaded: Back Face Cull Mode - " << back_face_cull_mode << '\n';

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
  int num_graphic_outfiles = 0;
  switch (graphic_outfiles.size()) {
    case 1:
      write_graphic = (graphic_outfiles[0] == "NONE") ? false : true;
      if (write_graphic) {
        num_graphic_outfiles = 1;
        emitter_output_filename = graphic_outfiles[0] + ".vtu";
        std::cout << "[LOG] Emitter Graphic File Output Path : " << emitter_output_filename << '\n';
      } else {
        std::cout << "[LOG] NO Graphic File Output Path" << '\n';
      }
      break;
    case 2:
      write_graphic = true;
      num_graphic_outfiles = 2;
      emitter_output_filename = graphic_outfiles[0] + ".vtu";
      receiver_output_filename = graphic_outfiles[1] + ".vtu";
      std::cout << "[LOG] Emitter Graphic File Output Path : " << emitter_output_filename << '\n';
      std::cout << "[LOG] Receiver Graphic File Output Path : " << receiver_output_filename << '\n';
      break;
    case 3:
      write_graphic = true;
      num_graphic_outfiles = 3;
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
  std::cout << " -----------------------------------------------------------------" << '\n';
    std::cout << " [Size]\t\t\t\t\t" << emitter->getNumElements() << " Elements" << '\n';
    std::cout << " [Aspect Ratio]\t\tMean\t\t" << emitter->evaluateAspectRatioMean() << '\n';
    std::cout << " [Aspect Ratio]\t\tMin/Max\t\t" << (emitter->evaluateAspectRatioMinMax()).first << " / " << (emitter->evaluateAspectRatioMinMax()).second << '\n';
    std::cout << " [Element Quality]\tMean\t\t" << emitter->evaluateElementQualityMean() << '\n';
    std::cout << " [Element Quality]\tMin/Max\t\t" << (emitter->evaluateElementQualityMinMax()).first << " / " << (emitter->evaluateElementQualityMinMax()).second << '\n';
    std::cout << " [Skewness]\t\tMean\t\t" << emitter->evaluateSkewnessMean() << '\n';
    std::cout << " [Skewness]\t\tMin/Max\t\t" << (emitter->evaluateSkewnessMinMax()).first << " / " << (emitter->evaluateSkewnessMinMax()).second << '\n';
    std::cout << " -----------------------------------------------------------------" << '\n';

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
    std::cout << " -----------------------------------------------------------------" << '\n';
    std::cout << " [Size]\t\t\t\t\t" << receiver->getNumElements() << " Elements" << '\n';
    std::cout << " [Aspect Ratio]\t\tMean\t\t" << receiver->evaluateAspectRatioMean() << '\n';
    std::cout << " [Aspect Ratio]\t\tMin/Max\t\t" << (receiver->evaluateAspectRatioMinMax()).first << " / " << (receiver->evaluateAspectRatioMinMax()).second << '\n';
    std::cout << " [Element Quality]\tMean\t\t" << receiver->evaluateElementQualityMean() << '\n';
    std::cout << " [Element Quality]\tMin/Max\t\t" << (receiver->evaluateElementQualityMinMax()).first << " / " << (receiver->evaluateElementQualityMinMax()).second << '\n';
    std::cout << " [Skewness]\t\tMean\t\t" << receiver->evaluateSkewnessMean() << '\n';
    std::cout << " [Skewness]\t\tMin/Max\t\t" << (receiver->evaluateSkewnessMinMax()).first << " / " << (receiver->evaluateSkewnessMinMax()).second << '\n';
    std::cout << " -----------------------------------------------------------------" << '\n';
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

  auto num_emitter_elements = emitter->getNumElements();
  auto num_receiver_elements = receiver->getNumElements();
  auto emitter_centroids = emitter->getCentroids();
  auto receiver_centroids = receiver->getCentroids();
  auto emitter_normals = emitter->getNormals();
  auto receiver_normals = receiver->getNormals();
  auto receiver_elements = receiver->getTriangles();

  FLOAT_TYPE back_face_cull_time;
  FLOAT_TYPE blocking_time;
  Timer solver_timer;
  std::shared_ptr<ViewFactor<FLOAT_TYPE>> results = std::make_shared<ViewFactor<FLOAT_TYPE>>();
  if (numeric == "DAI") {
    //* ---------- DOUBLE AREA INTEGRATION ---------- *//
    DoubleAreaIntegration<FLOAT_TYPE> solver;
    //! ---------- BACK-FACE CULLING ---------- !//
    std::vector<unsigned int> unculled_indices(num_emitter_elements * num_receiver_elements);
    if (back_face_cull_mode == "ON") {
      std::cout << "[LOG] Applying Back Face Culling" << '\n';
      unculled_indices = solver.backFaceCullMeshes(num_emitter_elements, num_receiver_elements, &emitter_centroids, &receiver_centroids, &emitter_normals, &receiver_normals);
    } else {
      std::iota(unculled_indices.begin(), unculled_indices.end(), 0);
    }
    back_face_cull_time = solver_timer.elapsed();
    //! ---------- OBSTRUCTIONS ---------- !//
    auto unblocked_indices = solver.evaluateBlockingBetweenMeshes(num_emitter_elements, num_receiver_elements, emitter_centroids, receiver_centroids, blockers, unculled_indices);
    blocking_time = solver_timer.elapsed() - back_face_cull_time;
    //! ---------- VIEW FACTOR SOLUTIONS ---------- !//
    results->linkTriangulations(emitter, receiver, unblocked_indices.size());
    solver.solveViewFactorBetweenMeshes(num_emitter_elements, num_receiver_elements, &emitter_centroids, &emitter_normals, &receiver_elements, &unblocked_indices, results);

  } else if (numeric == "SAI") {
    //* ---------- SINGLE AREA INTEGRATION ---------- *//
    SingleAreaIntegration<FLOAT_TYPE> solver;
    //! ---------- BACK-FACE CULLING ---------- !//
    std::vector<unsigned int> unculled_indices(num_emitter_elements * num_receiver_elements);
    if (back_face_cull_mode == "ON") {
      unculled_indices = solver.backFaceCullMeshes(num_emitter_elements, num_receiver_elements, &emitter_centroids, &receiver_centroids, &emitter_normals, &receiver_normals);
    } else {
      std::iota(unculled_indices.begin(), unculled_indices.end(), 0);
    }
    back_face_cull_time = solver_timer.elapsed();
    //! ---------- OBSTRUCTIONS ---------- !//
    auto unblocked_indices = solver.evaluateBlockingBetweenMeshes(num_emitter_elements, num_receiver_elements, emitter_centroids, receiver_centroids, blockers, unculled_indices);
    blocking_time = solver_timer.elapsed() - back_face_cull_time;
    //! ---------- VIEW FACTOR SOLUTIONS ---------- !//
    results->linkTriangulations(emitter, receiver, unblocked_indices.size());
    solver.solveViewFactorBetweenMeshes(num_emitter_elements, num_receiver_elements, &emitter_centroids, &emitter_normals, &receiver_elements, &unblocked_indices, results);
  }

  auto solver_time = solver_timer.elapsed() - back_face_cull_time - blocking_time;

  std::cout << "[LOG] Back Face Cull Completed in " << back_face_cull_time << " [s]" << '\n';
  std::cout << "[LOG] Blocking Completed in " << blocking_time << " [s]" << '\n';
  std::cout << "[LOG] View Factors Completed in " << solver_time << "[s]" << '\n';
  std::cout << "[LOG] Total Solution Time: " << back_face_cull_time + blocking_time + solver_time << " [s]" << '\n';

  auto surface_to_surface_view_factor = results->getSurfaceToSurfaceAverageVF();

  std::cout << "\n[RESULT] Surface-Surface View Factor : " << std::setprecision(15) << surface_to_surface_view_factor << '\n' << '\n';

  if (write_matrix) { std::cout << "[OUT] Writing Plain Text Matrix Output (not doing anything yet)" << '\n'; }

  if (write_graphic) {
    OutputWriter<FLOAT_TYPE> output_writer(results);
    switch (num_graphic_outfiles) {
      case 1:
        std::cout << "[OUT] Writing Emitter Graphic File Output" << '\n';
        output_writer.writeEmitterVisualization(emitter_output_filename);
        break;
      case 2:
        std::cout << "[OUT] Writing Emitter Graphic File Output" << '\n';
        output_writer.writeEmitterVisualization(emitter_output_filename);
        std::cout << "[OUT] Writing Receiver Graphic File Output" << '\n';
        output_writer.writeReceiverFromEmitterVisualization(receiver_output_filename);
        break;
      case 3:
        std::cout << "[OUT] Writing Emitter Graphic File Output" << '\n';
        output_writer.writeEmitterVisualization(emitter_output_filename);
        std::cout << "[OUT] Writing Receiver Graphic File Output" << '\n';
        output_writer.writeReceiverFromEmitterVisualization(receiver_output_filename);
        std::cout << "[OUT] Writing Unified Graphic File Output" << '\n';
        output_writer.writeCombinedVisualization(unified_output_filename);
        break;
    }
  }

  std::cout << "[LOG] Total I/O Time: " << total_program_timer.elapsed() << " [s]" << '\n';
}
#endif