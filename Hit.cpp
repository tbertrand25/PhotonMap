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

Hit& Hit::operator= (const Hit &h)
{
  t = h.t;
  pt = h.pt;
  normal = h.normal;
  mat = h.mat;
  
  return *this;
}