#ifndef OVF_STLREADER_H_
#define OVF_STLREADER_H_

#include "stl_reader.h"
#include "Triangulation.hpp"
#include "FileReader.hpp"
#include <exception>

namespace openviewfactor {

template <typename FLOAT_TYPE> class STLReader : public FileReader<FLOAT_TYPE> {

  public:
    std::shared_ptr<Triangulation<FLOAT_TYPE>> getMesh(const std::string &filename);
};
}

#endif