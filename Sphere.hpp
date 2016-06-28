//
//  Sphere.hpp
//  PhotonMap
//
//  Created by Tyler Bertrand on 5/27/16.
//  Copyright © 2016 Tyler Bertrand. All rights reserved.
//

#ifndef Sphere_hpp
#define Sphere_hpp

#include <stdio.h>
#include <memory>
#include <armadillo>

#include "Surface.hpp"
#include "Tracer.hpp"

class Sphere : public Surface
{
private:
  double radius;
  arma::vec3 center;
  
  void read(std::istream &ins) override;
  
public:
  Sphere(std::istream &ins);
  ~Sphere();
  
  void print(std::ostream &os) override;
  bool intersect(std::shared_ptr<Ray> r, std::shared_ptr<Hit> h) override;
  
  double get_radius();
  arma::vec3 get_center();
};

#endif /* Sphere_hpp */
