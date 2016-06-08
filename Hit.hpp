//
//  Hit.hpp
//  PhotonMap
//
//  Created by Tyler Bertrand on 5/27/16.
//  Copyright © 2016 Tyler Bertrand. All rights reserved.
//

#ifndef Hit_hpp
#define Hit_hpp

#include <stdio.h>
#include <armadillo>
#include <limits>

class Hit
{
private:
  double t = std::numeric_limits<double>::infinity();
  arma::vec3 color;
  
public:
  Hit();
  Hit(double t, arma::vec3 color);
  ~Hit();
  
  double get_t();
  arma::vec3 get_color();
  
  Hit& operator= (const Hit &h);
};

#endif /* Hit_hpp */
