//
//  Scene.cpp
//  PhotonMap
//
//  Created by Tyler Bertrand on 5/26/16.
//  Copyright © 2016 Tyler Bertrand. All rights reserved.
//

#include "Scene.hpp"

Scene::Scene(View v, std::vector<std::shared_ptr<Surface>> s, std::vector<std::shared_ptr<Light>> l)
{
  viewer = v;
  surfaces = s;
  lights = l;
}

Scene::~Scene()
{
  
}

std::vector<std::shared_ptr<Surface>> Scene::get_surfaces()
{
  return surfaces;
}

std::vector<std::shared_ptr<Light>> Scene::get_lights()
{
  return lights;
}

View Scene::get_view()
{
  return viewer;
}

std::ostream &operator << (std::ostream &os, const Scene &s )
{
  os << "\n" << s.viewer;
  
  for(auto surf : s.surfaces)
  {
    os << "\n";
    surf->print(os);
    os << "\n";
  }
  
  for(auto l : s.lights)
  {
    os << *l << "\n";
  }
  
  return os;
}