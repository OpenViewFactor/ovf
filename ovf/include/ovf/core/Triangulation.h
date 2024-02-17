#ifndef OVF_TRIANGULATION_H_
#define OVF_TRIANGULATION_H_

#include <string>
#include <vector>
#include <algorithm>

#include "ovf/core/config.h"
#include "ovf/core/Vector3.h"

namespace openviewfactor {

template <typename FLOAT_TYPE> class Triangulation {
  private:
    std::vector<Vector3<FLOAT_TYPE>> _pts;  // points list in the triangulation
    std::vector<size_t> _con; // connectivity list in the triangulation
    std::string _name;  // mesh name (i assume this was EMITTER, RECEIVER, BLOCKER)
  public:
    //* ----- CLASS CONSTRUCTORS ----- *//
     OVF_HOST_DEVICE Triangulation();  // default constructor
     OVF_HOST_DEVICE Triangulation(const std::string &name);  // name constructor

     //* ----- ACCESSOR METHODS ----- *//
     OVF_HOST_DEVICE std::string getName() const;
     OVF_HOST_DEVICE size_t size() const;
     OVF_HOST_DEVICE size_t capacity() const;
     OVF_HOST_DEVICE size_t numBytes() const;
     OVF_HOST_DEVICE FLOAT_TYPE area() const;
     
     //* ----- MUTATOR METHODS ----- *//

     //* ----- OPERATOR OVERLOADS ----- *//
};
}

#endif