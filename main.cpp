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

#include "Scene.hpp"
#include "View.hpp"
#include "Sphere.hpp"
#include "Quad.hpp"
#include "Ray.hpp"
#include "Hit.hpp"
#include "Light.hpp"

#include "Tracer.hpp"

namespace
{
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
    
    while(!ins.eof())
    {
      ins >> tok;
      
      if(tok == BEGIN_VIEW && !seen_view)
      {
        img_view = View(ins);
      }
      else if(tok == BEGIN_SPHERE)
      {
        surfaces.push_back(std::make_shared<Sphere>(ins));
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
}

int main(int argc, const char * argv[]) {
  Scene img_scene = process_rayfile(std::cin);
  
  std::shared_ptr<Hit> h;
  std::shared_ptr<Ray> r;
  
  std::cout << "P3" << std::endl;
  
  std::cout << img_scene.get_view().get_x_res() << " "
            << img_scene.get_view().get_y_res() << std::endl;
  
  std::cout << "255" << std::endl;
  
  int x_res = img_scene.get_view().get_x_res();
  int y_res = img_scene.get_view().get_y_res();
  
  const int samples = 9;
  
  arma::vec3 color;
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
            
            color += tracer::raycolor(r, h, img_scene, 0);
          }
        }
        
        color /= samples;
        
        std::cout << int(color(0) * 255) << " "
                  << int(color(1) * 255) << " "
                  << int(color(2) * 255) << " ";
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
        
        color = tracer::raycolor(r, h, img_scene, 0);
        
        std::cout << int(color(0) * 255) << " "
        << int(color(1) * 255) << " "
        << int(color(2) * 255) << " ";
      }
    }
  }
  
  std::clog << img_scene << std::endl;
  
  return 0;
}

