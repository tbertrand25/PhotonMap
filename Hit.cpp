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

Hit::Hit(double t, arma::vec3 pt, arma::vec3 normal, arma::vec3 color, arma::vec3 ambient, double diffuse, double phong, arma::vec3 reflectivity)
{
  this->t = t;
  this->pt = pt;
  this->normal = normal;
  this->color = color;
  this->ambient = ambient;
  this->diffuse = diffuse;
  this->phong = phong;
  this->reflectivity = reflectivity;
}

Hit::~Hit()
{
  
}

arma::vec3 Hit::get_color()
{
  return color;
}

double Hit::get_t()
{
  return t;
}

arma::vec3 Hit::get_normal()
{
  return normal;
}

arma::vec3 Hit::get_ambient()
{
  return ambient;
}

double Hit::get_diffuse()
{
  return diffuse;
}

double Hit::get_phong()
{
  return phong;
}

arma::vec3 Hit::get_reflectivity()
{
  return reflectivity;
}

arma::vec3 Hit::get_pt()
{
  return pt;
}

Hit& Hit::operator= (const Hit &h)
{
  t = h.t;
  pt = h.pt;
  normal = h.normal;
  color = h.color;
  ambient = h.ambient;
  diffuse = h.diffuse;
  phong = h.phong;
  reflectivity = h.reflectivity;
  
  return *this;
}