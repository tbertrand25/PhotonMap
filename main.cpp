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
  
  auto h = std::make_shared<Hit>();
  
  for(auto i : img_scene.get_surfaces())
  {
    std::clog << i->intersect(std::make_shared<Ray>(arma::vec3({0, 0, 5}), arma::vec3({0, 0, -1})), h) << std::endl;
    std::clog << "  t: " << h->get_t() << std::endl;
  }
  
  std::clog << img_scene << std::endl;
  
  return 0;
}

