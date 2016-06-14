//
//  Material.cpp
//  PhotonMap
//
//  Created by Tyler Bertrand on 6/13/16.
//  Copyright © 2016 Tyler Bertrand. All rights reserved.
//

#include "Material.hpp"

Material::Material(std::istream &ins)
{
  read(ins);
}

Material::~Material()
{
  
}

arma::vec3 Material::get_color()
{
  return color;
}

arma::vec3 Material::get_ambient()
{
  return ambient;
}

arma::vec3 Material::get_reflectivity()
{
  return reflectivity;
}

double Material::get_diffuse()
{
  return diffuse;
}

double Material::get_phong()
{
  return phong;
}

void Material::read(std::istream &ins)
{
  const std::string END_PHONG_MATERIAL_TAG = "end_material";
  const std::string COLOR_TAG = "color";
  const std::string AMBIENT_TAG = "ambient";
  const std::string DIFFUSE_TAG = "diffuse";
  const std::string PHONG_TAG = "phong";
  const std::string REFLECTIVITY_TAG = "reflectivity";
  
  bool seen_end_tag = false;
  bool seen_color = false;
  bool seen_ambient = false;
  bool seen_diffuse = false;
  bool seen_phong = false;
  bool seen_reflectivity = false;
  
  std::string tok;
  
  while(!ins.eof() && !seen_end_tag)
  {
    ins >> tok;
    
    if(tok == END_PHONG_MATERIAL_TAG)
    {
      seen_end_tag = true;
    }
    else if(tok == COLOR_TAG)
    {
      seen_color = true;
      
      ins >> color(0);
      ins >> color(1);
      ins >> color(2);
    }
    else if(tok == AMBIENT_TAG)
    {
      seen_ambient = true;
      
      ins >> ambient(0);
      ins >> ambient(1);
      ins >> ambient(2);
    }
    else if(tok == DIFFUSE_TAG)
    {
      seen_diffuse = true;
      
      ins >> diffuse;
    }
    else if(tok == PHONG_TAG)
    {
      seen_phong = true;
      
      ins >> phong;
    }
    else if(tok == REFLECTIVITY_TAG)
    {
      seen_reflectivity = true;
      
      ins >> reflectivity(0);
      ins >> reflectivity(1);
      ins >> reflectivity(2);
    }
  }
}

void Material::print(std::ostream &os)
{
  os << "color: [" << color(0) << ", " << color(1) << ", " << color(2) << "]\n";
  os << "ambient: [" << ambient(0) << ", " << ambient(1) << ", " << ambient(2) << "]\n";
  os << "diffuse: " << diffuse << "\n";
  os << "phong: " << phong << "\n";
  os << "reflectivity: [" << reflectivity(0) << ", " << reflectivity(1) << ", " << reflectivity(2) << "]\n";
}