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
#include <memory>
#include <armadillo>

#include "Hit.hpp"
#include "Ray.hpp"

class Surface
{
private:
  virtual void read(std::istream &ins) = 0;
  
protected:
  arma::vec3 color;
  
public:
  virtual bool intersect(std::shared_ptr<Ray> r, std::shared_ptr<Hit> h) = 0;
  virtual void print(std::ostream &os) = 0;
};

#endif /* Surface_hpp */
