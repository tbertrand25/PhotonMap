//
//  Quad.hpp
//  PhotonMap
//
//  Created by Tyler Bertrand on 6/11/16.
//  Copyright © 2016 Tyler Bertrand. All rights reserved.
//

#ifndef Quad_hpp
#define Quad_hpp

#include <stdio.h>
#include <vector>
#include <armadillo>
#include <cmath>

#include "Surface.hpp"
#include "Tracer.h"
#include "Material.hpp"

class Quad : public Surface
{
private:
  std::vector<arma::vec3> vertices;
  std::vector<std::vector<int>> vert_order;
  
  void read(std::istream &ins) override;
  
public:
  Quad(std::istream &ins);
  ~Quad();
  
  bool intersect(std::shared_ptr<Ray> r, std::shared_ptr<Hit> h) override;
  void print(std::ostream &os) override;
};

#endif /* Quad_hpp */
