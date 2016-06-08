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

Hit::Hit(double t, arma::vec3 color)
{
  this->t = t;
  this->color = color;
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

Hit& Hit::operator= (const Hit &h)
{
  t = h.t;
  color = h.color;
  
  return *this;
}