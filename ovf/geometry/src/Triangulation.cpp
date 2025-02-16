#include "Vector3.hpp"
#include "Triangle.hpp"
#include "Triangulation.hpp"

#include <iostream>

namespace openviewfactor {
  //* ----- CLASS CONSTRUCTORS ----- *//
  template <typename t>
  gpuify Triangulation<t>::Triangulation()
    : _pts(std::vector<Vector3<t>>()), _con(std::vector<std::array<size_t, 3>>()) {}

  //* ----- ACCESSOR METHODS ----- *//
  template <typename t>
  gpuify size_t Triangulation<t>::getNumElements() const { return _con.size(); }
  template <typename t>
  gpuify size_t Triangulation<t>::getNumPoints() const { return _pts.size(); }
  template <typename t>
  gpuify size_t Triangulation<t>::getNumBytes() const { return (_con.size() * sizeof(size_t) * 3) + (_pts.size() * sizeof(Vector3<t>)); }

  template <typename t>
  gpuify const std::array<size_t,3>* Triangulation<t>::getConPtr() const { return _con.data(); }

  template <typename t>
  gpuify const Vector3<t>* Triangulation<t>::getPtsPtr() const { return _pts.data(); }

  template <typename t>
  gpuify const Triangle<t> Triangulation<t>::operator[](size_t index) const {
    Triangle<t> T(_pts[ (_con[index])[0] ], _pts[ (_con[index])[1] ], _pts[ (_con[index])[2] ]);
    return T;
  }
  template <typename t>
  gpuify Triangle<t> Triangulation<t>::operator[](size_t index) {
    Triangle<t> T(_pts[ (_con[index])[0] ], _pts[ (_con[index])[1] ], _pts[ (_con[index])[2] ]);
    return T;
  }

  template <typename t>
  gpuify t Triangulation<t>::getMeshArea() const {
    t total_area = 0;
    for (size_t i = 0; i < this->getNumElements(); i++) {
      total_area += ((*this)[i]).getArea();
    }
    return total_area;
  }

  template <typename t>
  gpuify std::shared_ptr<Triangulation<t>> Triangulation<t>::getSubMesh(std::vector<unsigned int> indices) const {    
    auto sub_mesh = std::make_shared<Triangulation<t>>();
    std::vector<std::array<size_t, 3>> sub_mesh_connectivity(indices.size());
    std::generate(sub_mesh_connectivity.begin(), sub_mesh_connectivity.end(), [this, indices] (unsigned int i=0) mutable { return (this->_con)[indices[i++]]; }); 
    sub_mesh->setConnectivity(sub_mesh_connectivity);
    sub_mesh->setPoints(_pts);    
    return sub_mesh;
  }

  template <typename t>
  gpuify std::vector<t> Triangulation<t>::getAreas() const {
    std::vector<t> mesh_areas(this->getNumElements());
    for (int i = 0; i < getNumElements(); i++) {
      mesh_areas[i] = (*this)[i].getArea();
    }
    return mesh_areas;
  }

  template <typename t>
  gpuify std::vector<Vector3<t>> Triangulation<t>::getCentroids() const {
    std::vector<Vector3<t>> mesh_centroids(this->getNumElements());
    for (int i = 0; i < getNumElements(); i++) {
      mesh_centroids[i] = (*this)[i].getCentroid();
    }
    return mesh_centroids;
  }

  template <typename t>
  gpuify std::vector<Vector3<t>> Triangulation<t>::getNormals() const {
    std::vector<Vector3<t>> mesh_normals(this->getNumElements());
    for (int i = 0; i < getNumElements(); i++) {
      mesh_normals[i] = (*this)[i].getNormal();
    }
    return mesh_normals;
  }

  template <typename t>
  gpuify std::vector<Triangle<t>> Triangulation<t>::getTriangles() const {
    std::vector<Triangle<t>> mesh_triangles(this->getNumElements());
    for (unsigned int i = 0; i < this->getNumElements(); i++) {
      std::array<size_t, 3> connections = _con[i];
      mesh_triangles[i].setOA(_pts[connections[0]]).setOB(_pts[connections[1]]).setOC(_pts[connections[2]]);
    }
    return mesh_triangles;
  }

  //* ----- MUTATOR METHODS ----- *//
  template <typename t>
  gpuify Triangulation<t>& Triangulation<t>::clear() {
    _pts.clear();
    _con.clear();
    return *this;
  }
  template <typename t>
  gpuify Triangulation<t>& Triangulation<t>::addElement(Vector3<t> OA, Vector3<t> OB, Vector3<t> OC) {
    std::array<Vector3<t>, 3> new_points = {OA, OB, OC};
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
  template <typename t>
  gpuify Triangulation<t>& Triangulation<t>::addElement(Triangle<t> tri) {
    this->addElement(tri.getOA(), tri.getOB(), tri.getOC());
    return *this;
  }

  template <typename t>
  gpuify Triangulation<t>& Triangulation<t>::setConnectivity(std::vector<std::array<size_t, 3>> con) {
    _con.clear();
    _con = std::move(con);
    return *this;
  }
  template <typename t>
  gpuify Triangulation<t>& Triangulation<t>::setPoints(std::vector<Vector3<t>> pts) {
    _pts.clear();
    _pts = std::move(pts);
    return *this;
  }

  template <typename t>
  gpuify bool Triangulation<t>::operator==(const Triangulation<t>& rhs) const {
    if (this->getNumElements() != rhs.getNumElements()) { return false; }
    if (this->getNumPoints() != rhs.getNumPoints()) { return false; }
    if (this->getMeshArea() != rhs.getMeshArea()) { return false; }
    for (size_t i = 0; i < this->getNumElements(); i++) {
      if ((*this)[i] != rhs[i]) { return false; }
    }
    return true;
  }


  //* ----- MESH QUALITY ANALYSIS ----- *//
  template <typename t>
  gpuify std::vector<t> Triangulation<t>::getAllElementsAspectRatios() const {
    auto all_triangles = this->getTriangles();
    std::vector<t> aspect_ratios(all_triangles.size());
    for (int i = 0; i < all_triangles.size(); i++) {
      aspect_ratios[i] = all_triangles[i].evaluateAspectRatio();
    }
    return aspect_ratios;
  }

  template <typename t>
  gpuify std::vector<t> Triangulation<t>::getAllElementsElementQualities() const {
    auto all_triangles = this->getTriangles();
    std::vector<t> qualities(all_triangles.size());
    for (int i = 0; i < all_triangles.size(); i++) {
      qualities[i] = all_triangles[i].evaluateElementQuality();
    }
    return qualities;
  }

  template <typename t>
  gpuify std::vector<t> Triangulation<t>::getAllElementsSkewness() const {
    auto all_triangles = this->getTriangles();
    std::vector<t> skewness(all_triangles.size());
    for (int i = 0; i < all_triangles.size(); i++) {
      skewness[i] = all_triangles[i].evaluateSkewness();
    }
    return skewness;
  }

  template <typename t>
  gpuify t Triangulation<t>::evaluateAspectRatioMean() const {
    std::vector<t> all_aspect_ratios = this->getAllElementsAspectRatios();
    t mean_aspect_ratio = std::accumulate(all_aspect_ratios.begin(), all_aspect_ratios.end(), 0.0) / all_aspect_ratios.size();
    return mean_aspect_ratio;
  }

  template <typename t>
  gpuify t Triangulation<t>::evaluateElementQualityMean() const {
    std::vector<t> all_qualities = this->getAllElementsElementQualities();
    t mean_quality = std::accumulate(all_qualities.begin(), all_qualities.end(), 0.0) / all_qualities.size();
    return mean_quality;
  }

  template <typename t>
  gpuify t Triangulation<t>::evaluateSkewnessMean() const {
    std::vector<t> all_skewness = this->getAllElementsSkewness();
    t mean_skewness = std::accumulate(all_skewness.begin(), all_skewness.end(), 0.0) / all_skewness.size();
    return mean_skewness;
  }

  template <typename t>
  gpuify t Triangulation<t>::evaluateAspectRatioStandardDeviation() const {
    std::vector<t> all_aspect_ratios = this->getAllElementsAspectRatios();
    t mean_aspect_ratio = std::accumulate(all_aspect_ratios.begin(), all_aspect_ratios.end(), 0.0) / all_aspect_ratios.size();
    t standard_deviation = 0.0;
    for (int i = 0; i < all_aspect_ratios.size(); i++) {
      standard_deviation += std::pow(all_aspect_ratios[i] - mean_aspect_ratio, 2.0);
    }
    return std::sqrt(standard_deviation / all_aspect_ratios.size());
  }

  template <typename t>
  gpuify t Triangulation<t>::evaluateElementQualityStandardDeviation() const {
    std::vector<t> all_qualities = this->getAllElementsElementQualities();
    t mean_quality = std::accumulate(all_qualities.begin(), all_qualities.end(), 0.0) / all_qualities.size();
    t standard_deviation = 0.0;
    for (int i = 0; i < all_qualities.size(); i++) {
      standard_deviation += std::pow(all_qualities[i] - mean_quality, 2.0);
    }
    return std::sqrt(standard_deviation / all_qualities.size());
  }

  template <typename t>
  gpuify t Triangulation<t>::evaluateSkewnessStandardDeviation() const {
    std::vector<t> all_skewness = this->getAllElementsSkewness();
    t mean_skewness = std::accumulate(all_skewness.begin(), all_skewness.end(), 0.0) / all_skewness.size();
    t standard_deviation = 0.0;
    for (int i = 0; i < all_skewness.size(); i++) {
      standard_deviation += std::pow(all_skewness[i] - mean_skewness, 2.0);
    }
    return std::sqrt(standard_deviation / all_skewness.size());
  }

  template <typename t>
  gpuify std::pair<t,t> Triangulation<t>::evaluateAspectRatioMinMax() const {
    std::vector<t> all_aspect_ratios = this->getAllElementsAspectRatios();
    auto minimum_element = all_aspect_ratios[std::distance(all_aspect_ratios.begin(), std::min_element(all_aspect_ratios.begin(), all_aspect_ratios.end()))];
    auto maximum_element = all_aspect_ratios[std::distance(all_aspect_ratios.begin(), std::max_element(all_aspect_ratios.begin(), all_aspect_ratios.end()))];
    return std::pair<t, t>({minimum_element, maximum_element});
  }

  template <typename t>
  gpuify std::pair<t,t> Triangulation<t>::evaluateElementQualityMinMax() const {
    std::vector<t> all_qualities = this->getAllElementsElementQualities();
    auto minimum_element = all_qualities[std::distance(all_qualities.begin(), std::min_element(all_qualities.begin(), all_qualities.end()))];
    auto maximum_element = all_qualities[std::distance(all_qualities.begin(), std::max_element(all_qualities.begin(), all_qualities.end()))];
    return std::pair<t, t>({minimum_element, maximum_element});
  }

  template <typename t>
  gpuify std::pair<t,t> Triangulation<t>::evaluateSkewnessMinMax() const {
    std::vector<t> all_skewness = this->getAllElementsSkewness();
    auto minimum_element = all_skewness[std::distance(all_skewness.begin(), std::min_element(all_skewness.begin(), all_skewness.end()))];
    auto maximum_element = all_skewness[std::distance(all_skewness.begin(), std::max_element(all_skewness.begin(), all_skewness.end()))];
    return std::pair<t, t>({minimum_element, maximum_element});
  }

template class Triangulation<float>;
template class Triangulation<double>;
}