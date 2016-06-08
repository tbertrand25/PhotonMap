//
//  Tracer.h
//  PhotonMap
//
//  Created by Tyler Bertrand on 6/8/16.
//  Copyright © 2016 Tyler Bertrand. All rights reserved.
//

#ifndef Tracer_h
#define Tracer_h

#include <armadillo>
#include <limits>
#include <iostream>

#include "Scene.hpp"

namespace tracer
{
  static arma::vec3 calc_view_ray_direction(double row, double col, View v)
  {
    const double x_res = v.get_x_res();
    const double y_res = v.get_y_res();
    const double view_width = v.get_view_width();
    const double view_height = v.get_view_height();
    
    double x_mag = (col - (x_res / 2)) / (x_res / view_width);
    double y_mag = (row - (y_res / 2)) / (y_res / view_height);
    double z_mag = v.get_focal();
    
    arma::vec3 u_vec = v.get_u();
    arma::vec3 v_vec = v.get_v();
    arma::vec3 w_vec = -v.get_w();
    
    u_vec *= x_mag;
    v_vec *= y_mag;
    w_vec *= z_mag;
    
    arma::vec3 viewport_pt = v.get_eye();
    viewport_pt += u_vec;
    viewport_pt += v_vec;
    viewport_pt += w_vec;
    
    return arma::normalise(viewport_pt - v.get_eye());
  }
  
  static arma::vec3 raycolor(std::shared_ptr<Ray> r, std::shared_ptr<Hit> h, Scene s)
  {
    for(auto i : s.get_surfaces())
      i->intersect(r, h);
    
    if(h->get_t() != std::numeric_limits<double>::infinity())
      return h->get_color();
    else
      return arma::vec3({0,0,0});
  }
}

#endif /* Tracer_h */
