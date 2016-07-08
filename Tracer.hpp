//
//  Tracer.hpp
//  PhotonMap
//
//  Created by Tyler Bertrand on 6/20/16.
//  Copyright © 2016 Tyler Bertrand. All rights reserved.
//

#ifndef Tracer_hpp
#define Tracer_hpp

#include <stdio.h>

#include <armadillo>
#include <limits>
#include <iostream>
#include <algorithm>
#include <math.h>

#include "Scene.hpp"
#include "Material.hpp"
#include "PhotonMap.hpp"

namespace tracer
{
  static const double epsilon = 0.00001;
  
  float* vec3_to_array(arma::vec3 v);
  arma::vec3 array_to_vec3(float *a);
  void clamp_vec3(arma::vec3 &v);
  
  bool raytrace(std::shared_ptr<Ray> r, std::shared_ptr<Hit> h, Scene s);
  
  arma::vec3 raycolor(Photon_map &global_map,
                      Photon_map &caustic_map,
                      std::shared_ptr<Ray> r,
                      std::shared_ptr<Hit> h,
                      Scene s,
                      int hit_ct);
  
  bool in_shadow(std::shared_ptr<Ray> r, Scene s);
  arma::vec3 phong_reflection(Photon_map &global_map, Photon_map &caustic_map, std::shared_ptr<Ray> r, std::shared_ptr<Hit> h, Scene s, int hit_ct);
  arma::vec3 calc_view_ray_direction(double row, double col, View v);
  arma::vec3 phong_shade(Photon_map &global_map, Photon_map &caustic_map, std::shared_ptr<Ray> r, std::shared_ptr<Hit> h, Scene s, int hit_ct);
  arma::vec3 refract_shade(Photon_map &global_map, Photon_map &caustic_map, std::shared_ptr<Ray> r, std::shared_ptr<Hit> h, Scene s, int hit_ct);
  
  bool refract(arma::vec3 dir, arma::vec3 ref_origin, arma::vec3 normal,
               double n1, double n2, std::shared_ptr<Ray> refract_r);
  
  void print_vec3(arma::vec3 a);
  
  arma::vec3 hemisphere_sample(arma::vec3 normal);
  
  template <typename T> int signum(T val) {
    return (T(0) < val) - (val < T(0));
  }
}

#endif /* Tracer_hpp */
