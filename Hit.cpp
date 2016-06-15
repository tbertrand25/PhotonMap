//
//  Hit.cpp
//  PhotonMap
//
//  Created by Tyler Bertrand on 5/27/16.
//  Copyright © 2016 Tyler Bertrand. All rights reserved.
//

#include "Hit.hpp"

Hit::Hit()
{

}

Hit::Hit(double t, arma::vec3 pt, arma::vec3 normal, std::shared_ptr<Material> mat)
{
  this->t = t;
  this->pt = pt;
  this->normal = normal;
  this->mat = mat;
}

Hit::Hit(double t, arma::vec3 pt, arma::vec3 normal, arma::vec2 tex_coords, std::shared_ptr<Material> mat)
{
  this->t = t;
  this->pt = pt;
  this->normal = normal;
  this->mat = mat;
  this->tex_coords = tex_coords;
}

Hit::~Hit()
{
  
}

double Hit::get_t()
{
  return t;
}

arma::vec3 Hit::get_normal()
{
  return normal;
}

arma::vec3 Hit::get_pt()
{
  return pt;
}

std::shared_ptr<Material> Hit::get_material()
{
  return mat;
}

arma::vec2 Hit::get_tex_coords()
{
  return tex_coords;
}

Hit& Hit::operator= (const Hit &h)
{
  t = h.t;
  pt = h.pt;
  normal = h.normal;
  tex_coords = h.tex_coords;
  mat = h.mat;
  
  return *this;
}