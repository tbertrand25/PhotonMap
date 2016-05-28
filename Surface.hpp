//
//  Surface.hpp
//  PhotonMap
//
//  Created by Tyler Bertrand on 5/27/16.
//  Copyright © 2016 Tyler Bertrand. All rights reserved.
//

#ifndef Surface_hpp
#define Surface_hpp

#include <stdio.h>
#include <armadillo>

#include "Hit.hpp"
#include "Ray.hpp"

class Surface
{
private:
  virtual void read(std::istream &ins) = 0;
  
public:
  arma::vec3 color;
  
  Surface();
  ~Surface();
  
  virtual void print(std::ostream &os) = 0;
  virtual bool intersect(Ray *r, Hit *h) = 0;
};

#endif /* Surface_hpp */
