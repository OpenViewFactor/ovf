#include "cli.hpp"

int main(int argc, char *argv[]) {
  po::variables_map variables_map = parseCommandLine(argc, argv);

  if (variables_map.count("help")) {
    std::cout << getOptions() << '\n';
    return 0;
  }

  if (variables_map.count("version")) {
    std::cout << "OpenViewFactor Version : " << OVF_VERSION_STRING << '\n';
    return 0;
  }

  po::notify(variables_map);

  // std::string precision = variables_map["precision"].as<std::string>();
  // std::cout << "[LOG] Solver Setting Loaded: Floating Point Precision - " << precision << '\n';

  // if (precision == "SINGLE") {
  //   ovfWorkflow<float>(variables_map);
  // } else if (precision == "DOUBLE") {
  //   ovfWorkflow<double>(variables_map);
  // }

  ovfWorkflow(variables_map);
}