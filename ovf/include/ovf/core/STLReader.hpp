#ifndef OVF_STLREADER_H_
#define OVF_STLREADER_H_

#include "third_party/stl_reader.h"
#include "ovf/core/FileReader.hpp"
#include <exception>

namespace openviewfactor {

template <typename FLOAT_TYPE> class STLReader : public FileReader<FLOAT_TYPE> {

  using TGLN = Triangulation<FLOAT_TYPE>;
  public:
    TGLN getMesh(const std::string &filename);
};
}

#endif