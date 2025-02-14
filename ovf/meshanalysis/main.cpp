#include "CommandLineOptions.hpp"
#include "analysis.hpp"

int main(int argc, char *argv[]) {
  po::variables_map variables_map = parseCommandLine(argc, argv);
  
  if (variables_map.count("help")) {
    std::cout << getOptions() << '\n';
    return 0;
  }

  po::notify(variables_map);
  meshAnalysis(variables_map);
  return 0;
}