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

Hit::Hit(double t)
{
  this->t = t;
}

Hit::~Hit()
{
  
}

void Hit::set_t(double t)
{
  this->t = t;
}

double Hit::get_t()
{
  return t;
}

Hit& Hit::operator= (const Hit &h)
{
  t = h.t;
  
  return *this;
}