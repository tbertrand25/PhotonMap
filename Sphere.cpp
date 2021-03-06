//
//  Sphere.cpp
//  PhotonMap
//
//  Created by Tyler Bertrand on 5/27/16.
//  Copyright © 2016 Tyler Bertrand. All rights reserved.
//

#include <math.h>
#include <limits>

#include "Sphere.hpp"

Sphere::Sphere(std::istream &ins)
{
  read(ins);
}

Sphere::~Sphere()
{
  
}

double Sphere::get_radius() {
  return radius;
}

arma::vec3 Sphere::get_center() {
  return center;
}

// Tests r for intersection against <this>.
// Returns true if r intersects <this> in front of the viewing eye, else
//   returns false.
// Hit stores t value for the intersection.
bool Sphere::intersect(std::shared_ptr<Ray> r, std::shared_ptr<Hit> h)
{
  double t, t0, t1;
  
  arma::vec3 L = center - r->get_origin();
  float tca = arma::dot(L, r->get_direction());
  
  if(tca < 0)
    return false;
  
  double d2 = arma::dot(L, L) - (tca * tca);
  
  if(d2 < 0)
    return false;
  if(d2 > (radius * radius))
    return false;
  else
  {
    double thc = sqrt((radius * radius) - d2);
    t0 = tca - thc;
    t1 = tca + thc;
  }
  
  if(t0 < 0 && t1 < 0)
    return false;
  else if(t0 < t1 && t0 >= 0)
    t = t0;
  else
    t = t1;
  
  if(t < h->get_t())
  {
    arma::vec3 pt = r->get_origin() + (t * r->get_direction());
    arma::vec3 normal = arma::normalise(pt - center);
    *h = Hit(t, pt, normal, mat);
  }
  
  return true;
}

// Reads a sphere object from istream and initializes the Sphere object
void Sphere::read(std::istream &ins)
{
  const std::string END_SPHERE_TAG = "end_sphere";
  
  const std::string POSITION_TAG = "position";
  const std::string RADIUS_TAG = "radius";
  const std::string MATERIAL_TAG = "begin_material";
  
  bool seen_end_tag = false;
  bool seen_position = false;
  bool seen_radius = false;
  bool seen_material = false;
  
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
    else if(tok == MATERIAL_TAG)
    {
      seen_material = true;
      mat = std::make_shared<Material>(ins);
    }
  }
  // TODO: check that all required parameters have been seen
}

void Sphere::print(std::ostream &os)
{
  os << "radius: " << radius << "\n";
  os << "center: [" << center(0) << ", " << center(1) << ", " << center(2) << "]\n";
  mat->print(os);
}