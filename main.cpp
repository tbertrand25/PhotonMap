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
#include "Ray.hpp"
#include "Hit.hpp"

#include "Tracer.h"

namespace
{
// Processes a rayfile from an istream and returns the resulting Scene object
Scene process_rayfile(std::istream &ins)
{
  // Begin tag for rayfile
  const std::string BEGIN_FRAME = "begin_frame";
  
  // Begin tag for sphere definition
  const std::string BEGIN_SPHERE = "begin_sphere";
  
  // Begin tag for view definition
  const std::string BEGIN_VIEW = "begin_view";
  
  // Holds current chunk of istream
  std::string tok;
  
  bool seen_view = false;
  
  View img_view;
  std::vector<std::shared_ptr<Surface>> surfaces;
  
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
  }
  return Scene(img_view, surfaces);
}
}

int main(int argc, const char * argv[]) {
  Scene img_scene = process_rayfile(std::cin);
  
  std::shared_ptr<Hit> h;
  std::shared_ptr<Ray> r;
  
  std::cout << "P3" << std::endl;
  std::cout << img_scene.get_view().get_x_res() << " " << img_scene.get_view().get_y_res() << std::endl;
  std::cout << "255" << std::endl;
  
  for(int row = 0; row < img_scene.get_view().get_y_res(); row++)
  {
    for(int col = 0; col < img_scene.get_view().get_x_res(); col++)
    {
      arma::vec3 dir = tracer::calc_view_ray_direction(row, col, img_scene.get_view());
      
      r = std::make_shared<Ray>(img_scene.get_view().get_eye(), dir);
      h = std::make_shared<Hit>();
      
      arma::vec3 color = tracer::raycolor(r, h, img_scene);
      //std::clog << "[" << color(0) << ", " << color(1) << ", " << color(2) << "]" << std::endl;
      std::cout << int(color(0) * 255) << " " << int(color(1) * 255) << " " << int(color(2) * 255) << " ";
      
      //std::clog << "[" << row << ", " << col << "]" << std::endl;
      
      //h = std::make_shared<Hit>();
      //r = std::make_shared<Ray>(arma::vec3({0, 0, 5}), arma::vec3({0, 0, -1}));
    }
  }
  
  //std::clog << "MAIN:\n" << tracer::raycolor(r, h, img_scene) << std::endl;
  
  //std::clog << img_scene << std::endl;
  
  return 0;
}

