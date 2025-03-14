#include "all_headers.hpp"

#pragma once

//! ----- GEOMETRY DEFINITIONS ----- !//

namespace geometry {

//* Vectors
template <typename T> class v3 {
  public:
  T _x, _y, _z;
  v3<T>() : _x(0.0), _y(0.0), _z(0.0) {}
  v3<T>(T x, T y, T z) : _x(x), _y(y), _z(z) {}
  T operator[](unsigned int i) const {
    switch (i % 3) {
      case 0:
        return _x;
      case 1:
        return _y;
      case 2:
        return _z;
    }
    throw std::runtime_error("Cannot access that vector element");
  }
  bool operator==(v3<T> rhs) const {
    return (_x == rhs._x && _y == rhs._y && _z == rhs._z);
  }
  bool operator!=(v3<T> rhs) const {
    return !((*this) == rhs);
  }
  v3<T> operator+(v3<T> rhs) const {
    return v3<T>(_x + rhs._x, _y + rhs._y, _z + rhs._z);
  }
  v3<T> operator-(v3<T> rhs) const {
    return v3<T>(_x - rhs._x, _y - rhs._y, _z - rhs._z);
  }
};

template <typename T> T magnitude(v3<T> v) {
  return (std::sqrt( v[0]*v[0] + v[1]*v[1] + v[2]*v[2] ));
}
template <typename T> v3<T> scale(v3<T> v, T scale) {
  return v3<T>( v[0] * scale, v[1] * scale, v[2] * scale );
}
template <typename T> v3<T> normalize(v3<T> v) {
  return scale(v, (T)1.0 / magnitude(v));
}
template <typename T> v3<T> flip(v3<T> v) {
  return scale(v, (T)-1.0);
}
template <typename T> T dot(v3<T> a, v3<T> b) {
  return ( (a[0] * b[0]) + (a[1] * b[1]) + (a[2] * b[2]) );
}
template <typename T> v3<T> cross(v3<T> a, v3<T> b) {
  T x_component = a[1]*b[2] - a[2]*b[1];
  T y_component = -( a[0]*b[2] - a[2]*b[0] );
  T z_component = a[0]*b[1] - a[1]*b[0];
  return v3<T>( x_component, y_component, z_component );
}
template <typename T> unsigned int getLongestDirection(v3<T> v) {
  if (std::abs(v[1]) >= std::abs(v[0]) && std::abs(v[1]) >= std::abs(v[2])) { return 1; }
  if (std::abs(v[2]) >= std::abs(v[0]) && std::abs(v[2]) >= std::abs(v[1])) { return 2; }
  return 0;
}

template <typename T> v3<T> vectorElementsMinima(v3<T> a, v3<T> b) {
  return v3<T>( std::min(a[0],b[0]), std::min(a[1],b[1]), std::min(a[2],b[2]) );
}
template <typename T> v3<T> vectorElementsMaxima(v3<T> a, v3<T> b) {
  return v3<T>( std::max(a[0],b[0]), std::max(a[1],b[1]), std::max(a[2],b[2]) );
}



//* ray
template <typename T> class ray {
  public:
  v3<T> _O, _D, _invD;
  T _t;

  ray() : _O(v3<T>()), _D(v3<T>()), _invD(v3<T>()), _t(INFINITY) {}
  ray(v3<T> O, v3<T> D) : _O(O), _D(D), _invD(v3<T>(1.0 / D[0], 1.0 / D[1], 1.0 / D[2])), _t(INFINITY) {}
};



//* tri
template <typename T> class tri {
  public:
  std::array<T,9> _p;
  tri() : _p(std::array<T,9>()) {}
  tri(std::array<T,9> p) : _p(p) {}

  v3<T> operator[](unsigned int i) const {
    unsigned int p_i = i % 3;
    return v3<T>(_p[3*p_i + 0], _p[3*p_i + 1], _p[3*p_i + 2]);
  }
};

template <typename T> v3<T> centroid(tri<T> t) {
  v3<T> centroid;
  for (int point = 0; point < 3; point++) {
    centroid = centroid + t[point];
  }
  return (scale(centroid, (T)(1.0/3.0)));
}

template <typename T> v3<T> normal(tri<T> t) {
  return ( normalize( cross(t[1] - t[0], t[2] - t[0]) ) );
}

template <typename T> T area(tri<T> t) {
  return (magnitude( cross(t[1] - t[0], t[2] - t[0]) )/2.0);
}

template <typename T> T triSkewness(tri<T> t) {
  v3<T> OA = t[0];
  v3<T> OB = t[1];
  v3<T> OC = t[2];
  T angle_OA = std::acos( dot( OB - OA, OC - OA ) / ( magnitude(OB - OA) * magnitude(OC - OA) ) );
  T circumscribed_diameter = magnitude( OC - OB ) / std::sin(angle_OA);
  T equilateral_side_length = circumscribed_diameter * std::sqrt(3.0) * 0.5;
  T equilateral_area = 0.5 * (equilateral_side_length * equilateral_side_length*0.5*1.732050807568877);
  return ( (equilateral_area - area(t)) / equilateral_area );
}

template <typename T> T triElementQuality(tri<T> t) {
  T triangle_constant = 6.92820323;
  v3<T> OA = t[0];
  v3<T> OB = t[1];
  v3<T> OC = t[2];
  T L_AB = magnitude(OB - OA);
  T L_BC = magnitude(OC - OB);
  T L_CA = magnitude(OA - OC);
  T edge_length_square_sum = L_AB*L_AB + L_BC*L_BC + L_CA*L_CA;
  return ( triangle_constant * area(t) / edge_length_square_sum );
}

template <typename T> T triAspectRatio(tri<T> t) {
  T aspect_ratio = 0.0;
  for (int i = 0; i < 3; i++) {
    v3<T> p1 = t[i];
    v3<T> p2 = t[i+1];
    v3<T> p3 = t[i+2];
    v3<T> p1_bisector = ( p2 + scale(p3 - p2, (T)0.5) - p1 );
    v3<T> p1_bisector_dir = normalize(p1_bisector);
    v3<T> adjacent_edges_midpoint_span = ( scale(p2 - p1, (T)0.5) ) - ( scale(p3 - p1, (T)0.5) );
    if ( dot(normalize(adjacent_edges_midpoint_span), p1_bisector_dir) < 0.0 ) {
      p1_bisector_dir = flip(p1_bisector_dir);
    }
    T rect_1_L1 = magnitude(p1_bisector);
    T rect_1_L2 = magnitude( adjacent_edges_midpoint_span - scale(p1_bisector_dir, dot(adjacent_edges_midpoint_span, p1_bisector_dir)) );
    T bisector_aspect_ratio = std::max( rect_1_L1, rect_1_L2 ) / std::min( rect_1_L1, rect_1_L2 );

    v3<T> opposing_edge_dir = normalize( p3 - p2 );
    if ( dot(p1_bisector_dir, opposing_edge_dir) < 0.0 ) {
      opposing_edge_dir = flip(opposing_edge_dir);
    }
    T rect_2_L1 = magnitude(adjacent_edges_midpoint_span);
    T rect_2_L2 = magnitude( p1_bisector - scale(opposing_edge_dir, dot(p1_bisector, opposing_edge_dir)) );
    T span_aspect_ratio = std::max( rect_2_L1, rect_2_L2 ) / std::min( rect_2_L1, rect_2_L2 );
    
    aspect_ratio = std::max({ aspect_ratio, bisector_aspect_ratio, span_aspect_ratio });
  }
  return (aspect_ratio / std::sqrt(3.0));
}



//* Mesh
template <typename T> class mesh {
  public:
  std::vector<T> _p;
  std::vector<size_t> _c;
  mesh() : _p(std::vector<T>()), _c(std::vector<size_t>()) {}
  mesh(size_t num_triangles) : _p(std::vector<T>(num_triangles*9)), _c(std::vector<size_t>(num_triangles*3)) {}
  mesh(std::vector<T> p, std::vector<size_t> c) {
    _p = p;
    _c = c;
  }
  
  tri<T> operator[](unsigned int i) const {
    std::array<T,9> points;
    for (int j = 0; j < 9; j++) {
      points[j] = _p[ 3 * ( _c[3*i + (j/3)] ) + (j%3) ];
    }
    tri<T> t(points);
    return t;
  }

  mesh<T> operator[](std::vector<unsigned int> indices) const {
    mesh<T> sub_mesh;
    std::vector<tri<T>> triangles(indices.size());
    for (int i = 0; i < indices.size(); i++) {
      triangles[i] = (*this)[indices[i]];
    }
    sub_mesh + triangles;
    return sub_mesh;
  }

  mesh<T>& operator+(tri<T> t) {
    unsigned int pre_add_size = _p.size() / 3;
    for (int i = 0; i < 9; i++) {
      _p.push_back( (t[i / 3])[i % 3] );
    }
    for (int i = 0; i < 3; i++) {
      _c.push_back( pre_add_size + i );
    }
    return *this;
  }

  mesh<T>& operator+(std::vector<tri<T>> triangles) {
    for (auto el : triangles) {
      (*this)+el;
    }
    return *this;
  }

  mesh<T>& operator+(mesh<T>* m) {
    for (int i = 0; i < m->size(); i++) {
      (*this) + (*m)[i];
    }
    return *this;
  }

  unsigned int size() const {
    return (_c.size() / 3);
  }
};

template <typename T> std::vector<v3<T>> centroids(mesh<T>* m) {
  std::vector<v3<T>> centroids(m->size());
  for (int triangle = 0; triangle < m->size(); triangle++) {
    centroids[triangle] = centroid((*m)[triangle]);
  }
  return centroids;
}

template <typename T> std::vector<v3<T>> normals(mesh<T>* m) {
  std::vector<v3<T>> normals(m->size());
  for (int triangle = 0; triangle < m->size(); triangle++) {
    normals[triangle] = normal( (*m)[triangle] );
  }
  return normals;
}

template <typename T> std::vector<T> areas(mesh<T>* m) {
  std::vector<T> all_areas(m->size());
  for (int triangle = 0; triangle < m->size(); triangle++) {
    all_areas[triangle] = area((*m)[triangle]);
  }
  return all_areas;
}

template <typename T> std::vector<tri<T>> allTriangles(mesh<T>* m) {
  std::vector<tri<T>> triangles(m->size());
  for (int triangle = 0; triangle < m->size(); triangle++) {
    triangles[triangle] = (*m)[triangle];
  }
  return triangles;
}

template <typename T> std::vector<T> meshSkewness(mesh<T>* m) {
  std::vector<T> skewnesses(m->size());
  for (int triangle = 0; triangle < m->size(); triangle++) {
    skewnesses[triangle] = triSkewness((*m)[triangle]);
  }
  return skewnesses;
}

template <typename T> std::vector<T> meshElementQuality(mesh<T>* m) {
  std::vector<T> element_qualities(m->size());
  for (int triangle = 0; triangle < m->size(); triangle++) {
    element_qualities[triangle] = triElementQuality((*m)[triangle]);
  }
  return element_qualities;
}

template <typename T> std::vector<T> meshAspectRatio(mesh<T>* m) {
  std::vector<T> aspect_ratio(m->size());
  for (int triangle = 0; triangle < m->size(); triangle++) {
    aspect_ratio[triangle] = triAspectRatio((*m)[triangle]);
  }
  return aspect_ratio;
}

template <typename T> mesh<T> removeDegenerateElements(mesh<T>* m) {
  std::vector<unsigned int> good_elements(m->size());
  std::iota(good_elements.begin(), good_elements.end(), 0);
  for (int i = 0; i < m->size(); i++) {
    if ( std::isinf(triAspectRatio((*m)[i])) || std::isnan(triSkewness((*m)[i])) ) {
      good_elements[i] = m->size();
    }
  }
  auto it = std::remove(good_elements.begin(), good_elements.end(), m->size());

  std::cout << "[TEMPORARY] Removing " << std::distance(it, good_elements.end()) << " degenerate elements\n";

  good_elements.erase(it, good_elements.end());
  mesh<T> new_mesh = (*m)[good_elements];
  return new_mesh;
}

template <typename T> mesh<T> getMesh(const std::string& filename) {
  std::vector<T> coordinates, normals;
  std::vector<size_t> triangulations, solids;
  stl_reader::ReadStlFile(filename.c_str(), coordinates, normals, triangulations, solids);
  mesh<T> m(coordinates, triangulations);
  m = removeDegenerateElements(&m);
  return m;
}



//* BVHNode
template <typename T> class BVHNode {
  public:
  v3<T> _bbmin, _bbmax;
  unsigned int _N_tri, _left_or_first;

  BVHNode() : _bbmin(v3<T>(INFINITY, INFINITY, INFINITY)), _bbmax(v3<T>(-INFINITY, -INFINITY, -INFINITY)), _N_tri(0), _left_or_first(0) {}

  v3<T> min() const { return _bbmin; }
  v3<T> max() const { return _bbmax; }
  v3<T> span() const { return (max() - min()); }
  unsigned int numTri() const { return _N_tri; }
  bool isLeaf() const {
    return (this->numTri() != 0);
  }
  unsigned int childIndex() const {
    if (isLeaf()) {
      throw std::runtime_error("Leaf node does not have a child");
    } else {
      return _left_or_first;
    }
  }
  unsigned int firstTriangleIndex() const {
    if (isLeaf()) {
      return _left_or_first;
    } else {
      throw std::runtime_error("Non-leaf node does not have triangles");
    }
  }
  BVHNode<T>& grow(tri<T> t) {
    _bbmin = vectorElementsMinima(_bbmin, t[0]);
    _bbmin = vectorElementsMinima(_bbmin, t[1]);
    _bbmin = vectorElementsMinima(_bbmin, t[2]);
    _bbmax = vectorElementsMaxima(_bbmax, t[0]);
    _bbmax = vectorElementsMaxima(_bbmax, t[1]);
    _bbmax = vectorElementsMaxima(_bbmax, t[2]);
    return *this;
  }
  BVHNode<T>& grow(std::vector<tri<T>> t) {
    for (auto el : t) { 
      this->grow(el);
    }
    return *this;
  }
  BVHNode<T>& grow(mesh<T>* m) {
    std::vector<tri<T>> t = allTriangles(m);
    this->grow(t);
    return *this;
  }
};

template <typename T> T surfaceArea(BVHNode<T>* b) {
  v3<T> span = b->span();
  return ( std::abs(span[0]*span[1]) + std::abs(span[0]*span[2]) + std::abs(span[1]*span[2]) );
}

template <typename T> T cost(BVHNode<T>* b) {
  return surfaceArea(b) * b->numTri();
}

template <typename T> unsigned int bestSplitAxis(BVHNode<T>* b) {
  return getLongestDirection(b->span());
}

template <typename T> T surfaceAreaHeuristic(BVHNode<T>* b, mesh<T>* m, unsigned int axis, T candidate_position) {
  BVHNode<T> left;
  BVHNode<T> right;
  unsigned int mesh_size = m->size();
  std::vector<unsigned int> left_i(mesh_size), right_i(mesh_size);
  std::iota(left_i.begin(), left_i.end(), 0);
  std::iota(right_i.begin(), right_i.end(), 0);
  std::vector<v3<T>> mesh_centroids = centroids(m);

  for (int i = 0; i < b->numTri(); i++) {
    if (mesh_centroids[i][axis] <= candidate_position) {
      right_i[i] = mesh_size;
    } else {
      left_i[i] = mesh_size;
    }
  }

  auto left_it = std::remove(left_i.begin(), left_i.end(), mesh_size);
  left_i.erase(left_it, left_i.end());
  mesh<T> left_mesh = (*m)[left_i];
  left.grow(&left_mesh);
  left._N_tri = left_i.size();

  auto right_it = std::remove(right_i.begin(), right_i.end(), mesh_size);
  right_i.erase(right_it, right_i.end());
  mesh<T> right_mesh = (*m)[right_i];
  right.grow(&right_mesh);
  right._N_tri = right_i.size();

  T left_cost = cost(&left);
  T right_cost = cost(&right);
  T split_cost = 0.0;
  if ( !(std::isnan(left_cost)) ) { split_cost += left_cost; }
  if ( !(std::isnan(right_cost)) ) { split_cost += right_cost; }
  if (split_cost == 0.0) { split_cost = INFINITY; }
  return split_cost;
}

template <typename T> mesh<T> nodeSubmesh(BVHNode<T>* b, mesh<T>* m, std::vector<unsigned int>* tri_indices) {
  std::vector<unsigned int> submesh_indices(b->numTri());
  std::iota(submesh_indices.begin(), submesh_indices.end(), b->firstTriangleIndex());
  for (int i = 0; i < b->numTri(); i++) {
    submesh_indices[i] = (*tri_indices)[submesh_indices[i]];
  }
  mesh<T> submesh = (*m)[submesh_indices];
  return submesh;
}

template <typename T> std::pair<T,T> bestSplit(BVHNode<T>* b, mesh<T>* m, unsigned int axis, unsigned int num_evals, std::vector<unsigned int>* tri_indices) {
  mesh<T> submesh = nodeSubmesh(b, m, tri_indices);

  T axis_length = b->span()[axis];
  T axis_min = b->min()[axis];

  std::vector<T> eval_costs(num_evals); std::vector<T> eval_pos(num_evals);
  for (int i = 0; i < num_evals; i++) {
    T split_pos = axis_min + ( axis_length * (T)(i+1) / ( (T)(num_evals) + (T)(1) ) );
    eval_pos[i] = split_pos;

    T split_cost = surfaceAreaHeuristic(b, &submesh, axis, split_pos);
    eval_costs[i] = split_cost;
  }

  auto best_cost_it = std::min_element(eval_costs.begin(), eval_costs.end(), [] (const T& x, const T& y) {
    if (std::isnan(x)) return false;
    if (std::isnan(y)) return true;
    return (x < y);
  });
  T best_cost = *best_cost_it;
  unsigned int best_cost_index = std::distance(eval_costs.begin(), best_cost_it);
  
  std::pair<T,T> split_pos_cost;
  split_pos_cost.first = eval_pos[best_cost_index];
  split_pos_cost.second = best_cost;
  return split_pos_cost;
}




//* bvh
template <typename T>  class BVH {
  public:
  std::vector<BVHNode<T>> _nodes;
  std::vector<unsigned int> _tri_indices;
  unsigned int _nodes_used;

  BVH() {}
  BVH(mesh<T>* m) {
    if (m->size() > 0) {
      std::vector<BVHNode<T>> nodes;
      for (int i = 0; i < 2*m->size() - 1; i++) {
        BVHNode<T> new_node;
        nodes.push_back(new_node);
      }
      _nodes = nodes;

      std::vector<unsigned int> tri_indices(m->size());
      std::iota(tri_indices.begin(), tri_indices.end(), 0);
      _tri_indices = tri_indices;

      _nodes_used = 0;
    }
  }

  BVHNode<T>* operator[](unsigned int i) { return &(_nodes[i]); }

  BVH<T>& swapElements(unsigned int i1, unsigned int i2) {
    unsigned int temp = _tri_indices[i1];
    _tri_indices[i1] = _tri_indices[i2];
    _tri_indices[i2] = temp;
    return *this;
  }
};

template <typename T> void constructNewNode(BVH<T>* bvh, mesh<T>* m, unsigned int node_i) {
  BVHNode<T>* node = (*bvh)[node_i];
  mesh<T> submesh = nodeSubmesh(node, m, &(bvh->_tri_indices));
  node->grow(&submesh);
}

template <typename T> unsigned int createChildNodes(BVH<T>* bvh, mesh<T>* m, unsigned int node_i, unsigned int split_index, unsigned int num_left_tri) {
  unsigned int left_child_index = bvh->_nodes_used;
  (bvh->_nodes_used) += 2;
  
  BVHNode<T>* node = (*bvh)[node_i];

  BVHNode<T>* left_child = (*bvh)[left_child_index];
  left_child->_left_or_first = node->_left_or_first;
  left_child->_N_tri = num_left_tri;

  BVHNode<T>* right_child = (*bvh)[left_child_index + 1];
  right_child->_left_or_first = split_index;
  right_child->_N_tri = node->numTri() - num_left_tri;

  node->_left_or_first = left_child_index;
  node->_N_tri = 0;

  constructNewNode(bvh, m, left_child_index);
  constructNewNode(bvh, m, left_child_index + 1);

  return left_child_index;
}

template <typename T> unsigned int splitPrimitives(BVH<T>* bvh, mesh<T>* m, unsigned int node_i, unsigned int axis, T loc) {
  BVHNode<T>* node = (*bvh)[node_i];
  unsigned int split_index = node->firstTriangleIndex();
  unsigned int unsorted_index = split_index + node->numTri() - 1;

  while (split_index <= unsorted_index) {
    unsigned int element_index = bvh->_tri_indices[split_index];
    tri<T> current_element = (*m)[element_index];
    if (centroid(current_element)[axis] < loc) {
      split_index++;
    } else {
      bvh->swapElements(split_index, unsorted_index--);
    }
  }
  return split_index;
}

template <typename T> unsigned int subdivideNode(BVH<T>* bvh, mesh<T>* m, unsigned int node_i) {
  BVHNode<T>* node = (*bvh)[node_i];
  if (node->numTri() <= 20) { return 0; }

  unsigned int axis = bestSplitAxis(node);
  std::pair<T,T> split_pos_cost = bestSplit(node, m, axis, 20, &(bvh->_tri_indices));
  if (cost(node) < split_pos_cost.second) { return 1; }

  unsigned int split_index = splitPrimitives(bvh, m, node_i, axis, split_pos_cost.first);

  unsigned int num_left_tri = split_index - node->firstTriangleIndex();
  if (num_left_tri == 0 || num_left_tri == node->numTri()) { return 2; }

  unsigned int left_child_index = createChildNodes(bvh, m, node_i, split_index, num_left_tri);
  subdivideNode(bvh, m, left_child_index);
  subdivideNode(bvh, m, left_child_index + 1);

  return 3;
}

template <typename T> void constructBVH(BVH<T>* bvh, mesh<T>* m) {
  unsigned int root_i = 0;
  BVHNode<T>* root_node = (*bvh)[root_i];
  root_node->_N_tri = m->size();
  root_node->grow(m);
  bvh->_nodes_used = 1;
  subdivideNode(bvh, m, root_i);
}


}