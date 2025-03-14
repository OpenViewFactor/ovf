#include "all_headers.hpp"

#include "geometry.hpp"

#pragma once

//! ----- SOLVER DEFINITIONS ----- !//

namespace solver {

namespace geo = geometry;

template <typename T> bool backFaceCullElements(geo::v3<T> e_centroid, geo::v3<T> e_normal, geo::v3<T> r_centroid, geo::v3<T> r_normal) {
  geo::v3<T> ray = geo::normalize( r_centroid - e_centroid );
  bool emitter_culled = geo::dot( ray, e_normal ) <= 0.0;
  bool receiver_culled = geo::dot( ray, r_normal ) >= 0.0;
  return ( emitter_culled || receiver_culled );
}

template <typename T> void backFaceCullMeshes(std::vector<geo::v3<T>>* e_centroids, std::vector<geo::v3<T>>* e_normals, std::vector<geo::v3<T>>* r_centroids, std::vector<geo::v3<T>>* r_normals, std::vector<std::vector<unsigned int>*>* unculled_indices) {
  unsigned int N_e = e_centroids->size();
  unsigned int N_r = r_centroids->size();
  unsigned int problem_size = N_e * N_r;

  for ( auto sub_indices : (*unculled_indices) ) {
    std::iota(sub_indices->begin(), sub_indices->end(), 0);
  }

  for (int e = 0; e < N_e; e++) {
    std::vector<unsigned int>* sub_indices = (*unculled_indices)[e];
    #pragma omp parallel for
    for (int r = 0; r < N_r; r++) {
      bool culled = backFaceCullElements( (*e_centroids)[e], (*e_normals)[e], (*r_centroids)[r], (*r_normals)[r] );
      if (culled) {
        (*sub_indices)[ r ] = problem_size;
      }
    }
    auto it = std::remove(sub_indices->begin(), sub_indices->end(), problem_size);
    sub_indices->erase(it, sub_indices->end());
  }
}




template <typename T> geo::ray<T> intersectRayWithTri(geo::ray<T>* r, geo::tri<T> t) {
  geo::v3<T> OA = t[0];
  geo::v3<T> OB = t[1];
  geo::v3<T> OC = t[2];
  geo::v3<T> E1 = OB - OA;
  geo::v3<T> E2 = OC - OA;

  geo::v3<T> T_vec = r->_O - OA;
  geo::v3<T> P_vec = geo::cross(r->_D, E2);
  geo::v3<T> Q_vec = geo::cross(T_vec, E1);

  T scalar = 1.0 / geo::dot(P_vec, E1);

  T u = geo::dot(P_vec, T_vec) * scalar;
  if ( u < -0.001 || u > 1.001 ) { return *r; }

  T v = geo::dot(Q_vec, r->_D) * scalar;
  if ( v < -0.001 || v > 1.001 ) { return *r; }

  T intersection = geo::dot(Q_vec, E2) * scalar;
  if (intersection > 0.0 && intersection < r->_t) { r->_t = intersection; }
  return *r;
}


template <typename T> void naiveBlockingBetweenMeshes(geo::mesh<T>* o, std::vector<geo::v3<T>>* e_centroids, std::vector<geo::tri<T>>* r_triangles, std::vector<std::vector<unsigned int>*>* unculled_indices) {
  unsigned int problem_size = e_centroids->size() * r_triangles->size();

  for (int e = 0; e < unculled_indices->size(); e++) {
    std::vector<unsigned int>* sub_indices = (*unculled_indices)[e];
    
    #pragma omp parallel for
    for (int i = 0; i < sub_indices->size(); i++) {
      unsigned int r = (*sub_indices)[i];

      geo::v3<T> e_centroid = (*e_centroids)[e];
      geo::tri<T> r_tri = (*r_triangles)[r];
      geo::v3<T> r_centroid = geo::centroid(r_tri);

      geo::v3<T> ray_vector = r_centroid - e_centroid;
      T ray_length = geo::magnitude(ray_vector);
      geo::ray<T> cast_ray( e_centroid, geo::normalize(ray_vector) );

      for (int j = 0; j < o->size(); j++) {
        intersectRayWithTri(&cast_ray, (*o)[j]);
        bool blocked = ( cast_ray._t < ray_length );
        if (blocked) {
          (*sub_indices)[i] = problem_size;
          break;
        }
      }
    }
    auto it = std::remove(sub_indices->begin(), sub_indices->end(), problem_size);
    sub_indices->erase(it, sub_indices->end());
  }
}



template <typename T> T intersectRayWithNode(geo::ray<T>* r, geo::BVHNode<T>* b) {
  T tx1 = (b->min()[0] - r->_O[0]) * r->_invD[0];
  T tx2 = (b->max()[0] - r->_O[0]) * r->_invD[0];

  T tmin = std::min(tx1, tx2);
  T tmax = std::max(tx1, tx2);

  T ty1 = (b->min()[1] - r->_O[1]) * r->_invD[1];
  T ty2 = (b->max()[1] - r->_O[1]) * r->_invD[1];

  tmin = std::max(tmin, std::min(ty1, ty2));
  tmax = std::min(tmax, std::max(ty1, ty2));

  T tz1 = (b->min()[2] - r->_O[2]) * r->_invD[2];
  T tz2 = (b->max()[2] - r->_O[2]) * r->_invD[2];

  tmin = std::max(tmin, std::min(tz1, tz2));
  tmax = std::min(tmax, std::max(tz1, tz2));

  if (tmax >= tmin && tmin < r->_t && tmax > 0.0) return tmin; else return INFINITY;
}

template <typename T> void intersectRayWithBVH(geo::ray<T>* r, geo::BVH<T> bvh, geo::mesh<T>* m, T triangle_distance) {
  geo::BVHNode<T>* node = bvh[0];
  std::vector<geo::BVHNode<T>*> stack(bvh._nodes_used);
  unsigned int stack_pointer = 0;
  
  while (1) {
    if (node->isLeaf()) {
      for (int i = 0; i < node->numTri(); i++) {
        const geo::tri<T>& triangle = (*m)[ (bvh._tri_indices)[ (int)(node->firstTriangleIndex()) + i ] ];
        intersectRayWithTri(r, triangle);
        if (r->_t < triangle_distance && r->_t > 0.0) {
          return;
        }
      }
      if (stack_pointer == 0) {
        break;
      } else {
        node = stack[--stack_pointer];
      }
      continue;
    }
    unsigned int child_index = node->childIndex();
    geo::BVHNode<T>* child_one = bvh[child_index];
    geo::BVHNode<T>* child_two = bvh[child_index + 1];

    T distance_one = intersectRayWithNode(r, child_one);
    T distance_two = intersectRayWithNode(r, child_two);

    if (distance_one > distance_two) {
      std::swap(distance_one, distance_two);
      std::swap(child_one, child_two);
    }
    if (distance_one == INFINITY) {
      if (stack_pointer == 0) {
        break;
      } else {
        node = stack[--stack_pointer];
      }
    } else {
      node = child_one;
      if (distance_two != INFINITY) {
          stack[stack_pointer++] = child_two;
      }
    }
  }
}



template <typename T> void bvhBlockingBetweenMeshes(geo::BVH<T>* bvh, geo::mesh<T>* blocking_mesh, std::vector<geo::v3<T>>* e_centroids, std::vector<geo::tri<T>>* r_triangles, std::vector<std::vector<unsigned int>*>* unculled_indices) {
  unsigned int problem_size = e_centroids->size() * r_triangles->size();

  #pragma omp parallel for
  for (int e = 0; e < unculled_indices->size(); e++) {
    std::vector<unsigned int>* sub_indices = (*unculled_indices)[e];
    for (int i = 0; i < sub_indices->size(); i++) {
      unsigned int r = (*sub_indices)[i];

      geo::v3<T> e_centroid = (*e_centroids)[e];
      geo::tri<T> r_tri = (*r_triangles)[r];
      geo::v3<T> r_centroid = geo::centroid(r_tri);

      geo::v3<T> ray_vector = r_centroid - e_centroid;
      T ray_length = geo::magnitude(ray_vector);
      geo::ray<T> cast_ray( e_centroid, geo::normalize(ray_vector) );

      intersectRayWithBVH(&cast_ray, *bvh, blocking_mesh, ray_length);
      bool blocked = ( cast_ray._t < ray_length );
      if (blocked) {
        (*sub_indices)[i] = problem_size;
      }
    }
    auto it = std::remove(sub_indices->begin(), sub_indices->end(), problem_size);
    sub_indices->erase(it, sub_indices->end());
  }
}



template <typename T> T doubleAreaIntegration(geo::v3<T> e_centroid, geo::v3<T> e_normal, geo::v3<T> r_centroid, geo::v3<T> r_normal, T r_area) {
  geo::v3<T> ray_vector = r_centroid - e_centroid;
  
  T denominator = geo::dot(ray_vector, ray_vector) * geo::dot(ray_vector, ray_vector) * std::numbers::pi;
  T scale_factor = -1.0 / denominator;

  T fij = geo::dot(e_normal, ray_vector) * geo::dot(r_normal, ray_vector) * r_area * scale_factor;
  return fij;
}


template <typename T> T singleAreaIntegration(geo::v3<T> e_centroid, geo::v3<T> e_normal, geo::v3<T> r_OA, geo::v3<T> r_OB, geo::v3<T> r_OC) {
  std::array<geo::v3<T>, 3> cast_rays = {
    r_OA - e_centroid,
    r_OB - e_centroid,
    r_OC - e_centroid };
  std::array<geo::v3<T>, 3> section_normals = {
    geo::cross(cast_rays[1], cast_rays[0]),
    geo::cross(cast_rays[2], cast_rays[1]),
    geo::cross(cast_rays[0], cast_rays[2]) };

  std::array<T, 3> section_projections = {
    geo::dot(cast_rays[1], cast_rays[0]),
    geo::dot(cast_rays[2], cast_rays[1]),
    geo::dot(cast_rays[0], cast_rays[2]) };

  T edge_integral = 0.0;
  for (int i = 0; i < 3; i++) {
    geo::v3<T> section_normal = section_normals[i];
    T denominator = geo::magnitude(section_normal);
    T scale_factor = 1.0 / denominator;
    edge_integral += geo::dot(section_normal, e_normal) * scale_factor * ( std::numbers::pi * 0.5 - std::atan( section_projections[i] * scale_factor ) );
  }

  T scale_factor = 1.0 / ( 2.0 * std::numbers::pi );
  T fij = edge_integral * scale_factor;
  return fij;
}


template <typename T> void viewFactors(std::vector<geo::v3<T>>* e_centroids, std::vector<geo::v3<T>>* e_normals, std::vector<geo::tri<T>>* r_triangles, std::vector<std::vector<unsigned int>*>* unculled_indices, std::vector<std::vector<T>*>* view_factors, std::string solver_mode) {
  for (int e = 0; e < e_centroids->size(); e++) {

    std::vector<unsigned int>* sub_indices = (*unculled_indices)[e];
    std::vector<T>* sub_results = (*view_factors)[e];

    #pragma omp parallel for
    for (int i = 0; i < sub_indices->size(); i++) {

      unsigned int r = (*sub_indices)[i];
      geo::tri<T> r_triangle = (*r_triangles)[r];

      if (solver_mode == "DAI") {

        (*sub_results)[i] = doubleAreaIntegration( (*e_centroids)[e], (*e_normals)[e], geo::centroid(r_triangle), geo::normal(r_triangle), geo::area(r_triangle) );
  
      } else if (solver_mode == "SAI") {
  
        (*sub_results)[i] = singleAreaIntegration( (*e_centroids)[e], (*e_normals)[e], r_triangle[0], r_triangle[1], r_triangle[2] );
  
      }
    }
  }
}
  
  
}