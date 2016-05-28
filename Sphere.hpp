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
#include <armadillo>

#include "Surface.hpp"

class Sphere : public Surface
{
private:
  double radius;
  arma::vec3 center;
  
  virtual void read(std::istream &ins) override;
  
public:
  Sphere(std::istream &ins);
  ~Sphere();
  
  void print(std::ostream &os) override;
  bool intersect(Ray *r, Hit *h) override;
};

#endif /* Sphere_hpp */
