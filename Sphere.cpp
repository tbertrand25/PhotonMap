//
//  Sphere.cpp
//  PhotonMap
//
//  Created by Tyler Bertrand on 5/27/16.
//  Copyright © 2016 Tyler Bertrand. All rights reserved.
//

#include "Sphere.hpp"

Sphere::Sphere(std::istream &ins)
{
  read(ins);
}

Sphere::~Sphere()
{
  
}

bool Sphere::intersect(Ray *r, Hit *h)
{
  return true;
}

void Sphere::read(std::istream &ins)
{
  const std::string END_SPHERE_TAG = "end_sphere";
  
  const std::string POSITION_TAG = "position";
  const std::string RADIUS_TAG = "radius";
  const std::string COLOR_TAG = "color";
  
  bool seen_end_tag = false;
  bool seen_position = false;
  bool seen_radius = false;
  bool seen_color = false;
  
  std::string tok;
  
  while(!ins.eof() && !seen_end_tag)
  {
    ins >> tok;
    
    if(tok == END_SPHERE_TAG)
    {
      seen_end_tag = true;
    }
    else if(tok == POSITION_TAG)
    {
      seen_position = true;
      
      ins >> center(0);
      ins >> center(1);
      ins >> center(2);
    }
    else if(tok == RADIUS_TAG)
    {
      seen_radius = true;
      ins >> radius;
    }
    else if(tok == COLOR_TAG)
    {
      seen_color = true;
      
      ins >> color(0);
      ins >> color(1);
      ins >> color(2);
    }
  }
}

void Sphere::print(std::ostream &os)
{
  os << "radius: " << radius << "\n";
  os << "center: [" << center(0) << ", " << center(1) << ", " << center(2) << "]\n";
  os << "color: [" << color(0) << ", " << color(1) << ", " << color(2) << "]";
}