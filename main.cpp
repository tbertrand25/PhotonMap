//
//  main.cpp
//  PhotonMap
//
//  Created by Tyler Bertrand on 5/26/16.
//  Copyright © 2016 Tyler Bertrand. All rights reserved.
//

#include <iostream>
#include <stdio.h>
#include <math.h>
#include <memory>
#include <armadillo>
#include <fstream>
#include <iomanip>

#include "Scene.hpp"
#include "View.hpp"
#include "Sphere.hpp"
#include "Quad.hpp"
#include "Ray.hpp"
#include "Hit.hpp"
#include "Light.hpp"
#include "PhotonMap.hpp"
#include "Tracer.hpp"

namespace
{
  const int max_photons = 100000000;
  const int num_photons = 1000000;
  std::vector<std::shared_ptr<Sphere>> dielectric_spheres;
  
  // Processes a rayfile from an istream and returns the resulting Scene object
  Scene process_rayfile(std::istream &ins)
  {
    // Begin tag for rayfile
    const std::string BEGIN_FRAME = "begin_frame";
    
    // Begin tag for sphere definition
    const std::string BEGIN_SPHERE = "begin_sphere";
    
    // Begin tag for quad definition
    const std::string BEGIN_QUAD = "begin_quad";
    
    // Begin tag for view definition
    const std::string BEGIN_VIEW = "begin_view";
    
    // Begin tag for light definition
    const std::string BEGIN_LIGHT = "begin_light";
    
    // Holds current chunk of istream
    std::string tok;
    
    bool seen_view = false;
    
    View img_view;
    std::vector<std::shared_ptr<Surface>> surfaces;
    std::vector<std::shared_ptr<Light>> lights;
    
    // - added for photon map -
    std::shared_ptr<Sphere> sph;
    
    while(!ins.eof())
    {
      ins >> tok;
      
      if(tok == BEGIN_VIEW && !seen_view)
      {
        img_view = View(ins);
      }
      else if(tok == BEGIN_SPHERE)
      {
        sph = std::make_shared<Sphere>(ins);
        if(sph->get_material()->get_is_refractive())
          dielectric_spheres.push_back(sph);
        surfaces.push_back(sph);
      }
      else if(tok == BEGIN_QUAD)
      {
        surfaces.push_back(std::make_shared<Quad>(ins));
      }
      else if(tok == BEGIN_LIGHT)
      {
        lights.push_back(std::make_shared<Light>(ins));
      }
    }
    return Scene(img_view, surfaces, lights);
  }
  
  bool photon_trace(Photon_map &global_map,
                    float power[3],
                    std::shared_ptr<Ray> r,
                    std::shared_ptr<Hit> h,
                    Scene s) {
    
    if(tracer::raytrace(r, h, s)) {
      auto mat = h->get_material();
      //float power[3] = {1,1,1};
      float *pos, *dir;
      
      if(!mat->get_is_refractive()) {
        pos = tracer::vec3_to_array(h->get_pt());
        dir = tracer::vec3_to_array(r->get_direction());
        global_map.store(power, pos, dir);
        
        // determine if reflected or absorbed
        float roll = (rand()/(float)(RAND_MAX));
        if(roll < 0.6) {
          
//          float x, y, z;
//          arma::vec3 ref_dir;
//          do {
//            x = 2*(rand()/(float)(RAND_MAX)) - 1;
//            y = 2*(rand()/(float)(RAND_MAX)) - 1;
//            z = 2*(rand()/(float)(RAND_MAX)) - 1;
//            ref_dir = arma::normalise(arma::vec3({x, y, z}));
//          } while (arma::dot(ref_dir, h->get_normal()) < 0);
          
          arma::vec3 ref_dir = tracer::hemisphere_sample(h->get_normal());
          
          arma::vec3 ref_origin = h->get_pt();
          
          auto ref_r = std::make_shared<Ray>(ref_origin + (tracer::epsilon * h->get_normal()), ref_dir);
          auto ref_h = std::make_shared<Hit>();
          
//          power[0] = (mat->get_color()[0] + power[0]) / 2;
//          power[1] = (mat->get_color()[1] + power[1]) / 2;
//          power[2] = (mat->get_color()[2] + power[2]) / 2;
          
          power[0] = mat->get_color()[0];
          power[1] = mat->get_color()[1];
          power[2] = mat->get_color()[2];

          
          photon_trace(global_map, power, ref_r, ref_h, s);
        }
      }
      else {
        arma::vec3 ref_dir = arma::normalise(r->get_direction() - (2*arma::dot(r->get_direction(), h->get_normal()) * h->get_normal()));
        arma::vec3 ref_origin = h->get_pt();
        
        auto ref_r = std::make_shared<Ray>(ref_origin + (tracer::epsilon * h->get_normal()), ref_dir);
        auto ref_h = std::make_shared<Hit>();
        
        photon_trace(global_map, power, ref_r, ref_h, s);
      }
    }
    else
      return false;
    return true;
  }
  
  void emit_photons_pt_light(arma::vec3 pt, Photon_map &global_map, Scene s)
  {
    float emitted_photons = 0.0;
    float x=0, y=0, z=0;
    
    arma::vec3 direction, origin;
    std::shared_ptr<Hit> h;
    std::shared_ptr<Ray> r;
    
    while(emitted_photons < num_photons)
    {
//      do {
//        x = 2*(rand()/(float)(RAND_MAX)) - 1;
//        y = 2*(rand()/(float)(RAND_MAX)) - 1;
//        z = 2*(rand()/(float)(RAND_MAX)) - 1;
//        direction = arma::normalise(arma::vec3({x, y, z}));
//      } while(direction(1) >= 0);
      
      direction = tracer::hemisphere_sample(arma::vec3({0,-1,0}));
      
      x = (rand()/(float)(RAND_MAX)) - 0.5;
      z = (rand()/(float)(RAND_MAX)) - 0.5;
      origin = arma::vec3({pt(0) + x, pt(1), pt(2) + z});
      //origin = pt;
      r = std::make_shared<Ray>(origin, direction);
      h = std::make_shared<Hit>();
      float power[3] = {1,1,1};
        
      if(photon_trace(global_map, power, r, h, s))
        emitted_photons++;
    }
    global_map.scale_photon_power(1.0/emitted_photons);
    
  }
  
  void build_caustics_map(Photon_map &map,
                          std::vector<std::shared_ptr<Sphere>> dielectric_spheres,
                          Scene s) {
    
    float emitted_photons = 0.0;
    float x=0, y=0, z=0;
    double radius;
    arma::vec3 center;
    std::shared_ptr<Material> mat;
    arma::vec3 direction;
    double n1 = 1.0, n2;
    
    std::vector<std::shared_ptr<Light>> lights = s.get_lights();
    std::vector<std::shared_ptr<Surface>> surfaces = s.get_surfaces();
    arma::vec3 light = lights[0]->get_position();
    
    float pow[3] = {float(lights[0]->get_color()(0)),
                    float(lights[0]->get_color()(1)),
                    float(lights[0]->get_color()(2))};
    
    float pos[3], dir[3];
    
    for(auto sph : dielectric_spheres) {
      radius = sph->get_radius();
      center = sph->get_center();
      n2 = sph->get_material()->get_refract_index();
      
      while(emitted_photons < 10000) {
        //use rejection sampling to generate points inside the sphere
        do {
          x = 2*(rand()/(float)(RAND_MAX)) - 1;
          y = 2*(rand()/(float)(RAND_MAX)) - 1;
          z = 2*(rand()/(float)(RAND_MAX)) - 1;
          
          x *= radius;
          y *= radius;
          z *= radius;
        } while(sqrt(x*x + y*y + z*z) > radius);
        
        direction = arma::normalise((arma::vec3({x, y, z}) + center) - light);
        
        auto r = std::make_shared<Ray>(light, direction);
        auto h = std::make_shared<Hit>();
        
        if(sph->intersect(r, h)) {
          auto refract_r = std::make_shared<Ray>();
          auto refract_h = std::make_shared<Hit>();
          
          tracer::refract(r->get_direction(),
                          h->get_pt(),
                          h->get_normal(),
                          n1,
                          n2,
                          refract_r);
          
          if(sph->intersect(refract_r, refract_h)) {
            auto exit_r = std::make_shared<Ray>();
            auto exit_h = std::make_shared<Hit>();
            
            tracer::refract(refract_r->get_direction(),
                            refract_h->get_pt(),
                            -refract_h->get_normal(),
                            n2,
                            n1,
                            exit_r);
            
            if(tracer::raytrace(exit_r, exit_h, s)) {
              pos[0] = exit_h->get_pt()(0);
              pos[1] = exit_h->get_pt()(1);
              pos[2] = exit_h->get_pt()(2);
              
              dir[0] = exit_r->get_direction()(0);
              dir[1] = exit_r->get_direction()(1);
              dir[2] = exit_r->get_direction()(2);
              
              map.store(pow, pos, dir);
            }
          }
          else {
            std::cerr << "ERR: refracted ray missed sphere" << std::endl;
          }
        }
        else {
          std::cerr << "ERR: caustic photon missed object" << std::endl;
        }
        
        emitted_photons++;
      }
    }
    map.scale_photon_power(1/emitted_photons);
  }
}

int main(int argc, const char * argv[]) {
  srand(time(NULL));
  clock_t start;
  start = clock();
  
  std::ifstream infile;
  const std::string filename = "diffuse.ray";
  infile.open("/Users/tyler/Documents/PhotonMap/PhotonMap/Rayfiles/" + filename);
  
  std::ofstream outfile;
  outfile.open("/Users/tyler/Documents/PhotonMap/PhotonMap/Rayfiles/image.ppm");
  
  Scene img_scene = process_rayfile(infile);
  
  std::clog << "Building caustics map..." << std::endl;
  Photon_map caustics_map(max_photons);
  build_caustics_map(caustics_map, dielectric_spheres, img_scene);
  caustics_map.balance();
  std::clog << "... Done" << std::endl;
  
  std::clog << "Building global map..." << std::endl;
  Photon_map global_map(max_photons);
  emit_photons_pt_light(img_scene.get_lights()[0]->get_position(), global_map, img_scene);
  global_map.balance();
  std::clog << "...Done" << std::endl;
  
  //--------------------------------------------
  //-----  Begin ray tracing  ------------------
  //--------------------------------------------
  
  std::shared_ptr<Hit> h;
  std::shared_ptr<Ray> r;
  
  outfile << "P3" << std::endl;
  
  outfile << img_scene.get_view().get_x_res() << " "
            << img_scene.get_view().get_y_res() << std::endl;
  
  outfile << "255" << std::endl;
  
  int x_res = img_scene.get_view().get_x_res();
  int y_res = img_scene.get_view().get_y_res();
  
  const int samples = 4;
  
  arma::vec3 color, temp_color;
  arma::vec3 dir;
  if(samples > 1)
  {
    for(int row = 0; row < y_res; row++)
    {
      for(int col = 0; col < x_res; col++)
      {
        color = arma::vec3({0,0,0});
        
        for(int i = 0; i < sqrt(samples); i++)
        {
          for(int j = 0; j < sqrt(samples); j++)
          {
            dir = tracer::calc_view_ray_direction(row + ((i + (rand()/(float)(RAND_MAX))) / sqrt(samples)),
                                                  col + ((j + (rand()/(float)(RAND_MAX))) / sqrt(samples)),
                                                  img_scene.get_view());
            
            r = std::make_shared<Ray>(img_scene.get_view().get_eye(), dir);
            h = std::make_shared<Hit>();
            
            temp_color = tracer::raycolor(global_map, caustics_map, r, h, img_scene, 0);

            tracer::clamp_vec3(temp_color);
            color += temp_color;
          }
        }
        
        color /= samples;
        
        for(int i = 0; i < 3; i++) {
          color(i) *= 255;
          color(i) = 255 * pow((color(i)/255), 1/2.2);
          color(i) = int(color(i));
        }
        outfile << color(0) << " "
        << color(1) << " "
        << color(2) << " ";
      }
    }
  }
  else
  {
    for(int row = 0; row < y_res; row++)
    {
      for(int col = 0; col < x_res; col++)
      {
        dir = tracer::calc_view_ray_direction(row, col, img_scene.get_view());
        
        r = std::make_shared<Ray>(img_scene.get_view().get_eye(), dir);
        h = std::make_shared<Hit>();
        
        color = tracer::raycolor(global_map, caustics_map, r, h, img_scene, 0);
        
        tracer::clamp_vec3(color);
        
        for(int i = 0; i < 3; i++) {
          color(i) *= 255;
          color(i) = 255 * pow((color(i)/255), 1/2.2);
          color(i) = int(color(i));
        }
        outfile << color(0) << " "
                << color(1) << " "
                << color(2) << " ";
      }
    }
  }
  
  std::clog << img_scene << std::endl;
  std::clog << "Took " << ( std::clock() - start ) / (double) CLOCKS_PER_SEC << " seconds to render" << std::endl;
  outfile.close();
  return 0;
}

