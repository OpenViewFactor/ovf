#include "Vector3.hpp"
#include "Triangle.hpp"
#include "Triangulation.hpp"

#include <iostream>

namespace openviewfactor {
  //* ----- CLASS CONSTRUCTORS ----- *//
  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE Triangulation<FLOAT_TYPE>::Triangulation()
    : _pts(std::vector<Vector3<FLOAT_TYPE>>()), _con(std::vector<std::array<size_t, 3>>()) {}

  //* ----- ACCESSOR METHODS ----- *//
  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE size_t Triangulation<FLOAT_TYPE>::getNumElements() const { return _con.size(); }
  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE size_t Triangulation<FLOAT_TYPE>::getNumPoints() const { return _pts.size(); }
  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE size_t Triangulation<FLOAT_TYPE>::getNumBytes() const { return (_con.size() * sizeof(size_t) * 3) + (_pts.size() * sizeof(Vector3<FLOAT_TYPE>)); }

  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE const std::array<size_t,3>* Triangulation<FLOAT_TYPE>::getConPtr() const { return _con.data(); }

  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE const Vector3<FLOAT_TYPE>* Triangulation<FLOAT_TYPE>::getPtsPtr() const { return _pts.data(); }

  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE const Triangle<FLOAT_TYPE> Triangulation<FLOAT_TYPE>::operator[](size_t index) const {
    Triangle<FLOAT_TYPE> T(_pts[ (_con[index])[0] ], _pts[ (_con[index])[1] ], _pts[ (_con[index])[2] ]);
    return T;
  }
  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE Triangle<FLOAT_TYPE> Triangulation<FLOAT_TYPE>::operator[](size_t index) {
    Triangle<FLOAT_TYPE> T(_pts[ (_con[index])[0] ], _pts[ (_con[index])[1] ], _pts[ (_con[index])[2] ]);
    return T;
  }

  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE FLOAT_TYPE Triangulation<FLOAT_TYPE>::getMeshArea() const {
    FLOAT_TYPE total_area = 0;
    for (size_t i = 0; i < this->getNumElements(); i++) {
      total_area += ((*this)[i]).getArea();
    }
    return total_area;
  }

  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE std::shared_ptr<Triangulation<FLOAT_TYPE>> Triangulation<FLOAT_TYPE>::getSubMesh(std::vector<unsigned int> indices) const {    
    auto sub_mesh = std::make_shared<Triangulation<FLOAT_TYPE>>();
    std::vector<std::array<size_t, 3>> sub_mesh_connectivity(indices.size());
    std::generate(sub_mesh_connectivity.begin(), sub_mesh_connectivity.end(), [this, indices] (unsigned int i=0) mutable { return (this->_con)[indices[i++]]; }); 
    sub_mesh->setConnectivity(sub_mesh_connectivity);
    sub_mesh->setPoints(_pts);    
    return sub_mesh;
  }

  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE std::vector<FLOAT_TYPE> Triangulation<FLOAT_TYPE>::getAreas() const {
    std::vector<FLOAT_TYPE> mesh_areas(this->getNumElements());
    for (int i = 0; i < getNumElements(); i++) {
      mesh_areas[i] = (*this)[i].getArea();
    }
    return mesh_areas;
  }

  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE std::vector<Vector3<FLOAT_TYPE>> Triangulation<FLOAT_TYPE>::getCentroids() const {
    std::vector<Vector3<FLOAT_TYPE>> mesh_centroids(this->getNumElements());
    for (int i = 0; i < getNumElements(); i++) {
      mesh_centroids[i] = (*this)[i].getCentroid();
    }
    return mesh_centroids;
  }

  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE std::vector<Vector3<FLOAT_TYPE>> Triangulation<FLOAT_TYPE>::getNormals() const {
    std::vector<Vector3<FLOAT_TYPE>> mesh_normals(this->getNumElements());
    for (int i = 0; i < getNumElements(); i++) {
      mesh_normals[i] = (*this)[i].getNormal();
    }
    return mesh_normals;
  }

  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE std::vector<Triangle<FLOAT_TYPE>> Triangulation<FLOAT_TYPE>::getTriangles() const {
    std::vector<Triangle<FLOAT_TYPE>> mesh_triangles(this->getNumElements());
    for (unsigned int i = 0; i < this->getNumElements(); i++) {
      mesh_triangles[i] = (*this)[i];
    }
    return mesh_triangles;
  }

  //* ----- MUTATOR METHODS ----- *//
  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE Triangulation<FLOAT_TYPE>& Triangulation<FLOAT_TYPE>::clear() {
    _pts.clear();
    _con.clear();
    return *this;
  }
  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE Triangulation<FLOAT_TYPE>& Triangulation<FLOAT_TYPE>::addElement(Vector3<FLOAT_TYPE> OA, Vector3<FLOAT_TYPE> OB, Vector3<FLOAT_TYPE> OC) {
    std::array<Vector3<FLOAT_TYPE>, 3> new_points = {OA, OB, OC};
    std::array<size_t, 3> new_connectivity;
    for (size_t i = 0; i < 3; i++) {
      auto point_location_iterator = std::find(_pts.begin(), _pts.end(), new_points[i]);
      if (point_location_iterator != _pts.end()) {
        new_connectivity[i] = std::distance(_pts.begin(), point_location_iterator);
      } else {
        _pts.push_back(new_points[i]);
        new_connectivity[i] = _pts.size() - 1;
      }
    }
    _con.push_back(new_connectivity);
    return *this;
  }
  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE Triangulation<FLOAT_TYPE>& Triangulation<FLOAT_TYPE>::addElement(Triangle<FLOAT_TYPE> tri) {
    this->addElement(tri.getOA(), tri.getOB(), tri.getOC());
    return *this;
  }

  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE Triangulation<FLOAT_TYPE>& Triangulation<FLOAT_TYPE>::removeElement(unsigned int index) {
    auto element_connectivity = _con[index];
    std::remove(_con.begin(), _con.end(), element_connectivity);
    _con.pop_back();
    return *this;
  }

  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE Triangulation<FLOAT_TYPE>& Triangulation<FLOAT_TYPE>::removeDegenerateElements() {
    for (int i = 0; i < this->getNumElements(); i++) {
      if (std::isinf((*this)[i].evaluateAspectRatio()) || std::isnan((*this)[i].evaluateSkewness())) {
        this->removeElement(i);
        i--;
      }
    }
    return *this;
  }

  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE Triangulation<FLOAT_TYPE>& Triangulation<FLOAT_TYPE>::setConnectivity(std::vector<std::array<size_t, 3>> con) {
    _con.clear();
    _con = std::move(con);
    return *this;
  }
  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE Triangulation<FLOAT_TYPE>& Triangulation<FLOAT_TYPE>::setPoints(std::vector<Vector3<FLOAT_TYPE>> pts) {
    _pts.clear();
    _pts = std::move(pts);
    return *this;
  }

  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE bool Triangulation<FLOAT_TYPE>::operator==(const Triangulation<FLOAT_TYPE>& rhs) const {
    if (this->getNumElements() != rhs.getNumElements()) { return false; }
    if (this->getNumPoints() != rhs.getNumPoints()) { return false; }
    if (this->getMeshArea() != rhs.getMeshArea()) { return false; }
    for (size_t i = 0; i < this->getNumElements(); i++) {
      if ((*this)[i] != rhs[i]) { return false; }
    }
    return true;
  }


  //* ----- MESH QUALITY ANALYSIS ----- *//
  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE std::vector<FLOAT_TYPE> Triangulation<FLOAT_TYPE>::getAllElementsAspectRatios() const {
    auto num_elements = this->getNumElements();
    std::vector<FLOAT_TYPE> aspect_ratios(num_elements);
    for (int i = 0; i < num_elements; i++) {
      aspect_ratios[i] = (*this)[i].evaluateAspectRatio();
    }
    return aspect_ratios;
  }

  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE std::vector<FLOAT_TYPE> Triangulation<FLOAT_TYPE>::getAllElementsElementQualities() const {
    auto num_elements = this->getNumElements();
    std::vector<FLOAT_TYPE> qualities(num_elements);
    for (int i = 0; i < num_elements; i++) {
      qualities[i] = (*this)[i].evaluateElementQuality();
    }
    return qualities;
  }

  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE std::vector<FLOAT_TYPE> Triangulation<FLOAT_TYPE>::getAllElementsSkewness() const {
    auto num_elements = this->getNumElements();
    std::vector<FLOAT_TYPE> skewness(num_elements);
    for (int i = 0; i < num_elements; i++) {
      skewness[i] = (*this)[i].evaluateSkewness();
    }
    return skewness;
  }

  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE FLOAT_TYPE Triangulation<FLOAT_TYPE>::evaluateAspectRatioMean() const {
    std::vector<FLOAT_TYPE> all_aspect_ratios = this->getAllElementsAspectRatios();
    FLOAT_TYPE mean_aspect_ratio = std::accumulate(all_aspect_ratios.begin(), all_aspect_ratios.end(), 0.0) / all_aspect_ratios.size();
    return mean_aspect_ratio;
  }

  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE FLOAT_TYPE Triangulation<FLOAT_TYPE>::evaluateElementQualityMean() const {
    std::vector<FLOAT_TYPE> all_qualities = this->getAllElementsElementQualities();
    FLOAT_TYPE mean_quality = std::accumulate(all_qualities.begin(), all_qualities.end(), 0.0) / all_qualities.size();
    return mean_quality;
  }

  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE FLOAT_TYPE Triangulation<FLOAT_TYPE>::evaluateSkewnessMean() const {
    std::vector<FLOAT_TYPE> all_skewness = this->getAllElementsSkewness();
    FLOAT_TYPE mean_skewness = std::accumulate(all_skewness.begin(), all_skewness.end(), 0.0) / all_skewness.size();
    return mean_skewness;
  }

  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE FLOAT_TYPE Triangulation<FLOAT_TYPE>::evaluateAspectRatioStandardDeviation() const {
    std::vector<FLOAT_TYPE> all_aspect_ratios = this->getAllElementsAspectRatios();
    FLOAT_TYPE mean_aspect_ratio = std::accumulate(all_aspect_ratios.begin(), all_aspect_ratios.end(), 0.0) / all_aspect_ratios.size();
    FLOAT_TYPE standard_deviation = 0.0;
    for (int i = 0; i < all_aspect_ratios.size(); i++) {
      standard_deviation += std::pow(all_aspect_ratios[i] - mean_aspect_ratio, 2.0);
    }
    return std::sqrt(standard_deviation / all_aspect_ratios.size());
  }

  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE FLOAT_TYPE Triangulation<FLOAT_TYPE>::evaluateElementQualityStandardDeviation() const {
    std::vector<FLOAT_TYPE> all_qualities = this->getAllElementsElementQualities();
    FLOAT_TYPE mean_quality = std::accumulate(all_qualities.begin(), all_qualities.end(), 0.0) / all_qualities.size();
    FLOAT_TYPE standard_deviation = 0.0;
    for (int i = 0; i < all_qualities.size(); i++) {
      standard_deviation += std::pow(all_qualities[i] - mean_quality, 2.0);
    }
    return std::sqrt(standard_deviation / all_qualities.size());
  }

  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE FLOAT_TYPE Triangulation<FLOAT_TYPE>::evaluateSkewnessStandardDeviation() const {
    std::vector<FLOAT_TYPE> all_skewness = this->getAllElementsSkewness();
    FLOAT_TYPE mean_skewness = std::accumulate(all_skewness.begin(), all_skewness.end(), 0.0) / all_skewness.size();
    FLOAT_TYPE standard_deviation = 0.0;
    for (int i = 0; i < all_skewness.size(); i++) {
      standard_deviation += std::pow(all_skewness[i] - mean_skewness, 2.0);
    }
    return std::sqrt(standard_deviation / all_skewness.size());
  }

  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE std::pair<FLOAT_TYPE,FLOAT_TYPE> Triangulation<FLOAT_TYPE>::evaluateAspectRatioMinMax() const {
    std::vector<FLOAT_TYPE> all_aspect_ratios = this->getAllElementsAspectRatios();
    auto minimum_element = all_aspect_ratios[std::distance(all_aspect_ratios.begin(), std::min_element(all_aspect_ratios.begin(), all_aspect_ratios.end()))];
    auto maximum_element = all_aspect_ratios[std::distance(all_aspect_ratios.begin(), std::max_element(all_aspect_ratios.begin(), all_aspect_ratios.end()))];
    return std::pair<FLOAT_TYPE, FLOAT_TYPE>({minimum_element, maximum_element});
  }

  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE std::pair<FLOAT_TYPE,FLOAT_TYPE> Triangulation<FLOAT_TYPE>::evaluateElementQualityMinMax() const {
    std::vector<FLOAT_TYPE> all_qualities = this->getAllElementsElementQualities();
    auto minimum_element = all_qualities[std::distance(all_qualities.begin(), std::min_element(all_qualities.begin(), all_qualities.end()))];
    auto maximum_element = all_qualities[std::distance(all_qualities.begin(), std::max_element(all_qualities.begin(), all_qualities.end()))];
    return std::pair<FLOAT_TYPE, FLOAT_TYPE>({minimum_element, maximum_element});
  }

  template <typename FLOAT_TYPE>
  OVF_HOST_DEVICE std::pair<FLOAT_TYPE,FLOAT_TYPE> Triangulation<FLOAT_TYPE>::evaluateSkewnessMinMax() const {
    std::vector<FLOAT_TYPE> all_skewness = this->getAllElementsSkewness();
    auto minimum_element = all_skewness[std::distance(all_skewness.begin(), std::min_element(all_skewness.begin(), all_skewness.end()))];
    auto maximum_element = all_skewness[std::distance(all_skewness.begin(), std::max_element(all_skewness.begin(), all_skewness.end()))];
    return std::pair<FLOAT_TYPE, FLOAT_TYPE>({minimum_element, maximum_element});
  }

template class Triangulation<float>;
template class Triangulation<double>;
}