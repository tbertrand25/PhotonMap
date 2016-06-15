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
#include <algorithm>
#include <math.h>

#include "Scene.hpp"
#include "Material.hpp"

namespace tracer
{
  static const double epsilon = 0.00001;
  
  static arma::vec3 raycolor(std::shared_ptr<Ray> r, std::shared_ptr<Hit> h, Scene s, int hit_ct);
  static bool in_shadow(std::shared_ptr<Ray> r, Scene s);
  static arma::vec3 reflection(std::shared_ptr<Ray> r, std::shared_ptr<Hit> h, Scene s, int hit_ct);
  
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
  
  static arma::vec3 phong_shade(std::shared_ptr<Ray> r, std::shared_ptr<Hit> h, Scene s, int hit_ct)
  {
    
    
    arma::vec3 ambient_term;
    arma::vec3 diffuse_term;
    arma::vec3 specular_term;
    arma::vec3 reflection_term;
    
    arma::vec3 diffuse_temp;
    arma::vec3 specular_temp;
    
    arma::vec3 light_direction;
    
    std::shared_ptr<Material> mat = h->get_material();
    mat->get_phong();
    
    arma::vec3 color;
    if(mat->get_is_textured())
    {
      int x = h->get_tex_coords()(0);
      int y = h->get_tex_coords()(1);
      color = mat->get_tex_color(x, y);
      ambient_term = color;
    }
    else
    {
      color = mat->get_color();
    
    // Scale color by ambient term
    for(int i = 0; i < 3; i++)
      ambient_term(i) = mat->get_ambient()(i) * mat->get_color()(i);
    }
    
    // Calculate diffuse and specular components for each light
    for(auto light : s.get_lights())
    {
      light_direction = arma::normalise(light->get_position() - h->get_pt());
      
      // Create hadow ray
      auto shadow_r = std::make_shared<Ray>(h->get_pt(), light_direction);
      
      // If in shadow, don't add diffuse and specular term for this light
      if(in_shadow(shadow_r, s))
        continue;
      
      // Calculate diffuse term for this light
      diffuse_temp = std::max(double(0), arma::dot(h->get_normal(), light_direction)) * light->get_color();
      
      // Scale diffuse term by color and diffuse scalar
      for(int i = 0; i < 3; i++)
        diffuse_temp(i) *= color(i) * mat->get_diffuse();
      
      // Calculate half vector and specular term for this light
      arma::vec3 half = arma::normalise(((2 * dot(light_direction, h->get_normal())) * h->get_normal()) - light_direction);
      specular_temp = pow(std::max(arma::dot(half, arma::normalise(s.get_view().get_eye() - h->get_pt())), double(0)), mat->get_phong()) * light->get_color();
      
      // Add specular and diffuse term for this light to specular and diffuse totals
      diffuse_term += diffuse_temp;
      specular_term += specular_temp;
    }
    
    // Scale down diffuse and specular totals by the number of lights / 1.75 to prevent over-lighting
    diffuse_term /= s.get_lights().size() / 1.75;
    specular_term /= s.get_lights().size() / 1.75;
    
    // Calculate reflection direction
    arma::vec3 ref_dir = arma::normalise(r->get_direction() - (2*arma::dot(r->get_direction(), h->get_normal()) * h->get_normal()));
    
    // Create reflection ray
    auto ref_ray = std::make_shared<Ray>(h->get_pt(), ref_dir);
   
    // Calculate reflection term
    if(mat->get_is_reflective())
      reflection_term = reflection(ref_ray, h, s, hit_ct);
    
    return ambient_term + diffuse_term + specular_term + reflection_term;
  }
  
  static arma::vec3 raycolor(std::shared_ptr<Ray> r, std::shared_ptr<Hit> h, Scene s, int hit_ct)
  {
    arma::vec3 color({0, 0, 0});
    
    for(auto i : s.get_surfaces())
      i->intersect(r, h);
    
    if(h->get_t() != std::numeric_limits<double>::infinity())
    {
      if(h->get_material()->get_is_phong())
        color = phong_shade(r, h, s, hit_ct);
    }
    else
      color = arma::vec3({0.15,0.15,0.15}) / (hit_ct + 1);
    
    return color;
  }
  
  static bool in_shadow(std::shared_ptr<Ray> r, Scene s)
  {
    auto h = std::make_shared<Hit>();
    
    for(auto i : s.get_surfaces())
      i->intersect(r, h);
    
    if(h->get_t() != std::numeric_limits<double>::infinity())
      return true;
    else
      return false;
  }
  
  static arma::vec3 reflection(std::shared_ptr<Ray> r, std::shared_ptr<Hit> h, Scene s, int hit_ct)
  {
    if(hit_ct < 2)
    {
      auto reflect_h = std::make_shared<Hit>();
      arma::vec3 reflection_term = raycolor(r, reflect_h, s, hit_ct + 1);
      
      for(int i = 0; i < 3; i++)
        reflection_term(i) *= h->get_material()->get_reflectivity()(i);
      
      return reflection_term;
    }
    else
      return arma::vec3({0,0,0});
  }
  
  template <typename T> int signum(T val) {
    return (T(0) < val) - (val < T(0));
  }
  
  static void print_vec3(arma::vec3 a)
  {
    std::clog << "[" << a(0) << ", " << a(1) << ", " << a(2) << "]" << std::endl;
  }
}

#endif /* Tracer_h */
