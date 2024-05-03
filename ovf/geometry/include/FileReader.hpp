#ifndef OVF_FILEREADER_H_
#define OVF_FILEREADER_H_

#include "geometry/include/Triangulation.hpp"
#include <string>

namespace openviewfactor {

template <typename FLOAT_TYPE> class FileReader {

  using TGLN = Triangulation<FLOAT_TYPE>;
  
  public:
    virtual ~FileReader() {} //! declare this to be subclassable

    virtual TGLN getMesh(const std::string &filename) = 0;  //! pure virtual function will be defined in the subclass
};
}

#endif