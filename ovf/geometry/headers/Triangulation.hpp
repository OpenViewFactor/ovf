#ifndef OVF_TRIANGULATION_H_
#define OVF_TRIANGULATION_H_

#include <string>
#include <array>
#include <vector>
#include <algorithm>
#include <cmath>
#include <numeric>
#include <memory>

#include "../../config.hpp"
#include "Triangle.hpp"

namespace openviewfactor {

template <typename t> class Triangulation {

  private:
    std::vector<Vector3<t>> _pts;  // points list in the triangulation
    std::vector<std::array<size_t, 3>> _con; // connectivity list in the triangulation
  protected:
  public:
    //* ----- CLASS CONSTRUCTORS ----- *//
    gpuify Triangulation();  // default constructor

    //* ----- ACCESSOR METHODS ----- *//
    gpuify size_t getNumElements() const;
    gpuify size_t getNumPoints() const;
    gpuify size_t getNumBytes() const;

    gpuify const std::array<size_t,3>* getConPtr() const;
    gpuify const Vector3<t>* getPtsPtr() const;
    
    gpuify const Triangle<t> operator[](size_t index) const;
    gpuify Triangle<t> operator[](size_t index);
    
    gpuify t getMeshArea() const;

    gpuify std::shared_ptr<Triangulation<t>> getSubMesh(std::vector<unsigned int> indices) const;

    gpuify std::vector<Vector3<t>> getCentroids() const;
    gpuify std::vector<Vector3<t>> getNormals() const;
    gpuify std::vector<t> getAreas() const;
    gpuify std::vector<Triangle<t>> getTriangles() const;

    //* ----- MUTATOR METHODS ----- *//
    gpuify Triangulation<t>& clear();
    gpuify Triangulation<t>& addElement(Vector3<t> OA, Vector3<t> OB, Vector3<t> OC);
    gpuify Triangulation<t>& addElement(Triangle<t> tri);

    gpuify Triangulation<t>& setConnectivity(std::vector<std::array<size_t, 3>> con);
    gpuify Triangulation<t>& setPoints(std::vector<Vector3<t>> pts);

    gpuify bool operator==(const Triangulation<t>& rhs) const;

    //* ----- MESH QUALITY ANALYSIS ----- *//
    gpuify std::vector<t> getAllElementsAspectRatios() const;
    gpuify std::vector<t> getAllElementsSkewness() const;
    gpuify std::vector<t> getAllElementsElementQualities() const;
    gpuify t evaluateAspectRatioMean() const;
    gpuify t evaluateSkewnessMean() const;
    gpuify t evaluateElementQualityMean() const;
    gpuify t evaluateAspectRatioStandardDeviation() const;
    gpuify t evaluateSkewnessStandardDeviation() const;
    gpuify t evaluateElementQualityStandardDeviation() const;
    gpuify std::pair<t,t> evaluateAspectRatioMinMax() const;
    gpuify std::pair<t,t> evaluateSkewnessMinMax() const;
    gpuify std::pair<t,t> evaluateElementQualityMinMax() const;
};
}

#endif