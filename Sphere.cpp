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
  
  if(d2 < 0 - 0.00001)
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
  
  if(t < h->get_t() && t > 0.00001)
  {
    arma::vec3 pt = r->get_origin() + (t * r->get_direction());
    arma::vec3 normal = arma::normalise(pt - center);
    *h = Hit(t, pt, normal, color, ambient, diffuse, phong, reflectivity);
  }
  
  return true;
}

// Reads a sphere object from istream and initializes the Sphere object
void Sphere::read(std::istream &ins)
{
  const std::string END_SPHERE_TAG = "end_sphere";
  
  const std::string POSITION_TAG = "position";
  const std::string RADIUS_TAG = "radius";
  const std::string COLOR_TAG = "color";
  const std::string AMBIENT_TAG = "ambient";
  const std::string DIFFUSE_TAG = "diffuse";
  const std::string PHONG_TAG = "phong";
  const std::string REFLECTIVITY_TAG = "reflectivity";
  
  bool seen_end_tag = false;
  bool seen_position = false;
  bool seen_radius = false;
  bool seen_color = false;
  bool seen_ambient = false;
  bool seen_diffuse = false;
  bool seen_phong = false;
  bool seen_reflectivity = false;
  
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
  // TODO: check that all required parameters have been seen
}

void Sphere::print(std::ostream &os)
{
  os << "radius: " << radius << "\n";
  os << "center: [" << center(0) << ", " << center(1) << ", " << center(2) << "]\n";
  os << "color: [" << color(0) << ", " << color(1) << ", " << color(2) << "]\n";
  os << "ambient: [" << ambient(0) << ", " << ambient(1) << ", " << ambient(2) << "]\n";
  os << "diffuse: " << diffuse << "\n";
  os << "phong: " << phong << "\n";
  os << "reflectivity: [" << reflectivity(0) << ", " << reflectivity(1) << ", " << reflectivity(2) << "]\n";
}