#include "all_headers.hpp"

#include "cli.hpp"
#include "geometry.hpp"
#include "solver.hpp"
#include "results.hpp"
#include "io.hpp"
#include "workflow.hpp"

//! ----- GENERAL STUFF ----- !//

//! ----- main ----- !//
int main(int argc, char *argv[]) {
  cli::po::variables_map variables_map = cli::parseCommandLine(argc, argv);
  if (variables_map.count("help")) {
    std::cout << cli::getOptions() << '\n';
    return 0;
  }
  if (variables_map.count("version")) {
    std::cout << "OpenViewFactor Version : " << OVF_VERSION_STRING << '\n';
    return 0;
  }
  cli::po::notify(variables_map);
  std::string precision = variables_map["precision"].as<std::string>();
  if (precision == "SINGLE") {
    workflow::ovfWorkflow<float>(variables_map);
  } else if (precision == "DOUBLE") {
    workflow::ovfWorkflow<long double>(variables_map);
  }
  return 0;
}