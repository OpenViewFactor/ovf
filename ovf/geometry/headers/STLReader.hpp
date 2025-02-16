#ifndef OVF_STLREADER_H_
#define OVF_STLREADER_H_

#include "stl_reader.h"
#include "FileReader.hpp"
#include "Vector3.hpp"
#include "Triangle.hpp"
#include "Triangulation.hpp"
#include <vector>
#include <array>
#include <iostream>
#include <exception>
#include <memory>
#include <omp.h>

namespace openviewfactor {

template <typename t> class STLReader : public FileReader<t> {
  private:
  protected:
  public:
    std::shared_ptr<Triangulation<t>> getMesh(const std::string &filename);
};
}

#endif