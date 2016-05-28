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

class Hit
{
private:
  double t;
  
public:
  Hit(double t);
  ~Hit();
};

#endif /* Hit_hpp */
