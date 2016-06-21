//
//  Material.cpp
//  PhotonMap
//
//  Created by Tyler Bertrand on 6/13/16.
//  Copyright © 2016 Tyler Bertrand. All rights reserved.
//

#include "Material.hpp"
#include "Tracer.hpp"

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

double Material::get_refract_index()
{
  return refract_index;
}

arma::vec3 Material::get_refract_extinct()
{
  return refract_extinct;
}

unsigned long Material::get_tex_x_size()
{
  return tex_colors.size();
}

unsigned long Material::get_tex_y_size()
{
  return tex_colors.size();
}

arma::vec3 Material::get_tex_color(int x, int y)
{
  return tex_colors[y][x];
}

bool Material::get_is_reflective()
{
  return is_reflective;
}

bool Material::get_is_phong()
{
  return is_phong;
}

bool Material::get_is_textured()
{
  return is_textured;
}

bool Material::get_is_refractive()
{
  return is_refractive;
}

void Material::read(std::istream &ins)
{
  const std::string END_PHONG_MATERIAL_TAG = "end_material";
  const std::string COLOR_TAG = "color";
  const std::string AMBIENT_TAG = "ambient";
  const std::string DIFFUSE_TAG = "diffuse";
  const std::string PHONG_TAG = "phong";
  const std::string REFLECTIVITY_TAG = "reflectivity";
  const std::string TEXTURE_TAG = "texture";
  const std::string REFRACT_EXTINCT_TAG = "refract_extinct";
  const std::string REFRACT_INDEX_TAG = "refract_index";
  
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
      
      is_phong = true;
      ins >> ambient(0);
      ins >> ambient(1);
      ins >> ambient(2);
    }
    else if(tok == DIFFUSE_TAG)
    {
      seen_diffuse = true;
      
      is_phong = true;
      ins >> diffuse;
    }
    else if(tok == PHONG_TAG)
    {
      seen_phong = true;
      
      is_phong = true;
      ins >> phong;
    }
    else if(tok == REFLECTIVITY_TAG)
    {
      seen_reflectivity = true;
      
      ins >> reflectivity(0);
      ins >> reflectivity(1);
      ins >> reflectivity(2);
      
      if(!(reflectivity(0) == 0 &&
           reflectivity(1) == 0 &&
           reflectivity(2) == 0))
        is_reflective = true;
    }
    else if(tok == TEXTURE_TAG)
    {
      is_textured = true;
      ins >> texture;
      
      std::clog << "Loading texture...";
      
      //Create 2-D vector to hold texture colors
      std::vector<std::vector<arma::vec3>> texture_vec;
      std::ifstream texfile;
      texfile.open(texture);
      
      std::string prefix;
      arma::vec2 dimensions;
      int max_color;
      std::vector<arma::vec3> colors;
      arma::vec3 color;
      
      texfile >> prefix;
      texfile >> dimensions(0);
      texfile >> dimensions(1);
      texfile >> max_color;
      
      //Create gmVector3 for each pixel and load into 1-D vector
      while(!texfile.eof())
      {
        texfile >> color(0);
        texfile >> color(1);
        texfile >> color(2);
        for(int i = 0; i < 3; i++)
          color[i] /= max_color;
        colors.push_back(color);
      }
      
      //Load gmVector3 for each pixel from previous 1-D vector into 2-D vector
      std::vector<arma::vec3> row_colors;
      for(int row = 0; row < dimensions[1]; row++)
      {
        for(int col = 0; col < dimensions[0]; col++)
        {
          row_colors.push_back(colors[(row * dimensions[1]) + col]);
        }
        texture_vec.push_back(row_colors);
        row_colors.clear();
      }
      tex_colors = texture_vec;
      
      std::clog << "\rLoading texture... Done" << std::endl;
    }
    else if(tok == REFRACT_INDEX_TAG)
    {
      ins >> refract_index;
      is_refractive = true;
    }
    else if(tok == REFRACT_EXTINCT_TAG)
    {
      is_refractive = true;
      
      ins >> refract_extinct(0);
      ins >> refract_extinct(1);
      ins >> refract_extinct(2);
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
  os << "refract_index: [" << refract_extinct(0) << ", " << refract_extinct(1) << ", " << refract_extinct(2) << "]\n";
  os << "refract_index: " << refract_index << "\n";
}