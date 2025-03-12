#include "all_headers.hpp"

#include "geometry.hpp"
#include "solver.hpp"
#include "results.hpp"
#include "io.hpp"

#pragma once

//! ----- OpenViewFactor Workflow ----- !//
namespace workflow {

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
  void reset() {
    m_beg = Clock::now();
  }
  double elapsed() const {
    return std::chrono::duration_cast<Second>(Clock::now() - m_beg).count();
  }
};

template <typename T> void ovfWorkflow(cli::po::variables_map variables_map) {

  bool write_log = false;
  if ( variables_map["logfile"].as<std::string>() != "NONE" ) {
    write_log = true;
  }
  std::vector<std::string> log_messages;

  std::string log_wrapper = "------------------------------------------------------------------\n";
  std::string print_version = "[VERSION] OpenViewFactor Version: " + OVF_VERSION_STRING + "\n\n";
  
  std::cout << log_wrapper;
  std::cout << print_version;

  std::string back_face_cull_mode = variables_map["backfacecull"].as<std::string>();
  std::string blocking_type = variables_map["blockingtype"].as<std::string>();
  std::string self_int_type = variables_map["selfint"].as<std::string>();
  std::string numeric = variables_map["numerics"].as<std::string>();
  std::string compute = variables_map["compute"].as<std::string>();
  std::string precision = variables_map["precision"].as<std::string>();

  std::string load_back_face_cull = "[LOG] Solver Setting Loaded: Back Face Cull Mode -\t" + back_face_cull_mode + '\n';
  std::string load_blocking_mode = "[LOG] Solver Setting Loaded: Blocking Mode -\t\t" + blocking_type + '\n';
  std::string load_selfint = "[LOG] Solver Setting Loaded: Self-Intersection Mode -\t" + self_int_type + '\n';
  std::string load_numeric = "[LOG] Solver Setting Loaded: Numeric Method -\t\t" + numeric + '\n';
  std::string load_compute = "[LOG] Solver Setting Loaded: Compute Backend -\t\t" + compute + '\n';
  std::string load_precision = "[LOG] Solver Setting Loaded: Floating Point Precision -\t" + precision + '\n';

  std::cout << load_back_face_cull;
  std::cout << load_blocking_mode;
  std::cout << load_selfint;
  std::cout << load_numeric;
  std::cout << load_compute;
  std::cout << load_precision;

  log_messages.push_back(load_back_face_cull);
  log_messages.push_back(load_blocking_mode);
  log_messages.push_back(load_selfint);
  log_messages.push_back(load_numeric);
  log_messages.push_back(load_compute);
  log_messages.push_back(load_precision);

  std::cout << '\n';





  std::string bvh_outfile = variables_map["bvhout"].as<std::string>();
  std::string bvh_output_filename;
  std::string matrix_outfile = variables_map["matrixout"].as<std::string>();
  std::vector<std::string> graphic_outfiles = variables_map["graphicout"].as<std::vector<std::string>>();
  int num_graphic_outfiles = graphic_outfiles.size();
  std::string emitter_output_filename, receiver_output_filename, unified_output_filename;



  bool write_bvh = (bvh_outfile == "NONE") ? false : true;
  bool write_matrix = (matrix_outfile == "NONE") ? false : true;
  bool write_graphic = (graphic_outfiles[0] == "NONE") ? false : true;


  std::string log_bvh_output;
  if (write_bvh) {
    bvh_output_filename = bvh_outfile + ".vtu";
    log_bvh_output = "[LOG] Blocker BVH Output Path : " + bvh_output_filename + '\n';
  } else {
    log_bvh_output = "[LOG] NO Blocker BVH Output\n";
  }
  std::cout << log_bvh_output;
  log_messages.push_back(log_bvh_output);


  std::string log_matrix_output;
  if (write_matrix) {
    std::string matrix_output_filename = matrix_outfile + ".ovf";
    log_matrix_output = "[LOG] Plain Text Matrix Output Path : " + matrix_output_filename + '\n';
  } else {
    log_matrix_output = "[LOG] NO Plain Text Matrix Output\n";
  }
  std::cout << log_matrix_output;
  log_messages.push_back(log_matrix_output);
  

  std::string log_graphic_output;
  if (write_graphic) {
    emitter_output_filename = graphic_outfiles[0] + ".vtu";
    log_graphic_output = "[LOG] Emitter Graphic File Output Path : " + emitter_output_filename + '\n';
    if (num_graphic_outfiles > 1) {
      receiver_output_filename = graphic_outfiles[1] + ".vtu";
      log_graphic_output += "[LOG] Receiver Graphic File Output Path : " + receiver_output_filename + '\n';
    }
    if (num_graphic_outfiles > 2) {
      unified_output_filename = graphic_outfiles[2] + ".vtu";
      log_graphic_output += "[LOG] Unified Graphic File Output Path : " + unified_output_filename + '\n';
    }
  } else {
    log_graphic_output = "[LOG] NO Graphic File Output\n";
  }
  std::cout << log_graphic_output;
  log_messages.push_back(std::string(log_graphic_output));

  std::cout << '\n';





  std::vector<std::string> blocker_filenames;
  bool blocking_enabled = false;
  if (variables_map.count("obstructions")) {
    blocking_enabled = true;
    blocker_filenames = variables_map["obstructions"].as<std::vector<std::string>>();
  }

  std::vector<std::string> input_filenames = variables_map["inputs"].as<std::vector<std::string>>();
  std::string log_inputs_too_long = "<-----> [NOTIFIER] More than 2 input meshes were provided! Only the first two will be loaded\n";
  if (input_filenames.size() > 2) {
    std::cout << log_inputs_too_long;
    log_messages.push_back(log_inputs_too_long);
  }
  bool two_mesh_problem = (input_filenames.size() > 1) ? true : false;


  Timer loading_meshes_timer;

  geometry::mesh<T> blocking_mesh;
  geometry::mesh<T> new_blocking_mesh;
  geometry::mesh<T> e_mesh;
  geometry::mesh<T> r_mesh;

  std::cout << "[LOG] Loading Meshes\n";
  log_messages.push_back(std::string("[LOG] Loading Meshes\n"));
  
  std::cout << "[LOG] Loading Emitting Mesh : " << input_filenames[0] << '\n';
  log_messages.push_back(std::string("[LOG] Loading Emitting Mesh : " + input_filenames[0] + '\n'));
  e_mesh = geometry::getMesh<T>( input_filenames[0] );

  io::printMeshMetrics(&e_mesh);
  io::logMeshMetrics(&log_messages, &e_mesh);

  if (two_mesh_problem) {
    std::cout << "[LOG] Loading Receiving Mesh : " << input_filenames[1] << '\n';
    log_messages.push_back(std::string("[LOG] Loading Receiving Mesh : " + input_filenames[1] + '\n'));
    r_mesh = geometry::getMesh<T>( input_filenames[1] );
    io::printMeshMetrics(&r_mesh);
    io::logMeshMetrics(&log_messages, &r_mesh);
  } else {
    r_mesh = e_mesh;
  }

  std::string log_blocking_mesh;
  if (blocking_enabled) {
    for (auto file : blocker_filenames) {
      std::cout << "[LOG] Loading Blocking Mesh : " << file << '\n';
      log_messages.push_back(std::string("[LOG] Loading Blocking Mesh : " + file + '\n'));
      new_blocking_mesh = geometry::getMesh<T>( file );
      blocking_mesh = blocking_mesh + &new_blocking_mesh;
    }
  } else {
    std::cout << "[LOG] NO Blocking Meshes Loaded\n";
    log_messages.push_back(std::string("[LOG] NO Blocking Meshes Loaded\n"));
  }

  if (self_int_type == "EMITTER" || self_int_type == "BOTH") {
    std::cout << "[LOG] Adding Emitter Mesh to Blocking Structure\n";
    log_messages.push_back(std::string("[LOG] Adding Emitter Mesh to Blocking Structure\n"));
    blocking_mesh = blocking_mesh + &e_mesh;
  }
  if (self_int_type == "RECEIVER" || self_int_type == "BOTH") {
    std::cout << "[LOG] Adding Receiver Mesh to Blocking Structure\n";
    log_messages.push_back(std::string("[LOG] Adding Receiver Mesh to Blocking Structure\n"));
    blocking_mesh = blocking_mesh + &r_mesh;
  }

  std::cout << "[LOG] Meshes loaded in " << loading_meshes_timer.elapsed() << " [s]\n";
  log_messages.push_back(std::string("[LOG] Meshes loaded in " + std::to_string(loading_meshes_timer.elapsed()) + " [s]\n"));

  unsigned int problem_size = e_mesh.size() * r_mesh.size();
  std::cout << "[LOG] Problem Size: " << problem_size << " Pairs\n";
  log_messages.push_back(std::string("[LOG] Problem Size: " + std::to_string(problem_size) + " Pairs\n"));

  std::cout << '\n';





  Timer bvh_timer;

  geometry::BVH<T> blocker(&blocking_mesh);

  if (blocking_type == "BVH") {

    if (blocking_enabled || self_int_type != "NONE") {
      std::cout << "[LOG] Generating obstructing Boundary Volume Hierarchy (BVH)\n";
      log_messages.push_back(std::string("[LOG] Generating obstructing Boundary Volume Hierarchy (BVH)\n"));
      geometry::constructBVH(&blocker, &blocking_mesh);
      std::cout << "[LOG] BVH generated in " << bvh_timer.elapsed() << " [s]\n";
      std::cout << "[LOG] BVH Nodes Used = " << blocker._nodes_used << '\n';
      log_messages.push_back(std::string("[LOG] BVH generated in " + std::to_string(bvh_timer.elapsed()) + " [s]\n" + "[LOG] BVH Nodes Used = " + std::to_string(blocker._nodes_used) + '\n'));
      std::cout << '\n';
    }
    
    if (write_bvh) {
      std::cout << "[OUTPUT] Writing BVH visualization\n";
      log_messages.push_back(std::string("[OUTPUT] Writing BVH visualization\n"));
      io::writeToFile(&blocking_mesh, bvh_outfile + "-mesh" + ".vtu");
      io::writeToFile(&blocker, bvh_output_filename);
    }

  }


  

  Timer solver_timer;

  std::vector<geometry::v3<T>> e_centroids = geometry::centroids(&e_mesh);
  std::vector<geometry::v3<T>> e_normals = geometry::normals(&e_mesh);

  std::vector<geometry::v3<T>> r_centroids = geometry::centroids(&r_mesh);
  std::vector<geometry::v3<T>> r_normals = geometry::normals(&r_mesh);
  std::vector<geometry::tri<T>> r_triangles = geometry::allTriangles(&r_mesh);

  std::vector<std::vector<unsigned int>*> unculled_indices(e_mesh.size());
  for (int i = 0; i < e_mesh.size(); i++) {
    std::vector<unsigned int>* sub_indices = new std::vector<unsigned int>(r_mesh.size());
    std::iota(sub_indices->begin(), sub_indices->end(), 0);
    unculled_indices[i] = sub_indices;
  }

  if (back_face_cull_mode == "ON") {
    std::cout << "[LOG] Applying Back-Face Cull\n";
    log_messages.push_back(std::string("[LOG] Applying Back-Face Cull\n"));

    solver::backFaceCullMeshes(&e_centroids, &e_normals, &r_centroids, &r_normals, &unculled_indices);

    std::cout << "[LOG] Back-Face Cull completed in " << solver_timer.elapsed() << " [s]\n";
    log_messages.push_back(std::string("[LOG] Back-Face Cull completed in " + std::to_string(solver_timer.elapsed()) + " [s]\n"));

    solver_timer.reset();
  }

  if (blocking_enabled) {
    std::cout << "[LOG] Applying Blocking\n";
    log_messages.push_back(std::string("[LOG] Applying Blocking\n"));
  
    if (blocking_type == "NAIVE") {
      solver::naiveBlockingBetweenMeshes(&blocking_mesh, &e_centroids, &r_triangles, &unculled_indices);
    } else if (blocking_type == "BVH") {
      solver::bvhBlockingBetweenMeshes(&blocker, &blocking_mesh, &e_centroids, &r_triangles, &unculled_indices);
    }

    std::cout << "[LOG] Blocking completed in " << solver_timer.elapsed() << " [s]\n";
    log_messages.push_back(std::string("[LOG] Blocking completed in " + std::to_string(solver_timer.elapsed()) + " [s]\n"));
    solver_timer.reset();
  }

  std::cout << "[LOG] Evaluating View Factors\n";
  log_messages.push_back(std::string("[LOG] Evaluating View Factors\n"));
  if (numeric == "DAI") {
    std::cout << "[LOG] Applying Double Area Integration\n";
    log_messages.push_back(std::string("[LOG] Applying Double Area Integration\n"));
  } else if (numeric == "SAI") {
    std::cout << "[LOG] Applying Single Area Integration\n";
    log_messages.push_back(std::string("[LOG] Applying Single Area Integration\n"));
  }
  
  std::vector<std::vector<T>*> view_factors(e_mesh.size());
  for (int i = 0; i < e_mesh.size(); i++) {
    std::vector<T>* sub_results = new std::vector<T>( ( (unculled_indices)[i] )->size() );
    view_factors[i] = sub_results;
  }
  solver::viewFactors(&e_centroids, &e_normals, &r_triangles, &unculled_indices, &view_factors, numeric);

  std::cout << "[LOG] View Factors completed in " << solver_timer.elapsed() << " [s]\n";
  log_messages.push_back(std::string("[LOG] View Factors completed in " + std::to_string(solver_timer.elapsed()) + " [s]\n"));

  std::cout << '\n';



  Timer results_timer;

  std::cout << "[LOG] Evaluating Results\n";
  log_messages.push_back(std::string("[LOG] Evaluating Results\n"));

  results::solution<T> s(&unculled_indices, &view_factors, e_mesh.size(), r_mesh.size());
  std::vector<T> e_areas = geometry::areas(&e_mesh);
  T surface_to_surface_vf = results::surfaceVF(&s, &e_areas);

  std::cout << "[RESULT] Surface-Surface View Factor: " << std::setprecision(15) << surface_to_surface_vf << '\n';
  log_messages.push_back(std::string("[RESULT] Surface-Surface View Factor: " + std::to_string(surface_to_surface_vf) + '\n'));

  std::cout << "[LOG] Results evaluated in " << results_timer.elapsed() << " [s]\n";
  log_messages.push_back(std::string("[LOG] Results evaluated in " + std::to_string(results_timer.elapsed()) + " [s]\n"));

  std::cout << '\n';


  Timer output_timer;

  if (write_bvh) {
    std::cout << "[OUTPUT] Writing BVH visualization\n";
    io::writeToFile(&blocking_mesh, bvh_outfile + "-mesh" + ".vtu");
    io::writeToFile(&blocker, bvh_output_filename);
    std::cout << "[LOG] Blocker visualizations written in " << output_timer.elapsed() << " [s]\n";
    output_timer.reset();
  }


  if (write_matrix) {
    std::cout << "[OUTPUT] Writing plain-text matrix output\n";
    //TODO write plain-text output
    std::cout << "[LOG] Results matrix written in " << output_timer.elapsed() << " [s]\n";
    output_timer.reset();
  }


  if (write_graphic) {
    std::cout << "[OUTPUT] Writing Emitter .vtu file\n";
    io::writeToFile(&s, &e_mesh, &r_mesh, emitter_output_filename, io::VisualOutputMode::EMITTER);

    std::cout << "[LOG] Emitter visualization written in " << output_timer.elapsed() << " [s]\n";
    output_timer.reset();

    if (num_graphic_outfiles > 1) {
      std::cout << "[OUTPUT] Writing Receiver .vtu file\n";
      io::writeToFile(&s, &e_mesh, &r_mesh, receiver_output_filename, io::VisualOutputMode::RECEIVER);

      std::cout << "[LOG] Receiver visualization written in " << output_timer.elapsed() << " [s]\n";
      output_timer.reset();
    }

    if (num_graphic_outfiles > 2) {
      std::cout << "[OUTPUT] Writing Unified .vtu file\n";
      io::writeToFile(&s, &e_mesh, &r_mesh, emitter_output_filename, io::VisualOutputMode::BOTH);

      std::cout << "[LOG] Unified visualization written in " << output_timer.elapsed() << " [s]\n";
      output_timer.reset();
    }
  }

  std::cout << "------------------------------------------------------------------\n";

  if (write_log) {
    std::ofstream log_file ( variables_map["logfile"].as<std::string>() + ".txt" );
    for (auto message : log_messages) {
      log_file << message;
    }
    log_file.close();
  }
}

}