//
//  Light.cpp
//  PhotonMap
//
//  Created by Tyler Bertrand on 6/9/16.
//  Copyright © 2016 Tyler Bertrand. All rights reserved.
//

#include "Light.hpp"

Light::Light(std::istream &ins)
{
  read(ins);
}

Light::~Light()
{
  
}

arma::vec3 Light::get_position()
{
  return position;
}

arma::vec3 Light::get_color()
{
  return color;
}

void Light::read(std::istream &ins)
{
  const std::string END_LIGHT_TAG = "end_light";
  
  const std::string POSITION_TAG = "position";
  const std::string COLOR_TAG = "color";
  
  bool seen_end_tag = false;
  bool seen_position = false;
  bool seen_color = false;
  
  std::string tok;
  
  while(!ins.eof() && !seen_end_tag)
  {
    ins >> tok;
    
    if(tok == END_LIGHT_TAG)
    {
      seen_end_tag = true;
    }
    else if(tok == POSITION_TAG)
    {
      seen_position = true;
      
      ins >> position(0);
      ins >> position(1);
      ins >> position(2);
    }
    else if(tok == COLOR_TAG)
    {
      seen_color = true;
      
      ins >> color(0);
      ins >> color(1);
      ins >> color(2);
    }
  }
  // TODO: check that all required parameters have been seen
}

std::ostream &operator << (std::ostream &os, const Light &l)
{
  os << "position: [" << l.position(0) << ", " << l.position(1) << ", " << l.position(2) << "]\n";
  os << "color: [" << l.color(0) << ", " << l.color(1) << ", " << l.color(2) << "]\n";
  return os;
}