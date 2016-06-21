//
//  Ray.cpp
//  PhotonMap
//
//  Created by Tyler Bertrand on 5/27/16.
//  Copyright © 2016 Tyler Bertrand. All rights reserved.
//

#include "Ray.hpp"

Ray::Ray(arma::vec3 origin, arma::vec3 direction)
{
  this->origin = origin;
  this->direction = arma::normalise(direction);
}

Ray::Ray()
{
  
}

Ray::~Ray()
{
  
}

arma::vec3 Ray::get_origin()
{
  return origin;
}

arma::vec3 Ray::get_direction()
{
  return direction;
}

Ray& Ray::operator= (const Ray &r)
{
  origin = r.origin;
  direction = r.direction;
  
  return *this;
}