//
//  Scene.cpp
//  PhotonMap
//
//  Created by Tyler Bertrand on 5/26/16.
//  Copyright © 2016 Tyler Bertrand. All rights reserved.
//

#include "Scene.hpp"

Scene::Scene(View v, std::vector<Surface*> s)
{
  viewer = v;
  surfaces = s;
}

Scene::~Scene()
{
  
}

std::vector<Surface*> Scene::get_surfaces()
{
  return surfaces;
}

View Scene::get_view()
{
  return viewer;
}

std::ostream &operator << (std::ostream &os, const Scene &s )
{
  os << "\n" << s.viewer;
  
  for(Surface* surf : s.surfaces)
  {
    os << "\n";
    surf->print(os);
    os << "\n";
  }
  
  return os;
}