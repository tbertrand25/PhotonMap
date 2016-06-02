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
  
public:
  Hit();
  Hit(double t);
  ~Hit();
  
  void set_t(double t);
  double get_t();
  
  Hit& operator= (const Hit &h);
};

#endif /* Hit_hpp */
