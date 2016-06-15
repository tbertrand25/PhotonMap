//
//  Surface.hpp
//  PhotonMap
//
//  Created by Tyler Bertrand on 6/8/16.
//  Copyright © 2016 Tyler Bertrand. All rights reserved.
//

#ifndef Surface_hpp
#define Surface_hpp

#include <stdio.h>
#include <memory>
#include <armadillo>

#include "Hit.hpp"
#include "Ray.hpp"
#include "View.hpp"
#include "Material.hpp"

class Surface
{
private:
  virtual void read(std::istream &ins) = 0;
  
protected:
  std::shared_ptr<Material> mat;
  
public:
  virtual bool intersect(std::shared_ptr<Ray> r, std::shared_ptr<Hit> h) = 0;
  virtual void print(std::ostream &os) = 0;
  
  std::shared_ptr<Material> get_material()
  {
    return mat;
  }
};

#endif /* Surface_hpp */

