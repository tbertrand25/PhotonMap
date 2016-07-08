//
//  Tracer.cpp
//  PhotonMap
//
//  Created by Tyler Bertrand on 6/20/16.
//  Copyright © 2016 Tyler Bertrand. All rights reserved.
//

#include "Tracer.hpp"

namespace tracer
{
  float* vec3_to_array(arma::vec3 v) {
    float *a;
    a[0] = float(v(0));
    a[1] = float(v(1));
    a[2] = float(v(2));
    return a;
  }
  
  arma::vec3 array_to_vec3(float *a) {
    return arma::vec3({a[0], a[1], a[2]});
  }
  
  void clamp_vec3(arma::vec3 &v) {
    if(v(0) > 1)
      v(0) = 1;
    if(v(1) > 1)
      v(1) = 1;
    if(v(2) > 1)
      v(2) = 1;
  }
  
  arma::vec3 calc_view_ray_direction(double row, double col, View v)
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
  
  arma::vec3 phong_shade(Photon_map &global_map, Photon_map &caustic_map, std::shared_ptr<Ray> r, std::shared_ptr<Hit> h, Scene s, int hit_ct)
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
      auto shadow_r = std::make_shared<Ray>(h->get_pt() + (tracer::epsilon * h->get_normal()), light_direction);
      
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
    
    //Calculate reflection term
    if(mat->get_is_reflective())
      reflection_term = phong_reflection(global_map, caustic_map, ref_ray, h, s, hit_ct);
    
    return ambient_term + diffuse_term + (0.5 * specular_term) + reflection_term;
  }
  
  arma::vec3 refract_shade(Photon_map &global_map, Photon_map &caustic_map, std::shared_ptr<Ray> r, std::shared_ptr<Hit> h, Scene s, int hit_ct)
  {
    arma::vec3 refract_color = arma::vec3({0,0,0});
    
    double refract_index = h->get_material()->get_refract_index();
    const double ext_index = 1.0;
    
    auto refract_r = std::make_shared<Ray>();
    auto refract_h = std::make_shared<Hit>();
    
    refract(r->get_direction(), h->get_pt(), h->get_normal(),
            ext_index, refract_index, refract_r);
    
    raytrace(refract_r, refract_h, s);
    
    auto exit_r = std::make_shared<Ray>();
    auto exit_h = std::make_shared<Hit>();
    
    refract(refract_r->get_direction(), refract_h->get_pt(), -refract_h->get_normal(),
            refract_index, ext_index, exit_r);
    
    refract_color = raycolor(global_map, caustic_map, exit_r, exit_h, s, hit_ct + 1);
    
    //Calculate R0, c, and R
    double R0, R, c, kr, kg, kb;
    R0 = ((ext_index - 1)*(ext_index - 1))/((ext_index + 1)*(ext_index + 1));
    c = arma::dot(refract_r->get_direction(), refract_h->get_normal());
    R = R0 + ((1 - R0)*pow((1 - c),5));
    
    //Calculate extinction
    arma::vec3 a = h->get_material()->get_refract_extinct();
    kr = exp(-log(a[0])*refract_h->get_t());
    kg = exp(-log(a[1])*refract_h->get_t());
    kb = exp(-log(a[2])*refract_h->get_t());
    
    //Apply extinction
    refract_color *= (1 - R);
    refract_color[0] *= kr;
    refract_color[1] *= kg;
    refract_color[2] *= kb;
    
    arma::vec3 half;
    arma::vec3 specular = arma::vec3({0,0,0});
    arma::vec3 light_direction;
    for(auto light : s.get_lights())
    {
      light_direction = arma::normalise(light->get_position() - h->get_pt());
      
      // Calculate half vector and specular term for this light
      half = arma::normalise(((2 * dot(light_direction, h->get_normal())) * h->get_normal()) - light_direction);
      specular += pow(std::max(arma::dot(half, arma::normalise(s.get_view().get_eye() - h->get_pt())), double(0)), 55) * light->get_color();
    }
    
    arma::vec3 reflection_color = arma::vec3({0, 0, 0});
    const double ref_scalar = 0.2;
    
    // Create reflection ray
    // Calculate reflection direction
    arma::vec3 ref_dir = arma::normalise(r->get_direction() - (2*arma::dot(r->get_direction(), h->get_normal()) * h->get_normal()));
    auto ref_ray = std::make_shared<Ray>(h->get_pt(), ref_dir);
//
//    //Calculate reflection term
//    if(hit_ct < 2)
//    {
//      auto reflect_h = std::make_shared<Hit>();
//      reflection_color = ref_scalar * raycolor(global_map, caustic_map, ref_ray, reflect_h, s, hit_ct + 1);
//      tracer::clamp_vec3(reflection_color);
//    }
    auto reflect_h = std::make_shared<Hit>();
    
    if(raytrace(ref_ray, reflect_h, s))
    {
      float caustic_irrad[3];
      float *pos, *normal;
      normal = vec3_to_array(reflect_h->get_normal());
      pos = vec3_to_array(reflect_h->get_pt());
      caustic_map.irradiance_estimate(caustic_irrad, pos, normal, 0.1, 100);
      reflection_color = array_to_vec3(caustic_irrad);
    }
    
    return refract_color + specular + reflection_color;
  }
  
  bool refract(arma::vec3 dir, arma::vec3 refract_origin, arma::vec3 normal,
               double n1, double n2, std::shared_ptr<Ray> refract_r)
  {
    arma::vec3 term1, term2;
    double term2_sqr;
    
    //calculate term to be sqrt'd first to check if negative
    term2_sqr = 1 - (((n1*n1)*(1-(arma::dot(dir,normal)*arma::dot(dir,normal)))) / (n2 * n2));
    
    if(term2_sqr >= 0)
      term2 = normal * sqrt(term2_sqr);
    else
      return false;
    
    term1 = (n1 * (dir - (arma::dot(dir, normal) * normal))) / n2;
    
    arma::vec3 ref_dir = arma::normalise((term1 - term2));
    arma::vec3 ref_origin = refract_origin + (-tracer::epsilon * normal);
    
    *refract_r = Ray(ref_origin, ref_dir);
    
    return true;
  }
  
  arma::vec3 raycolor(Photon_map &global_map, Photon_map &caustic_map, std::shared_ptr<Ray> r, std::shared_ptr<Hit> h, Scene s, int hit_ct)
  {
    arma::vec3 color({0, 0, 0});
    
    for(auto i : s.get_surfaces())
      i->intersect(r, h);
    
    if(h->get_t() != std::numeric_limits<double>::infinity())
    {
      auto mat = h->get_material();
      
      if(h->get_material()->get_is_phong()) {
        arma::vec3 mat_color;
        float caustic_irrad[3], direct_irrad[3];
        float *pos, *normal;
        pos = vec3_to_array(h->get_pt());
        normal = vec3_to_array(h->get_normal());
        
        for(int i = 0; i < 3; i++)
        {
          if(h->get_material()->get_is_textured()) {
            mat_color(i) *= h->get_material()->get_tex_color(h->get_tex_coords()(0),
                                                         h->get_tex_coords()(1))(i);
          }
          else {
            mat_color(i) = h->get_material()->get_color()(i);
          }
        }
        
        caustic_map.irradiance_estimate(caustic_irrad, pos, normal, 0.15, 500);
        global_map.irradiance_estimate(direct_irrad, pos, normal, 0.2, 2000);
      
        arma::vec3 light_dir = arma::normalise(s.get_lights()[0]->get_position() - h->get_pt());
        
        double lamb_dot = arma::dot(h->get_normal(), light_dir);
        
        if(lamb_dot < 0)
          lamb_dot = 0;
        
        arma::vec3 lambertian = lamb_dot * mat_color;
        
        arma::vec3 direct_color = (array_to_vec3(direct_irrad) * 30);

        color = lambertian;
        color = direct_color;
        for(int i = 0; i < 3; i++)
        {
          if(h->get_material()->get_is_textured()) {
            color(i) *= h->get_material()->get_tex_color(h->get_tex_coords()(0),
                                                         h->get_tex_coords()(1))(i);
          }
          else {
            color(i) *= h->get_material()->get_color()(i);
          }
        }
        
//        arma::vec3 gather_dir;
//        arma::vec3 gather_origin;
//        arma::vec3 gather_irrad_temp;
//        arma::vec3 gather_irrad_vec;
//        std::shared_ptr<Ray> gather_r;
//        std::shared_ptr<Hit> gather_h;
//        float gather_irrad[3];
//        
//        for(int i = 0; i < 100; i++) {
//          gather_dir = hemisphere_sample(h->get_normal());
//          gather_origin = h->get_pt() + (tracer::epsilon * gather_dir);
//          
//          gather_r = std::make_shared<Ray>(gather_origin, gather_dir);
//          gather_h = std::make_shared<Hit>();
//          
//          if(raytrace(gather_r, gather_h, s)) {
//            if(!gather_h->get_material()->get_is_refractive()) {
//              pos = vec3_to_array(gather_h->get_pt());
//              normal = vec3_to_array(gather_h->get_normal());
//
//              global_map.irradiance_estimate(gather_irrad, pos, normal, 0.15, 200);
//              gather_irrad_temp = (array_to_vec3(gather_irrad) * 15);
//              
//              for(int i = 0; i < 3; i++) {
//                gather_irrad_temp(i) *= gather_h->get_material()->get_color()(i);
//              }
//              
//              gather_irrad_vec += gather_irrad_temp;
//            }
//          }
//        }
//        gather_irrad_vec /= 100;
        
//        for(int i = 0; i < 3; i++) {
//          color(i) = (color(i)*0.6) + gather_irrad_vec(i);
//        }
        
        color(0) += caustic_irrad[0];
        color(1) += caustic_irrad[1];
        color(2) += caustic_irrad[2];
      }
      else if(h->get_material()->get_is_refractive()) {
        color += refract_shade(global_map, caustic_map, r, h, s, hit_ct);
      }
    }
    else
      color = arma::vec3({0.15,0.15,0.15}) / (hit_ct + 1);
    
    return color;
  }
  
  bool in_shadow(std::shared_ptr<Ray> r, Scene s)
  {
    auto h = std::make_shared<Hit>();
    
    for(auto i : s.get_surfaces())
      i->intersect(r, h);
    
    if(h->get_t() != std::numeric_limits<double>::infinity())
      return true;
    else
      return false;
  }
  
  arma::vec3 phong_reflection(Photon_map &global_map, Photon_map &caustic_map, std::shared_ptr<Ray> r, std::shared_ptr<Hit> h, Scene s, int hit_ct)
  {
    if(hit_ct < 2)
    {
      auto reflect_h = std::make_shared<Hit>();
      arma::vec3 reflection_term = raycolor(global_map, caustic_map, r, reflect_h, s, hit_ct + 1);
      
      for(int i = 0; i < 3; i++)
        reflection_term(i) *= h->get_material()->get_reflectivity()(i);
      
      return reflection_term;
    }
    else
      return arma::vec3({0,0,0});
  }
  
  bool raytrace(std::shared_ptr<Ray> r, std::shared_ptr<Hit> h, Scene s)
  {
    for(auto i : s.get_surfaces())
      i->intersect(r, h);
    
    if(h->get_t() != std::numeric_limits<double>::infinity())
      return true;
    else
      return false;
  }
  
  void print_vec3(arma::vec3 a)
  {
    std::clog << "[" << a(0) << ", " << a(1) << ", " << a(2) << "]" << std::endl;
  }
  
  arma::vec3 hemisphere_sample(arma::vec3 normal) {
    float xi1 = (float)rand()/(float)RAND_MAX;
    float xi2 = (float)rand()/(float)RAND_MAX;
    
    float theta = acos(sqrt(1.0-xi1));
    float phi = 2.0 * 3.14159 * xi2;
    
    float xs = sinf(theta) * cosf(phi);
    float ys = cosf(theta);
    float zs = sinf(theta) * sinf(phi);
    
    arma::vec3 y({normal(0), normal(1), normal(2)});
    arma::vec3 h = y;
    
    if(fabs(h(0)) <= fabs(h(1)) && fabs(h(0)) <= fabs(h(2))) {
      h(0) = 1.0;
    }
    else if(fabs(h(1)) <= fabs(h(0)) && fabs(h(1)) <= fabs(h(2))) {
      h(1) = 1.0;
    }
    else {
      h(2) = 1.0;
    }
    
    arma::vec3 x = arma::cross(h, y);
    arma::vec3 z = arma::cross(x, y);
    
    arma::vec3 direction = xs * x + ys * y + zs * z;
    return arma::normalise(direction);
  }
}