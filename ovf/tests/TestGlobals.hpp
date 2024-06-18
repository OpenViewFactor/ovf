#ifndef OVF_TESTGLOBALS_H_
#define OVF_TESTGLOBALS_H_

#include <string>
#include <vector>

// Input and Output directory for testing
std::string OVF_INPUT(const std::string &path) {
    return (std::string("${ovf_test_input_path}/") + path);
}
std::string OVF_OUTPUT(const std::string &path) {
    return (std::string("${ovf_test_output_path}/") + path);
}

#endif