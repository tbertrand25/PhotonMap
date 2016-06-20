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

namespace tracer
{
  static const double epsilon = 0.00001;
  
  arma::vec3 raycolor(std::shared_ptr<Ray> r, std::shared_ptr<Hit> h, Scene s, int hit_ct);
  bool in_shadow(std::shared_ptr<Ray> r, Scene s);
  arma::vec3 reflection(std::shared_ptr<Ray> r, std::shared_ptr<Hit> h, Scene s, int hit_ct);
  arma::vec3 calc_view_ray_direction(double row, double col, View v);
  arma::vec3 phong_shade(std::shared_ptr<Ray> r, std::shared_ptr<Hit> h, Scene s, int hit_ct);
  void print_vec3(arma::vec3 a);
  
  template <typename T> int signum(T val) {
    return (T(0) < val) - (val < T(0));
  }
}

#endif /* Tracer_hpp */
